#!/usr/bin/python3


import asyncio
import joulescope
import json
import sys
import time
import traceback
import argparse
import threading
import re
import logging
import queue
import sqlite3


class Database:
    def __init__(self):
        self.queue = queue.Queue()

    def open(self):
        self.thread = threading.Thread(target=self.inserter, args=())
        self.thread.daemon = True
        self.thread.start()

    def inserter(self):
        self.db = sqlite3.connect("power.db")
        self.db.cursor().execute(
            "CREATE TABLE IF NOT EXISTS raw ("
            "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "  time TIMESTAMP NOT NULL,"
            "  energy NUMERIC NOT NULL,"
            "  message TEXT NOT NULL"
            ")"
        )
        self.db.commit()

        pending = 0

        while True:
            try:
                sql, values = self.queue.get(True, 0.5)
                self.db.cursor().execute(sql, values)
                pending += 1
            except queue.Empty:
                if pending > 0:
                    self.db.commit()
                    pending = 0
            except:
                time.sleep(1)

    def add_raw(self, energy, message):
        values = [time.time(), energy, message]
        self.queue.put(
            ("INSERT INTO raw (time, energy, message) VALUES (?, ?, ?)", values)
        )


class FkListener:
    def __init__(self, args):
        self.db = Database()
        self.db.open()
        self.energy = None

    def get_device_time(self):
        return 0

    def update_energy(self, energy):
        self.energy = energy

    def message(self, message):
        if self.energy:
            self.db.add_raw(self.energy, message)


class FkLogParser:
    def __init__(self, listener, args):
        self.args = args
        self.listener = listener
        self.re_ansi_escape = re.compile(r"\x1B[@-_][0-?]*[ -/]*[@-~]")

    def handle(self, line):
        clean = self.re_ansi_escape.sub("", line).strip()
        self.listener.message(clean)


class Processor:
    def __init__(self, args, queue):
        self.args = args
        self.queue = queue

    def start(self):
        self.thread = threading.Thread(target=self.run, args=())
        self.thread.daemon = True
        self.thread.start()

    def run(self):
        while True:
            while not self.queue.empty():
                device_time, data = self.queue.get()
                logging.info("queue: processing: %d" % (device_time,))


class Joulescope:
    def __init__(self, args, get_device_time, update_energy):
        self.args = args
        self.get_device_time = get_device_time
        self.update_energy = update_energy
        self.queue = queue.Queue()
        self.processor = Processor(self.args, self.queue)
        self.accumulators = {
            "time": 0.0,
            "fields": {"charge": [0.0, 0.0], "energy": [0.0, 0.0],},
        }

    def start(self):
        self.thread = threading.Thread(target=self.run, args=())
        self.thread.daemon = True
        self.thread.start()
        self.processor.start()

    def on_event_cbk(self, event=0, message=""):
        logging.info("js: on_event")

    def on_stop_cbk(self, event=0, message=""):
        logging.info("js: on_stop")

    def on_statistics(self, data):
        self.accumulators["time"] += data["time"]["delta"]
        for field in ["charge", "energy"]:
            x = data["accumulators"][field]["value"]
            z = self.accumulators["fields"][field]
            if x > z[1]:
                z[0] += x - z[1]
                z[1] = x
            else:
                z[0] += x
                z[1] = x
            data["accumulators"][field]["value"] = z[0]
        energy = data["accumulators"]["energy"]["value"]

        self.update_energy(energy)

    def run(self):
        logging.info("js: scanning...")

        devices = joulescope.scan()
        devices_length = len(devices)
        if devices_length == 0:
            logging.info("js: no devices!")
            return

        device = devices[0]

        logging.info("js: opening...")

        device.reduction_frequency = 10
        device.open(event_callback_fn=self.on_event_cbk)
        device.statistics_callback = self.on_statistics
        device.parameter_set("source", "raw")
        device.parameter_set("i_range", "auto")

        info = device.info()
        logging.info("js: device_info: %s" % (json.dumps(info),))
        for p in device.parameters():
            logging.info("js: parameter: %s" % (p.name,))
        logging.info("js: stream duration = %s" % (device.stream_buffer_duration,))
        logging.info("js: sampling frequency %s" % (device.sampling_frequency,))
        logging.info("js: reduction frequency %s" % (device.reduction_frequency,))

        try:
            while True:
                try:
                    if True:
                        logging.info("js: waiting for device time...")
                        device_time = self.get_device_time()
                        logging.info("js: starting: %d" % (device_time,))
                        data = device.read(contiguous_duration=30)
                        self.queue.put([device_time, data])
                        logging.info("js: queueing: %d / %d" % (device_time, len(data)))
                    else:
                        time.sleep(1)
                except:
                    traceback.print_exc(file=sys.stdout)
                    time.sleep(1)
        finally:
            self.js.close()


async def rtt_listener(loop, listener, js, args):
    parser = FkLogParser(listener, args)
    while True:
        try:
            reader, writer = await asyncio.open_connection("127.0.0.1", args.port)

            try:
                while True:
                    data = await reader.readline()
                    if not data:
                        break
                    try:
                        text = data.decode()
                        parser.handle(text)
                        print(text, end="")
                    except Exception as e:
                        print("error decoding", e)
            except Exception as e:
                print("error reading", e)
            finally:
                try:
                    writer.close()
                except Exception as e:
                    print("error closing", e)
        except Exception as e:
            print("error connecting", e)

        time.sleep(1)


if __name__ == "__main__":
    logging.basicConfig(format="%(asctime)-15s %(message)s", level=logging.INFO)

    parser = argparse.ArgumentParser(description="fk log monitor tool")
    parser.add_argument("--port", dest="port", type=int, default=None, help="")
    parser.add_argument("--joulescope", dest="joulescope", action="store_true", help="")
    args, nargs = parser.parse_known_args()

    listener = FkListener(args)
    js = Joulescope(args, listener.get_device_time, listener.update_energy)

    if args.joulescope:
        js.start()

    loop = asyncio.get_event_loop()
    loop.run_until_complete(rtt_listener(loop, listener, js, args))
    loop.close()
