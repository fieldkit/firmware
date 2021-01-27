#!/usr/bin/python3

import sys
import datetime
import os
import logging
import re
import csv
import matplotlib.pyplot as plt

log = logging.getLogger("battery")

# 00013623 idle       info    status: 2021/1/16 17:44:31 'Pleasing Seahorse 40' (192.168.2.1) memory(188 / 147464) pages(6 / 6 / 18)
# 00009420 startup    info    battery: battery: v_bus = 3.008000V v_s = 5.570000mV 0.000000mA 0.000000mW 0.000000%
# 00010041 startup    info    readings: [ 1] 'diagnostics.battery_vbus' = 3.008000

status_time = re.compile("^status: (\d\d\d\d.\d+.\d+ \d+:\d+:\d+)")
status_voltage = re.compile("^battery: battery: v_bus = (\S+)V v_s = (\S+)mV")
reading_voltage = re.compile("^readings: \[..\] 'diagnostics.battery_vbus' = (\S+)")
path_time = re.compile("(\d\d\d\d\d\d\d\d_\d\d\d\d\d\d)")


class Samples:
    def __init__(self):
        self.stations = {}

    def station(self, name):
        return self.stations.setdefault(name, {})

    def mark_time(self, station, file_time, uptime, time):
        self.station(station).setdefault(file_time, []).append(
            {"uptime": uptime, "time": time}
        )

    def mark_voltage(self, station, file_time, uptime, voltage):
        self.station(station).setdefault(file_time, []).append(
            {"uptime": uptime, "voltage": voltage}
        )

    def write_csvs(self):
        stations = self.stations.keys()
        for station in stations:
            station_all = []

            with open(f"{station}.csv", "w") as all_file:
                files = self.stations[station]
                keys = list(files.keys())
                keys.sort()
                all_writer = csv.writer(all_file, quoting=csv.QUOTE_MINIMAL)
                for key in keys:
                    file_data = []
                    stamp = key.strftime("%Y%m%d_%H%M%S")
                    with open(stamp + ".csv", "w") as key_file:
                        writer = csv.writer(key_file, quoting=csv.QUOTE_MINIMAL)
                        for row in files[key]:
                            if "voltage" in row:
                                all_writer.writerow([row["uptime"], row["voltage"]])
                                writer.writerow([key, row["uptime"], row["voltage"]])
                                station_all.append(row["voltage"])
                                file_data.append(row["voltage"])
                    plt.plot(file_data)
                    plt.ylabel("battery")
                    plt.savefig(f"{station}_{stamp}.png")
                    plt.clf()

            plt.plot(station_all)
            plt.ylabel("battery")
            plt.savefig(f"{station}.png")


def maybe_float(s: str):
    try:
        return float(s)
    except ValueError:
        return None


def process(samples, path: str):
    log.info(f"{path}")

    file_time = None
    m = path_time.search(path)
    if m:
        file_time = datetime.datetime.strptime(m.group(1), "%Y%m%d_%H%M%S")

    station = "station"
    path_parts = path.split("/")
    if len(path_parts) >= 4:
        name_parts = path_parts[-3].split("-")[0].strip()
        station = name_parts

    with open(path, "r") as file:
        for line in file:
            fields = re.split("\s+", line.strip(), maxsplit=3)
            if len(fields) != 4 or fields[0] == "":
                continue
            uptime = maybe_float(fields[0])
            if uptime:
                try:
                    message = fields[3]
                    m = status_time.search(message)
                    if m:
                        time = datetime.datetime.strptime(
                            m.group(1), "%Y/%m/%d %H:%M:%S"
                        )

                    m = status_voltage.search(message)
                    if m:
                        voltage = float(m.group(1))
                        # samples.mark_voltage(station, file_time, uptime, voltage)

                    m = reading_voltage.search(message)
                    if m:
                        voltage = float(m.group(1))
                        samples.mark_voltage(station, file_time, uptime, voltage)
                except:
                    log.exception("error processing line")


def main():
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s [%(levelname)s] %(message)s",
        handlers=[logging.StreamHandler()],
    )

    samples = Samples()
    for path in sys.argv[1:]:
        for path, dirs, files in os.walk(path):
            log.info(f"{path}")
            for file in files:
                name, file_extension = os.path.splitext(file)
                if file_extension in [".txt"]:
                    process(samples, os.path.join(path, file))

    samples.write_csvs()


if __name__ == "__main__":
    main()
