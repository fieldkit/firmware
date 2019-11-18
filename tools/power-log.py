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

class FkListener:
    def __init__(self, args):
        self.tasks = []
        self.device_time_queue = queue.Queue()
        self.device_time = None

    def get_device_time(self):
        while not self.device_time_queue.empty():
            ignored = self.device_time_queue.get()
        return self.device_time_queue.get()

    def update_device_time(self, time):
        self.device_time = time
        self.device_time_queue.put(time)

    def started(self):
        self.tasks = []

    def firmware(self, firmware_hash):
        self.firmware_hash = firmware_hash

    def task_started(self, name):
        self.tasks.append(name)
        logging.info("TASKS: %s" % (self.tasks,))

    def task_finished(self, name):
        try:
            self.tasks.remove(name)
            logging.info("TASKS: %s" % (self.tasks,))
        except Exception as e:
            logging.error(e)

    def connection_serviced(self, path):
        pass

    def status(self, name, ip, free):
        pass

class FkLogParser:
    def __init__(self, listener, args):
        self.args = args
        self.listener = listener
        self.re_ansi_escape = re.compile(r'\x1B[@-_][0-?]*[ -/]*[@-~]')
        self.re_starting = re.compile(r'bl: starting')
        self.re_hash = re.compile(r'hash=\'(\w+)\'')
        self.re_alive = re.compile(r'(\w+):\s+alive')
        self.re_dead = re.compile(r'(\w+):\s+dead')
        self.re_routing = re.compile(r'routing \'(.+)\'')
        self.re_status = re.compile(r'\'(.+)\' \((.+)\) \(free = (\d+), arena = (\d+), used = (\d+)\)')

    def handle(self, line):
        clean = self.re_ansi_escape.sub('', line)
        try:
            parts = clean.split(" ")
            self.listener.update_device_time(int(parts[0]))
        except:
            pass

        m = self.re_starting.search(clean)
        if m: self.listener.started()
        m = self.re_hash.search(clean)
        if m: self.listener.firmware(m.group(1))
        m = self.re_alive.search(clean)
        if m: self.listener.task_started(m.group(1))
        m = self.re_dead.search(clean)
        if m: self.listener.task_finished(m.group(1))
        m = self.re_routing.search(clean)
        if m: self.listener.connection_serviced(m.group(1))
        m = self.re_status.search(clean)
        if m: self.listener.status(m.group(1), m.group(2), m.group(3))

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
                logging.info("processing: %d" % (device_time,))

class Joulescope:
    def __init__(self, args, get_device_time):
        self.args = args
        self.get_device_time = get_device_time
        self.queue = queue.Queue()
        self.processor = Processor(self.args, self.queue)

    def start(self):
        self.thread = threading.Thread(target=self.run, args=())
        self.thread.daemon = True
        self.thread.start()
        self.processor.start()

    def on_event_cbk(self, event=0, message=''):
        print("on_event")

    def on_stop_cbk(self, event=0, message=''):
        print("on_stop")

    def on_statistics(self, data):
        print('on_statistics', len(data), data)

    def run(self):
        devices = joulescope.scan()
        devices_length = len(devices)
        if devices_length == 0:
            return

        device = devices[0]

        device.open(event_callback_fn=self.on_event_cbk)
        info = device.info()
        logging.info("device_info: %s" % (json.dumps(info),))

        device.parameter_set('source', 'raw')
        device.parameter_set('i_range', 'auto')

        try:
            while True:
                try:
                    device_time = self.get_device_time()
                    logging.info("starting: %d" % (device_time,))
                    data = device.read(contiguous_duration=30)
                    self.queue.put([device_time, data])
                    logging.info("queueing: %d / %d", (device_time, len(data)))
                except:
                    traceback.print_exc(file=sys.stdout)
                    time.sleep(1)
        finally:
            self.js.close()

async def rtt_listener(loop, listener, js, args):
    parser = FkLogParser(listener, args)
    while True:
        try:
            reader, writer = await asyncio.open_connection('127.0.0.1', args.port)
            while True:
                data = await reader.readline()
                if not data:
                    break
                try:
                    text = data.decode()
                    parser.handle(text)
                    print(text, end='')
                except Exception as e:
                    print(e)
        except Exception as e:
            print(e)
            print(".", end='', flush=True)

        time.sleep(1)

parser = argparse.ArgumentParser(description='fk log monitor tool')
parser.add_argument('--port', dest="port", type=int, default=9400, help="")
parser.add_argument('--joulescope', dest="joulescope", action="store_true", help="")
args, nargs = parser.parse_known_args()

listener = FkListener(args)
js = Joulescope(args, listener.get_device_time)

if args.joulescope:
    js.start()

loop = asyncio.get_event_loop()
loop.run_until_complete(rtt_listener(loop, listener, js, args))
loop.close()
