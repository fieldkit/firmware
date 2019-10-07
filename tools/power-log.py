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

class FkListener:
    def __init__(self, args):
        self.tasks = []

    def started(self):
        self.tasks = []

    def firmware(self, firmware_hash):
        self.firmware_hash = firmware_hash

    def task_started(self, name):
        self.tasks.append(name)
        print("TASKS", self.tasks)

    def task_finished(self, name):
        try:
            self.tasks.remove(name)
            print("TASKS", self.tasks)
        except Exception as e:
            print(e)

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
        # 2019/10/4 23:44:38 'Ancient Goose 81' (0.0.0.0) (free = 139556, arena = 58072, used = 25796)
        self.re_status = re.compile(r'\'(.+)\' \((.+)\) \(free = (\d+), arena = (\d+), used = (\d+)\)')

    def handle(self, line):
        clean = self.re_ansi_escape.sub('', line)
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

class Joulescope:
    def __init__(self, args):
        self.args = args

    def open(self):
        self.thread = threading.Thread(target=self.run, args=())
        self.thread.daemon = True
        self.thread.start()

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
        print('DEVICE_INFO', json.dumps(info))

        device.parameter_set('source', 'raw')
        device.parameter_set('i_range', 'auto')
        # device.statistics_callback = self.on_statistics
        # device.start(stop_fn=self.on_stop_cbk)

        try:
            while True:
                try:
                    data = device.read(contiguous_duration=30)
                    print(len(data))
                except:
                    traceback.print_exc(file=sys.stdout)
                    time.sleep(1)
        finally:
            self.js.close()

async def rtt_listener(loop, js, args):
    listener = FkListener(args)
    parser = FkLogParser(listener, args)
    while True:
        try:
            print(args.port)
            reader, writer = await asyncio.open_connection('127.0.0.1', args.port)
            while True:
                data = await reader.readline()
                if not data:
                    break
                text = data.decode()
                parser.handle(text)
                print(text, end='')
        except Exception as e:
            print(e)
            print(".", end='', flush=True)

        time.sleep(1)

parser = argparse.ArgumentParser(description='fk log monitor tool')
parser.add_argument('--port', dest="port", type=int, default=9400, help="")
parser.add_argument('--joulescope', dest="joulescope", action="store_true", help="")
args, nargs = parser.parse_known_args()

js = Joulescope(args)

if args.joulescope:
    js.open()

loop = asyncio.get_event_loop()
loop.run_until_complete(rtt_listener(loop, js, args))
loop.close()
