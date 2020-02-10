#!/usr/bin/python3

import logging
import time
import sqlite3
import influxdb
import re

def dict_factory(cursor, row):
    d = {}
    for idx, col in enumerate(cursor.description):
        d[col[0]] = row[idx]
    return d

class Database:
    def __init__(self):
        self.db = sqlite3.connect('power.db')
        self.db.row_factory = dict_factory

    def query_logs(self):
        return self.db.cursor().execute("SELECT * FROM raw ORDER by time").fetchall()

class RawRecord:
    def __init__(self, d):
        self.time = d['time']
        self.energy = d['energy']
        self.message = d['message'].strip()
        try:
            parts = self.message.split(" ")
            self.uptime = int(parts[0])
        except:
            self.uptime = None

class Pattern:
    def __init__(self, expr, decode, hook):
        self.re = re.compile(expr)
        self.decode = decode
        self.hook = hook

class LogParser:
    def __init__(self, listener):
        one_arg = lambda m: (m.group(1),)

        self.patterns = [
            Pattern(r'bl: starting', lambda m: (), listener.started),
            Pattern(r'hash=\'(\w+)\'', one_arg, listener.firmware),
            Pattern(r'serial=\'([\w-]+)\'', one_arg, listener.serial),
            Pattern(r'(\w+):\s+alive', one_arg, listener.task_started),
            Pattern(r'(\w+):\s+dead', one_arg, listener.task_finished),
            Pattern(r'\'(.+)\' \((.+)\) memory', lambda m: (m.group(1), m.group(2)), listener.status),
            Pattern(r'\'(.+)\' mk=(....) version=(\d+)', lambda m: (m.group(1), m.group(2), m.group(3)), listener.module),
        ]

        self.listener = listener

    def handle(self, raw):
        for p in self.patterns:
            m = p.re.search(raw.message)
            if m:
                args = p.decode(m)
                p.hook(raw, *args)

class EnergyAndTime:
    def __init__(self, energy, time):
        self.energy = energy
        self.time = time

class PowerAnalysis:
    def __init__(self, db):
        self.db = db
        self.parser = LogParser(self)
        self.tasks = []
        self.modules = []
        self.active = {}
        self.firmware_hash = None
        self.serial = None
        self.status_energy = None
        self.influx_data = []
        self.session = 0

    def run(self):
        for raw in self.db.query_logs():
            self.parser.handle(RawRecord(raw))

    def started(self, raw):
        self.tasks = []
        self.modules = []
        self.active = {}
        self.firmware_hash = None
        self.serial = None
        self.status_energy = None
        self.session += 1
        logging.info("STARTED")

    def serial(self, raw, serial):
        self.serial = serial
        logging.info("SERIAL: %s" % (serial,))

    def firmware(self, raw, firmware_hash):
        self.firmware_hash = firmware_hash
        logging.info("FIRMWARE: %s" % (firmware_hash,))

    def task_started(self, raw, name):
        if name == 'readings':
            self.modules = []
        self.active[name] = EnergyAndTime(raw.energy, raw.time)
        self.tasks.append(name)
        self.status_energy = None
        logging.info("TASKS.S: %s" % (self.tasks,))

    def task_finished(self, raw, name):
        self.status_energy = None

        if self.firmware_hash is None or self.serial is None:
            return

        start_info = self.active[name]
        if start_info is None:
            logging.warn("TASKS.F: %s" % (self.tasks,))
            return

        delta = raw.energy - start_info.energy
        duration = raw.time - start_info.time
        self.active[name] = None
        self.tasks.remove(name)

        p = {
            "time": time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime(raw.time)),
            "measurement": "task_power",
            "tags": {
                "device_id": self.serial,
                "firmware_hash": self.firmware_hash,
                "modules": ','.join(self.modules),
                "tasks": ','.join(self.tasks),
                "task_name": name,
                "session": self.session,
            },
            "fields": {
                "energy": delta,
                "duration": duration,
            },
        }

        self.influx_data.append(p)

        logging.info("TASKS.F: %s" % (self.tasks,))

    def status(self, raw, name, ip):
        if self.status_energy:
            delta = raw.energy - self.status_energy

            p = {
                "time": time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime(raw.time)),
                "measurement": "idle_power",
                "tags": {
                    "device_id": self.serial,
                    "firmware_hash": self.firmware_hash,
                    "modules": ','.join(self.modules),
                    "tasks": ','.join(self.tasks),
                    "session": self.session,
                },
                "fields": {
                    "energy": delta,
                    "duration": 0.0,
                },
            }

            self.influx_data.append(p)

        self.status_energy = raw.energy

    def module(self, raw, name, mk, version):
        self.modules.append(name)

if __name__ == "__main__":
    logging.basicConfig(format='%(asctime)-15s %(message)s', level=logging.INFO)

    db = Database()

    pa = PowerAnalysis(db)
    pa.run()

    x = influxdb.InfluxDBClient('127.0.0.1', 8086, 'admin', 'supersecretpassword', 'power')
    x.drop_database('power')
    x.create_database('power')
    x.write_points(pa.influx_data)
