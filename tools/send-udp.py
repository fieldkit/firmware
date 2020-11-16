#!/usr/bin/python3

import socket
import struct
import sys
import time
import logging


def main():
    logging.basicConfig(stream=sys.stdout, level=logging.INFO)
    log = logging.getLogger("send-udp")

    logged_data = """
00 00 84 00 00 00 00 01 00 00 00 01 09 5f 73 65 72 76 69 63 65 73 07 5f 64 6e 73 2d 73 64 04 5f
75 64 70 05 6c 6f 63 61 6c 00 00 0c 00 01 00 00 11 94 00 0b 03 5f 66 6b 04 5f 74 63 70 c0 23 00
00 29 05 a0 00 00 11 94 00 12 00 04 00 0e 00 00 e2 33 8e 8a 13 25 3a 43 b2 b1 bf 1a
"""

    logging.info("starting")

    packed = bytearray.fromhex(logged_data.replace("\n", " ").replace(" ", ""))

    multicast_group = ("224.1.2.3", 22143)  # custom
    multicast_group = ("224.0.0.251", 5353)  # mDNs
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.settimeout(0.2)
    ttl = struct.pack("b", 1)
    sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, ttl)

    logging.info("ok")

    try:
        while True:
            logging.info("sending")
            sent = sock.sendto(packed, multicast_group)
            time.sleep(1)
    finally:
        logging.info("closing")
        sock.close()


if __name__ == "__main__":
    main()
