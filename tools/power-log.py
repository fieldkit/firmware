#!/usr/bin/python3

import asyncio
import joulescope
import time
import argparse
import re

class FkListener:
    def __init__(self, args):
        pass

    def started(self, firmware):
        self.tasks = []

    def firmware(self, firmware):
        self.firmware = firmware

    def task_started(self, name):
        pass

    def task_finished(self, name):
        pass

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

async def rtt_listener(loop, args):
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

            writer.close()
            reader.close()
        except Exception as e:
            print(e)
            print(".", end='', flush=True)

        time.sleep(1)

parser = argparse.ArgumentParser(description='fk log monitor tool')
parser.add_argument('--port', dest="port", type=int, default=9400, help="")
args, nargs = parser.parse_known_args()

loop = asyncio.get_event_loop()
loop.run_until_complete(rtt_listener(loop, args))
loop.close()

if False:
        js = joulescope.scan_require_one()
        js.open()

        try:
                js.parameter_set('source', 'on')
                js.parameter_set('i_range', 'auto')
                data = js.read(contiguous_duration=0.25)
        finally:
                js.close()
