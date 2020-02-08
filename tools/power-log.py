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
        pass

    def open(self):
        self.db = sqlite3.connect('power.db')
        c = self.db.cursor()
        c.execute('CREATE TABLE IF NOT EXISTS samples ('
                  '  id INTEGER PRIMARY KEY AUTOINCREMENT,'
                  '  run INTEGER NOT NULL,'
                  '  device_id TEXT NOT NULL,'
                  '  firmware_hash TEXT NOT NULL,'
                  '  uptime INTEGER NOT NULL,'
                  '  modules TEXT,'
                  '  status BOOLEAN NOT NULL,'
                  '  tasks TEXT NOT NULL,'
                  '  task_start BOOLEAN,'
                  '  task_name TEXT,'
                  '  energy_delta NUMERIC,'
                  '  energy_total NUMERIC NOT NULL'
                  ')')

        self.run = 0
        run = c.execute("SELECT MAX(run) FROM samples").fetchone()[0]
        if run: self.run = run

        self.db.commit()

    def started(self):
        self.run += 1

    def record(self, sample):
        if sample is None:
            return
        values = [
            self.run,
            sample.device_id,
            sample.firmware_hash,
            sample.uptime,
            sample.tasks,
            sample.modules,
            sample.status,
            sample.task_start,
            sample.task_name,
            sample.energy_delta,
            sample.energy_total,
        ]
        self.db.cursor().execute('INSERT INTO samples (run, device_id, firmware_hash, uptime, tasks, modules, status, task_start, task_name, energy_delta, energy_total) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)', values)
        self.db.commit()

class Sample:
    def __init__(self):
        self.device_id = None
        self.firmware_hash = None
        self.uptime = None
        self.modules = ''
        self.status = False
        self.tasks = ''
        self.task_start = None
        self.task_name = None
        self.energy_delta = None
        self.energy_total = 0.0

    def make_status(self):
        c = Sample()
        c.device_id = self.device_id
        c.firmware_hash = self.firmware_hash
        c.uptime = self.uptime
        c.modules = None
        c.status = True
        c.tasks = self.tasks
        c.task_start = False
        c.task_name = None
        c.energy_delta = None
        c.energy_total = self.energy_total
        return c

    def make_started(self, name):
        c = Sample()
        c.device_id = self.device_id
        c.firmware_hash = self.firmware_hash
        c.uptime = self.uptime
        c.modules = None
        c.status = False
        c.tasks = self.tasks
        c.task_start = True
        c.task_name = name
        c.energy_delta = None
        c.energy_total = self.energy_total
        return c

    def make_finished(self, name, delta):
        c = Sample()
        c.device_id = self.device_id
        c.firmware_hash = self.firmware_hash
        c.uptime = self.uptime
        c.modules = self.modules
        c.status = False
        c.tasks = self.tasks
        c.task_start = False
        c.task_name = name
        c.energy_delta = delta
        c.energy_total = self.energy_total
        return c

class FkListener:
    def __init__(self, args):
        self.db = Database()
        self.db.open()
        self.energy_base = 0.0
        self.tasks = []
        self.modules = []
        self.device_time_queue = queue.Queue()
        self.device_time = None
        self.active = {}
        self.sample = Sample()

    def update_energy(self, energy):
        self.sample.energy_total = energy - self.energy_base

    def get_device_time(self):
        while not self.device_time_queue.empty():
            ignored = self.device_time_queue.get()
        return self.device_time_queue.get()

    def update_device_time(self, time):
        self.device_time = time
        self.device_time_queue.put(time)
        self.sample.uptime = time

    def started(self):
        self.energy_base = self.sample.energy_total
        self.tasks = []
        self.sample.tasks = ''
        self.db.started()
        logging.info("STARTED")

    def serial(self, serial):
        self.sample.device_id = serial
        logging.info("SERIAL: %s" % (self.sample.device_id,))

    def firmware(self, firmware_hash):
        self.sample.firmware_hash = firmware_hash
        logging.info("FIRMWARE: %s" % (self.sample.firmware_hash,))

    def task_started(self, name):
        if name == 'readings':
            self.modules = []
        self.active[name] = self.sample.energy_total
        self.tasks.append(name)
        self.sample.tasks = ','.join(self.tasks)
        self.db.record(self.sample.make_started(name))
        logging.info("TASKS: %s" % (self.tasks,))

    def task_finished(self, name):
        delta = self.sample.energy_total - self.active[name]
        self.active[name] = None
        self.tasks.remove(name)
        self.sample.tasks = ','.join(self.tasks)
        self.sample.modules = ','.join(self.modules)
        self.db.record(self.sample.make_finished(name, delta))
        logging.info("TASKS: %s" % (self.tasks,))

    def status(self, name, ip):
        self.db.record(self.sample.make_status())

    def module(self, name, mk, version):
        self.modules.append(name)

    def connection_serviced(self, path):
        pass

class FkLogParser:
    def __init__(self, listener, args):
        self.args = args
        self.listener = listener
        self.re_ansi_escape = re.compile(r'\x1B[@-_][0-?]*[ -/]*[@-~]')
        self.re_starting = re.compile(r'bl: starting')
        self.re_hash = re.compile(r'hash=\'(\w+)\'')
        self.re_serial = re.compile(r'serial=\'([\w-]+)\'')
        self.re_alive = re.compile(r'(\w+):\s+alive')
        self.re_dead = re.compile(r'(\w+):\s+dead')
        self.re_routing = re.compile(r'routing \'(.+)\'')
        self.re_status = re.compile(r'\'(.+)\' \((.+)\) memory')
        self.re_module = re.compile(r'\'(.+)\' mk=(....) version=(\d+)')

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
        m = self.re_serial.search(clean)
        if m: self.listener.serial(m.group(1))
        m = self.re_alive.search(clean)
        if m: self.listener.task_started(m.group(1))
        m = self.re_dead.search(clean)
        if m: self.listener.task_finished(m.group(1))
        m = self.re_routing.search(clean)
        if m: self.listener.connection_serviced(m.group(1))
        m = self.re_status.search(clean)
        if m: self.listener.status(m.group(1), m.group(2))
        m = self.re_module.search(clean)
        if m: self.listener.module(m.group(1), m.group(2), m.group(3))

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
            'time': 0.0,
            'fields': {
                'charge': [0.0, 0.0],
                'energy': [0.0, 0.0],
            },
        }

    def start(self):
        self.thread = threading.Thread(target=self.run, args=())
        self.thread.daemon = True
        self.thread.start()
        self.processor.start()

    def on_event_cbk(self, event=0, message=''):
        logging.info("js: on_event")

    def on_stop_cbk(self, event=0, message=''):
        logging.info("js: on_stop")

    def on_statistics(self, data):
        self.accumulators['time'] += data['time']['delta']
        for field in ['charge', 'energy']:
            x = data['accumulators'][field]['value']
            z = self.accumulators['fields'][field]
            if x > z[1]:
                z[0] += x - z[1]
                z[1] = x
            else:
                z[0] += x
                z[1] = x
            data['accumulators'][field]['value'] = z[0]
        energy = data['accumulators']['energy']['value']

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
        device.parameter_set('source', 'raw')
        device.parameter_set('i_range', 'auto')

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

if __name__ == "__main__":
    logging.basicConfig(format='%(asctime)-15s %(message)s', level=logging.INFO)

    parser = argparse.ArgumentParser(description='fk log monitor tool')
    parser.add_argument('--port', dest="port", type=int, default=None, help="")
    parser.add_argument('--joulescope', dest="joulescope", action="store_true", help="")
    args, nargs = parser.parse_known_args()

    listener = FkListener(args)
    js = Joulescope(args, listener.get_device_time, listener.update_energy)

    if args.joulescope:
        js.start()

    loop = asyncio.get_event_loop()
    loop.run_until_complete(rtt_listener(loop, listener, js, args))
    loop.close()
