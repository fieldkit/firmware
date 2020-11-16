#!/usr/bin/python3

import re

source = """
udp-recv[mdns]: 00 00 84 00 00 00 00 02 00 00 00 03 10 5f 73 70 6f 74 69 66 79 2d 63 6f 6e 6e 65 63 74 04 5f 74
udp-recv[mdns]: 63 70 05 6c 6f 63 61 6c 00 00 0c 00 01 00 00 00 3c 00 24 21 39 33 39 37 66 34 33 66 2d 62 36 35
udp-recv[mdns]: 32 2d 35 33 34 36 2d 39 65 33 64 2d 32 35 36 64 36 38 39 2d 30 c0 0c 09 5f 73 65 72 76 69 63 65
udp-recv[mdns]: 73 07 5f 64 6e 73 2d 73 64 04 5f 75 64 70 c0 22 00 0c 00 01 00 00 00 3c 00 02 c0 0c c0 33 00 21
udp-recv[mdns]: 00 01 00 00 00 3c 00 2a 00 00 00 00 92 2e 21 39 33 39 37 66 34 33 66 2d 62 36 35 32 2d 35 33 34
udp-recv[mdns]: 36 2d 39 65 33 64 2d 32 35 36 64 36 38 39 2d 30 c0 22 c0 33 00 10 00 01 00 00 00 3c 00 21 0b 43
udp-recv[mdns]: 50 61 74 68 3d 2f 7a 63 2f 30 0b 56 45 52 53 49 4f 4e 3d 31 2e 30 08 53 74 61 63 6b 3d 53 50 c0
udp-recv[mdns]: 8e 00 01 00 01 00 00 00 3c 00 04 c0 a8 00 c0  (239 bytes)

udp-recv[mdns]: 00 00 00 00 00 03 00 00 00 00 00 01 0f 5f 63 6f 6d 70 61 6e 69 6f 6e 2d 6c 69 6e 6b 04 5f 74 63
udp-recv[mdns]: 70 05 6c 6f 63 61 6c 00 00 0c 00 01 08 5f 68 6f 6d 65 6b 69 74 c0 1c 00 0c 00 01 0c 5f 73 6c 65
udp-recv[mdns]: 65 70 2d 70 72 6f 78 79 04 5f 75 64 70 c0 21 00 0c 00 01 00 00 29 05 a0 00 00 11 94 00 12 00 04
udp-recv[mdns]: 00 0e 00 03 e2 33 8e 88 2d 65 16 14 86 df a4 9d  (112 bytes)

udp-recv[mdns]: 1a 6b 00 00 00 01 00 00 00 00 00 00 03 31 39 32 03 31 36 38 01 30 03 31 34 39 07 69 6e 2d 61 64
udp-recv[mdns]: 64 72 04 61 72 70 61 00 00 0c 00 01  (44 bytes)

udp-recv[mdns]: 22 14 00 00 00 01 00 00 00 00 00 00 03 31 39 32 03 31 36 38 01 30 03 31 35 32 07 69 6e 2d 61 64
udp-recv[mdns]: 64 72 04 61 72 70 61 00 00 0c 00 01  (44 bytes)

udp-recv[mdns]: 22 15 00 00 00 01 00 00 00 00 00 00 03 31 39 32 03 31 36 38 01 30 03 32 30 39 07 69 6e 2d 61 64
udp-recv[mdns]: 64 72 04 61 72 70 61 00 00 0c 00 01  (44 bytes)

udp-recv[mdns]: 22 3a 00 00 00 01 00 00 00 00 00 00 03 31 39 32 03 31 36 38 01 30 03 31 38 37 07 69 6e 2d 61 64
udp-recv[mdns]: 64 72 04 61 72 70 61 00 00 0c 00 01  (44 bytes)

udp-recv[mdns]: 00 00 00 00 00 03 00 00 00 03 00 00 07 41 6e 64 72 6f 69 64 05 6c 6f 63 61 6c 00 00 ff 80 01 c0
udp-recv[mdns]: 0c 00 ff 80 01 0b 6a 61 63 6f 62 2d 70 68 6f 6e 65 05 5f 68 74 74 70 04 5f 74 63 70 c0 14 00 ff
udp-recv[mdns]: 80 01 c0 0c 00 01 00 01 00 00 00 78 00 04 c0 a8 00 95 c0 0c 00 1c 00 01 00 00 00 78 00 10 fe 80
udp-recv[mdns]: 00 00 00 00 00 00 42 4e 36 ff fe 1e ec c1 c0 25 00 21 00 01 00 00 00 78 00 08 00 00 00 00 00 50
udp-recv[mdns]: c0 0c  (130 bytes)

udp-recv[mdns]: 00 00 84 00 00 00 00 08 00 00 00 04 03 31 34 39 01 30 03 31 36 38 03 31 39 32 07 69 6e 2d 61 64
udp-recv[mdns]: 64 72 04 61 72 70 61 00 00 0c 80 01 00 00 00 78 00 0f 07 41 6e 64 72 6f 69 64 05 6c 6f 63 61 6c
udp-recv[mdns]: 00 01 31 01 43 01 43 01 45 01 45 01 31 01 45 01 46 01 46 01 46 01 36 01 33 01 45 01 34 01 32 01
udp-recv[mdns]: 34 01 30 01 30 01 30 01 30 01 30 01 30 01 30 01 30 01 30 01 30 01 30 01 30 01 30 01 38 01 45 01
udp-recv[mdns]: 46 03 69 70 36 c0 22 00 0c 80 01 00 00 00 78 00 02 c0 32 0b 6a 61 63 6f 62 2d 70 68 6f 6e 65 05
udp-recv[mdns]: 5f 68 74 74 70 04 5f 74 63 70 c0 3a 00 10 80 01 00 00 11 94 00 01 00 09 5f 73 65 72 76 69 63 65
udp-recv[mdns]: 73 07 5f 64 6e 73 2d 73 64 04 5f 75 64 70 c0 3a 00 0c 00 01 00 00 11 94 00 02 c0 9f c0 9f 00 0c
udp-recv[mdns]: 00 01 00 00 11 94 00 02 c0 93 c0 32 00 01 80 01 00 00 00 78 00 04 c0 a8 00 95 c0 32 00 1c 80 01
udp-recv[mdns]: 00 00 00 78 00 10 fe 80 00 00 00 00 00 00 42 4e 36 ff fe 1e ec c1 c0 93 00 21 80 01 00 00 00 78
udp-recv[mdns]: 00 08 00 00 00 00 00 50 c0 32 c0 0c 00 2f 80 01 00 00 00 78 00 06 c0 0c 00 02 00 08 c0 41 00 2f
udp-recv[mdns]: 80 01 00 00 00 78 00 06 c0 41 00 02 00 08 c0 93 00 2f 80 01 00 00 11 94 00 09 c0 93 00 05 00 00
udp-recv[mdns]: 80 00 40 c0 32 00 2f 80 01 00 00 00 78 00 08 c0 32 00 04 40 00 00 08  (375 bytes)

udp-send[mdns]: 00 00 84 00 00 00 00 04 00 00 00 01 20 34 66 62 31 61 31 35 35 35 33 33 36 35 37 33 32 33 32 32
udp-send[mdns]: 30 32 30 32 30 66 66 31 39 33 38 31 31 03 5f 66 6b 04 5f 74 63 70 05 6c 6f 63 61 6c 00 00 21 80
udp-send[mdns]: 01 00 00 00 78 00 2e 00 00 00 00 00 50 20 34 66 62 31 61 31 35 35 35 33 33 36 35 37 33 32 33 32
udp-send[mdns]: 32 30 32 30 32 30 66 66 31 39 33 38 31 31 05 6c 6f 63 61 6c 00 20 34 66 62 31 61 31 35 35 35 33
udp-send[mdns]: 33 36 35 37 33 32 33 32 32 30 32 30 32 30 66 66 31 39 33 38 31 31 03 5f 66 6b 04 5f 74 63 70 05
udp-send[mdns]: 6c 6f 63 61 6c 00 00 10 80 01 00 00 00 78 00 01 00 09 5f 73 65 72 76 69 63 65 73 07 5f 64 6e 73
udp-send[mdns]: 2d 73 64 04 5f 75 64 70 05 6c 6f 63 61 6c 00 00 0c 00 01 00 00 00 78 00 10 03 5f 66 6b 04 5f 74
udp-send[mdns]: 63 70 05 6c 6f 63 61 6c 00 03 5f 66 6b 04 5f 74 63 70 05 6c 6f 63 61 6c 00 00 0c 00 01 00 00 00
udp-send[mdns]: 78 00 31 20 34 66 62 31 61 31 35 35 35 33 33 36 35 37 33 32 33 32 32 30 32 30 32 30 66 66 31 39
udp-send[mdns]: 33 38 31 31 03 5f 66 6b 04 5f 74 63 70 05 6c 6f 63 61 6c 00 20 34 66 62 31 61 31 35 35 35 33 33
udp-send[mdns]: 36 35 37 33 32 33 32 32 30 32 30 32 30 66 66 31 39 33 38 31 31 05 6c 6f 63 61 6c 00 00 01 80 01
udp-send[mdns]: 00 00 00 78 00 04 c0 a8 00 d1  (362 bytes)

udp-recv[mdns]: 00 00 84 00 00 00 00 02 00 00 00 04 09 5f 73 65 72 76 69 63 65 73 07 5f 64 6e 73 2d 73 64 04 5f
udp-recv[mdns]: 75 64 70 05 6c 6f 63 61 6c 00 00 0c 00 01 00 00 00 3c 00 18 10 5f 73 70 6f 74 69 66 79 2d 63 6f
udp-recv[mdns]: 6e 6e 65 63 74 04 5f 74 63 70 c0 23 c0 34 00 0c 00 01 00 00 00 3c 00 24 21 39 33 39 37 66 34 33
udp-recv[mdns]: 66 2d 62 36 35 32 2d 35 33 34 36 2d 39 65 33 64 2d 32 35 36 64 36 38 39 2d 30 c0 34 c0 34 00 0c
udp-recv[mdns]: 00 01 00 00 00 3c 00 02 c0 58 c0 58 00 21 00 01 00 00 00 3c 00 2a 00 00 00 00 92 2e 21 39 33 39
udp-recv[mdns]: 37 66 34 33 66 2d 62 36 35 32 2d 35 33 34 36 2d 39 65 33 64 2d 32 35 36 64 36 38 39 2d 30 c0 23
udp-recv[mdns]: c0 58 00 10 00 01 00 00 00 3c 00 21 0b 43 50 61 74 68 3d 2f 7a 63 2f 30 0b 56 45 52 53 49 4f 4e
udp-recv[mdns]: 3d 31 2e 30 08 53 74 61 63 6b 3d 53 50 c0 9c 00 01 00 01 00 00 00 3c 00 04 c0 a8 00 c0  (253 bytes)

udp-send[mdns]: 00 00 84 00 00 00 00 04 00 00 00 01 20 34 66 62 31 61 31 35 35 35 33 33 36 35 37 33 32 33 32 32
udp-send[mdns]: 30 32 30 32 30 66 66 31 39 33 38 31 31 03 5f 66 6b 04 5f 74 63 70 05 6c 6f 63 61 6c 00 00 21 80
udp-send[mdns]: 01 00 00 00 78 00 2e 00 00 00 00 00 50 20 34 66 62 31 61 31 35 35 35 33 33 36 35 37 33 32 33 32
udp-send[mdns]: 32 30 32 30 32 30 66 66 31 39 33 38 31 31 05 6c 6f 63 61 6c 00 20 34 66 62 31 61 31 35 35 35 33
udp-send[mdns]: 33 36 35 37 33 32 33 32 32 30 32 30 32 30 66 66 31 39 33 38 31 31 03 5f 66 6b 04 5f 74 63 70 05
udp-send[mdns]: 6c 6f 63 61 6c 00 00 10 80 01 00 00 00 78 00 01 00 09 5f 73 65 72 76 69 63 65 73 07 5f 64 6e 73
udp-send[mdns]: 2d 73 64 04 5f 75 64 70 05 6c 6f 63 61 6c 00 00 0c 00 01 00 00 00 78 00 10 03 5f 66 6b 04 5f 74
udp-send[mdns]: 63 70 05 6c 6f 63 61 6c 00 03 5f 66 6b 04 5f 74 63 70 05 6c 6f 63 61 6c 00 00 0c 00 01 00 00 00
udp-send[mdns]: 78 00 31 20 34 66 62 31 61 31 35 35 35 33 33 36 35 37 33 32 33 32 32 30 32 30 32 30 66 66 31 39
udp-send[mdns]: 33 38 31 31 03 5f 66 6b 04 5f 74 63 70 05 6c 6f 63 61 6c 00 20 34 66 62 31 61 31 35 35 35 33 33
udp-send[mdns]: 36 35 37 33 32 33 32 32 30 32 30 32 30 66 66 31 39 33 38 31 31 05 6c 6f 63 61 6c 00 00 01 80 01
udp-send[mdns]: 00 00 00 78 00 04 c0 a8 00 d1  (362 bytes)


udp-recv[mdns]: 00 00 00 00 00 03 00 00 00 03 00 00 0b 6a 61 63 6f 62 2d 70 68 6f 6e 65 05 5f 68 74 74 70 04 5f
udp-recv[mdns]: 74 63 70 05 6c 6f 63 61 6c 00 00 ff 00 01 07 41 6e 64 72 6f 69 64 c0 23 00 ff 00 01 c0 2e 00 ff
udp-recv[mdns]: 00 01 c0 0c 00 21 00 01 00 00 00 78 00 08 00 00 00 00 00 50 c0 2e c0 2e 00 01 00 01 00 00 00 78
udp-recv[mdns]: 00 04 c0 a8 00 95 c0 2e 00 1c 00 01 00 00 00 78 00 10 fe 80 00 00 00 00 00 00 42 4e 36 ff fe 1e
udp-recv[mdns]: ec c1  (130 bytes)

udp-send[mdns]: 00 00 84 00 00 00 00 04 00 00 00 01 20 34 66 62 31 61 31 35 35 35 33 33 36 35 37 33 32 33 32 32
udp-send[mdns]: 30 32 30 32 30 66 66 31 39 33 38 31 31 03 5f 66 6b 04 5f 74 63 70 05 6c 6f 63 61 6c 00 00 21 80
udp-send[mdns]: 01 00 00 00 78 00 2e 00 00 00 00 00 50 20 34 66 62 31 61 31 35 35 35 33 33 36 35 37 33 32 33 32
udp-send[mdns]: 32 30 32 30 32 30 66 66 31 39 33 38 31 31 05 6c 6f 63 61 6c 00 20 34 66 62 31 61 31 35 35 35 33
udp-send[mdns]: 33 36 35 37 33 32 33 32 32 30 32 30 32 30 66 66 31 39 33 38 31 31 03 5f 66 6b 04 5f 74 63 70 05
udp-send[mdns]: 6c 6f 63 61 6c 00 00 10 80 01 00 00 00 78 00 01 00 09 5f 73 65 72 76 69 63 65 73 07 5f 64 6e 73
udp-send[mdns]: 2d 73 64 04 5f 75 64 70 05 6c 6f 63 61 6c 00 00 0c 00 01 00 00 00 78 00 10 03 5f 66 6b 04 5f 74
udp-send[mdns]: 63 70 05 6c 6f 63 61 6c 00 03 5f 66 6b 04 5f 74 63 70 05 6c 6f 63 61 6c 00 00 0c 00 01 00 00 00
udp-send[mdns]: 78 00 31 20 34 66 62 31 61 31 35 35 35 33 33 36 35 37 33 32 33 32 32 30 32 30 32 30 66 66 31 39
udp-send[mdns]: 33 38 31 31 03 5f 66 6b 04 5f 74 63 70 05 6c 6f 63 61 6c 00 20 34 66 62 31 61 31 35 35 35 33 33
udp-send[mdns]: 36 35 37 33 32 33 32 32 30 32 30 32 30 66 66 31 39 33 38 31 31 05 6c 6f 63 61 6c 00 00 01 80 01
udp-send[mdns]: 00 00 00 78 00 04 c0 a8 00 d1  (362 bytes)

udp-recv[mdns]: 00 00 84 00 00 00 00 08 00 00 00 04 0b 6a 61 63 6f 62 2d 70 68 6f 6e 65 05 5f 68 74 74 70 04 5f
udp-recv[mdns]: 74 63 70 05 6c 6f 63 61 6c 00 00 10 80 01 00 00 11 94 00 01 00 09 5f 73 65 72 76 69 63 65 73 07
udp-recv[mdns]: 5f 64 6e 73 2d 73 64 04 5f 75 64 70 c0 23 00 0c 00 01 00 00 11 94 00 02 c0 18 c0 18 00 0c 00 01
udp-recv[mdns]: 00 00 11 94 00 02 c0 0c c0 0c 00 21 80 01 00 00 00 78 00 10 00 00 00 00 00 50 07 41 6e 64 72 6f
udp-recv[mdns]: 69 64 c0 23 03 31 34 39 01 30 03 31 36 38 03 31 39 32 07 69 6e 2d 61 64 64 72 04 61 72 70 61 00
udp-recv[mdns]: 00 0c 80 01 00 00 00 78 00 02 c0 7a 01 31 01 43 01 43 01 45 01 45 01 31 01 45 01 46 01 46 01 46
udp-recv[mdns]: 01 36 01 33 01 45 01 34 01 32 01 34 01 30 01 30 01 30 01 30 01 30 01 30 01 30 01 30 01 30 01 30
udp-recv[mdns]: 01 30 01 30 01 30 01 38 01 45 01 46 03 69 70 36 c0 9a 00 0c 80 01 00 00 00 78 00 02 c0 7a c0 7a
udp-recv[mdns]: 00 01 80 01 00 00 00 78 00 04 c0 a8 00 95 c0 7a 00 1c 80 01 00 00 00 78 00 10 fe 80 00 00 00 00
udp-recv[mdns]: 00 00 42 4e 36 ff fe 1e ec c1 c0 0c 00 2f 80 01 00 00 11 94 00 09 c0 0c 00 05 00 00 80 00 40 c0
udp-recv[mdns]: 84 00 2f 80 01 00 00 00 78 00 06 c0 84 00 02 00 08 c0 ac 00 2f 80 01 00 00 00 78 00 06 c0 ac 00
udp-recv[mdns]: 02 00 08 c0 7a 00 2f 80 01 00 00 00 78 00 08 c0 7a 00 04 40 00 00 08  (375 bytes)

udp-recv[mdns]: 00 00 00 00 00 01 00 01 00 00 00 00 03 5f 66 6b 04 5f 74 63 70 05 6c 6f 63 61 6c 00 00 0c 00 01
udp-recv[mdns]: c0 0c 00 0c 00 01 00 00 00 78 00 23 20 34 66 62 31 61 31 35 35 35 33 33 36 35 37 33 32 33 32 32
udp-recv[mdns]: 30 32 30 32 30 66 66 31 39 33 38 31 31 c0 0c  (79 bytes)

udp-recv[mdns]: 00 00 00 00 00 01 00 01 00 00 00 00 03 5f 66 6b 04 5f 74 63 70 05 6c 6f 63 61 6c 00 00 0c 00 01
udp-recv[mdns]: c0 0c 00 0c 00 01 00 00 00 78 00 23 20 34 66 62 31 61 31 35 35 35 33 33 36 35 37 33 32 33 32 32
udp-recv[mdns]: 30 32 30 32 30 66 66 31 39 33 38 31 31 c0 0c  (79 bytes)

udp-recv[mdns]: 00 00 00 00 00 01 00 00 00 00 00 00 03 5f 66 6b 05 5f 68 74 74 70 04 5f 74 63 70 05 6c 6f 63 61
udp-recv[mdns]: 6c 00 00 0c 00 01  (38 bytes)

udp-recv[mdns]: 00 00 00 00 00 17 00 04 00 00 00 00 08 5f 61 69 72 70 6f 72 74 04 5f 74 63 70 05 6c 6f 63 61 6c
udp-recv[mdns]: 00 00 0c 00 01 08 5f 61 69 72 70 6c 61 79 c0 15 00 0c 00 01 05 5f 72 61 6f 70 c0 15 00 0c 00 01
udp-recv[mdns]: 06 5f 75 73 63 61 6e c0 15 00 0c 00 01 07 5f 75 73 63 61 6e 73 c0 15 00 0c 00 01 07 5f 69 70 70
udp-recv[mdns]: 75 73 62 c0 15 00 0c 00 01 08 5f 73 63 61 6e 6e 65 72 c0 15 00 0c 00 01 04 5f 69 70 70 c0 15 00
udp-recv[mdns]: 0c 00 01 05 5f 69 70 70 73 c0 15 00 0c 00 01 08 5f 70 72 69 6e 74 65 72 c0 15 00 0c 00 01 0f 5f
udp-recv[mdns]: 70 64 6c 2d 64 61 74 61 73 74 72 65 61 6d c0 15 00 0c 00 01 04 5f 70 74 70 c0 15 00 0c 00 01 07
udp-recv[mdns]: 5f 72 64 6c 69 6e 6b c0 15 00 0c 00 01 08 5f 68 6f 6d 65 6b 69 74 c0 15 00 0c 00 01 0b 5f 61 66
udp-recv[mdns]: 70 6f 76 65 72 74 63 70 c0 15 00 0c 00 01 04 5f 73 6d 62 c0 15 00 0c 00 01 04 5f 72 66 62 c0 15
udp-recv[mdns]: 00 0c 00 01 06 5f 61 64 69 73 6b c0 15 00 0c 00 01 0f 5f 63 6f 6d 70 61 6e 69 6f 6e 2d 6c 69 6e
udp-recv[mdns]: 6b c0 15 00 0c 00 01 0c 5f 73 6c 65 65 70 2d 70 72 6f 78 79 04 5f 75 64 70 c0 1a 00 0c 00 01 0d
udp-recv[mdns]: 5f 61 70 70 6c 65 2d 6d 6f 62 64 65 76 c0 15 00 0c 00 01 08 39 32 39 33 30 37 64 61 04 5f 73 75
udp-recv[mdns]: 62 0e 5f 61 70 70 6c 65 2d 6d 6f 62 64 65 76 32 c0 15 00 0c 00 01 0f 5f 61 70 70 6c 65 2d 70 61
udp-recv[mdns]: 69 72 61 62 6c 65 c0 15 00 0c 00 01 c0 ee 00 0c 00 01 00 00 10 28 00 0d 0a 4a 41 43 4f 42 2d 48
udp-recv[mdns]: 4f 4d 45 c0 ee c0 ee 00 0c 00 01 00 00 10 2f 00 19 16 43 6f 6e 73 65 72 76 69 66 79 20 4d 61 63
udp-recv[mdns]: 42 6f 6f 6b 20 50 72 6f c0 ee c0 f9 00 0c 00 01 00 00 10 2f 00 19 16 43 6f 6e 73 65 72 76 69 66
udp-recv[mdns]: 79 20 4d 61 63 42 6f 6f 6b 20 50 72 6f c0 f9 c1 11 00 0c 00 01 00 00 10 2f 00 19 16 43 6f 6e 73
udp-recv[mdns]: 65 72 76 69 66 79 20 4d 61 63 42 6f 6f 6b 20 50 72 6f c1 11  (532 bytes)

udp-recv[mdns]: 2a d4 00 00 00 01 00 00 00 00 00 00 03 31 39 32 03 31 36 38 01 30 03 32 30 39 07 69 6e 2d 61 64
udp-recv[mdns]: 64 72 04 61 72 70 61 00 00 0c 00 01  (44 bytes)

udp-recv[mdns]: 00 00 00 00 00 04 00 01 00 00 00 01 0f 5f 63 6f 6d 70 61 6e 69 6f 6e 2d 6c 69 6e 6b 04 5f 74 63
udp-recv[mdns]: 70 05 6c 6f 63 61 6c 00 00 0c 00 01 08 5f 68 6f 6d 65 6b 69 74 c0 1c 00 0c 00 01 0c 5f 73 6c 65
udp-recv[mdns]: 65 70 2d 70 72 6f 78 79 04 5f 75 64 70 c0 21 00 0c 00 01 03 5f 66 6b c0 1c 00 0c 00 01 c0 53 00
udp-recv[mdns]: 0c 00 01 00 00 00 60 00 23 20 34 66 62 31 61 31 35 35 35 33 33 36 35 37 33 32 33 32 32 30 32 30
udp-recv[mdns]: 32 30 66 66 31 39 33 38 31 31 c0 53 00 00 29 05 a0 00 00 11 94 00 12 00 04 00 0e 00 8d 22 db 70
udp-recv[mdns]: 89 76 6f ce d4 ea a7 45 06  (169 bytes)

udp-recv[mdns]: 00 00 84 00 00 00 00 06 00 00 00 01 09 5f 6b 65 72 62 65 72 6f 73 16 43 6f 6e 73 65 72 76 69 66
udp-recv[mdns]: 79 2d 4d 61 63 42 6f 6f 6b 2d 50 72 6f 05 6c 6f 63 61 6c 00 00 10 00 01 00 00 11 94 00 33 32 4c
udp-recv[mdns]: 4b 44 43 3a 53 48 41 31 2e 42 46 38 35 34 30 37 43 39 46 33 44 37 43 45 42 41 33 38 41 34 42 30
udp-recv[mdns]: 43 45 38 39 36 41 36 30 30 41 36 45 39 45 31 35 31 09 5f 73 65 72 76 69 63 65 73 07 5f 64 6e 73
udp-recv[mdns]: 2d 73 64 04 5f 75 64 70 c0 2d 00 0c 00 01 00 00 11 94 00 0c 04 5f 73 73 68 04 5f 74 63 70 c0 2d
udp-recv[mdns]: 16 43 6f 6e 73 65 72 76 69 66 79 20 4d 61 63 42 6f 6f 6b 20 50 72 6f 0c 5f 64 65 76 69 63 65 2d
udp-recv[mdns]: 69 6e 66 6f c0 99 00 10 00 01 00 00 11 94 00 33 14 6d 6f 64 65 6c 3d 4d 61 63 42 6f 6f 6b 50 72
udp-recv[mdns]: 6f 31 33 2c 32 0a 6f 73 78 76 65 72 73 3d 31 39 12 65 63 6f 6c 6f 72 3d 31 35 37 2c 31 35 37 2c
udp-recv[mdns]: 31 36 30 c0 71 00 0c 00 01 00 00 11 94 00 0c 09 5f 73 66 74 70 2d 73 73 68 c0 99 c0 71 00 0c 00
udp-recv[mdns]: 01 00 00 11 94 00 07 04 5f 72 66 62 c0 99 c0 71 00 0c 00 01 00 00 11 94 00 07 04 5f 73 6d 62 c0
udp-recv[mdns]: 99 00 00 29 05 a0 00 00 11 94 00 12 00 04 00 0e 00 24 ac de 48 00 11 22 78 4f 43 4f f4 3a  (350 bytes)

udp-recv[mdns]: 00 00 00 00 00 01 00 00 00 02 00 01 16 43 6f 6e 73 65 72 76 69 66 79 2d 4d 61 63 42 6f 6f 6b 2d
udp-recv[mdns]: 50 72 6f 05 6c 6f 63 61 6c 00 00 ff 00 01 c0 0c 00 1c 00 01 00 00 00 78 00 10 fe 80 00 00 00 00
udp-recv[mdns]: 00 00 1c 0a 54 27 87 d8 c7 93 c0 0c 00 01 00 01 00 00 00 78 00 04 c0 a8 00 c2 00 00 29 05 a0 00
udp-recv[mdns]: 00 11 94 00 12 00 04 00 0e 00 24 ac de 48 00 11 22 78 4f 43 4f f4 3a  (119 bytes)

udp-recv[mdns]: 00 00 84 00 00 00 00 01 00 00 00 00 0a 4a 41 43 4f 42 2d 48 4f 4d 45 0c 5f 64 65 76 69 63 65 2d
udp-recv[mdns]: 69 6e 66 6f 04 5f 74 63 70 05 6c 6f 63 61 6c 00 00 10 80 01 00 00 11 94 00 0f 0e 6d 6f 64 65 6c
udp-recv[mdns]: 3d 4d 61 63 53 61 6d 62 61  (73 bytes)

udp-recv[mdns]: 00 00 00 00 00 01 00 00 00 02 00 01 16 43 6f 6e 73 65 72 76 69 66 79 2d 4d 61 63 42 6f 6f 6b 2d
udp-recv[mdns]: 50 72 6f 05 6c 6f 63 61 6c 00 00 ff 00 01 c0 0c 00 1c 00 01 00 00 00 78 00 10 fe 80 00 00 00 00
udp-recv[mdns]: 00 00 1c 0a 54 27 87 d8 c7 93 c0 0c 00 01 00 01 00 00 00 78 00 04 c0 a8 00 c2 00 00 29 05 a0 00
udp-recv[mdns]: 00 11 94 00 12 00 04 00 0e 00 24 ac de 48 00 11 22 78 4f 43 4f f4 3a  (119 bytes)

udp-recv[mdns]: 00 00 00 00 00 05 00 00 00 05 00 01 16 43 6f 6e 73 65 72 76 69 66 79 20 4d 61 63 42 6f 6f 6b 20
udp-recv[mdns]: 50 72 6f 04 5f 73 73 68 04 5f 74 63 70 05 6c 6f 63 61 6c 00 00 ff 00 01 16 43 6f 6e 73 65 72 76
udp-recv[mdns]: 69 66 79 20 4d 61 63 42 6f 6f 6b 20 50 72 6f 09 5f 73 66 74 70 2d 73 73 68 c0 28 00 ff 00 01 16
udp-recv[mdns]: 43 6f 6e 73 65 72 76 69 66 79 20 4d 61 63 42 6f 6f 6b 20 50 72 6f 04 5f 72 66 62 c0 28 00 ff 00
udp-recv[mdns]: 01 16 43 6f 6e 73 65 72 76 69 66 79 20 4d 61 63 42 6f 6f 6b 20 50 72 6f 04 5f 73 6d 62 c0 28 00
udp-recv[mdns]: ff 00 01 16 43 6f 6e 73 65 72 76 69 66 79 20 4d 61 63 42 6f 6f 6b 20 50 72 6f 0f 5f 63 6f 6d 70
udp-recv[mdns]: 61 6e 69 6f 6e 2d 6c 69 6e 6b c0 28 00 ff 00 01 c0 0c 00 21 00 01 00 00 00 78 00 1f 00 00 00 00
udp-recv[mdns]: 00 16 16 43 6f 6e 73 65 72 76 69 66 79 2d 4d 61 63 42 6f 6f 6b 2d 50 72 6f c0 2d c0 38 00 21 00
udp-recv[mdns]: 01 00 00 00 78 00 08 00 00 00 00 00 16 c0 e2 c0 5f 00 21 00 01 00 00 00 78 00 08 00 00 00 00 17
udp-recv[mdns]: 0c c0 e2 c0 81 00 21 00 01 00 00 00 78 00 08 00 00 00 00 01 bd c0 e2 c0 a3 00 21 00 01 00 00 00
udp-recv[mdns]: 78 00 08 00 00 00 00 d3 a0 c0 e2 00 00 29 05 a0 00 00 11 94 00 12 00 04 00 0e 00 24 ac de 48 00
udp-recv[mdns]: 11 22 78 4f 43 4f f4 3a  (360 bytes)

udp-recv[mdns]: 00 00 84 00 00 00 00 03 00 00 00 02 09 5f 73 65 72 76 69 63 65 73 07 5f 64 6e 73 2d 73 64 04 5f
udp-recv[mdns]: 75 64 70 05 6c 6f 63 61 6c 00 00 0c 00 01 00 00 11 94 00 17 0f 5f 63 6f 6d 70 61 6e 69 6f 6e 2d
udp-recv[mdns]: 6c 69 6e 6b 04 5f 74 63 70 c0 23 16 43 6f 6e 73 65 72 76 69 66 79 2d 4d 61 63 42 6f 6f 6b 2d 50
udp-recv[mdns]: 72 6f c0 23 00 1c 80 01 00 00 00 78 00 10 fe 80 00 00 00 00 00 00 1c 0a 54 27 87 d8 c7 93 c0 4b
udp-recv[mdns]: 00 01 80 01 00 00 00 78 00 04 c0 a8 00 c2 c0 4b 00 2f 80 01 00 00 00 78 00 08 c0 4b 00 04 40 00
udp-recv[mdns]: 00 08 00 00 29 05 a0 00 00 11 94 00 12 00 04 00 0e 00 24 ac de 48 00 11 22 78 4f 43 4f f4 3a  (191 bytes)

udp-recv[mdns]: 00 00 00 00 00 17 00 04 00 00 00 01 08 5f 61 69 72 70 6f 72 74 04 5f 74 63 70 05 6c 6f 63 61 6c
udp-recv[mdns]: 00 00 0c 00 01 08 5f 61 69 72 70 6c 61 79 c0 15 00 0c 00 01 05 5f 72 61 6f 70 c0 15 00 0c 00 01
udp-recv[mdns]: 06 5f 75 73 63 61 6e c0 15 00 0c 00 01 07 5f 75 73 63 61 6e 73 c0 15 00 0c 00 01 07 5f 69 70 70
udp-recv[mdns]: 75 73 62 c0 15 00 0c 00 01 08 5f 73 63 61 6e 6e 65 72 c0 15 00 0c 00 01 04 5f 69 70 70 c0 15 00
udp-recv[mdns]: 0c 00 01 05 5f 69 70 70 73 c0 15 00 0c 00 01 08 5f 70 72 69 6e 74 65 72 c0 15 00 0c 00 01 0f 5f
udp-recv[mdns]: 70 64 6c 2d 64 61 74 61 73 74 72 65 61 6d c0 15 00 0c 00 01 04 5f 70 74 70 c0 15 00 0c 00 01 07
udp-recv[mdns]: 5f 72 64 6c 69 6e 6b c0 15 00 0c 00 01 08 5f 68 6f 6d 65 6b 69 74 c0 15 00 0c 00 01 0b 5f 61 66
udp-recv[mdns]: 70 6f 76 65 72 74 63 70 c0 15 00 0c 00 01 04 5f 73 6d 62 c0 15 00 0c 00 01 04 5f 72 66 62 c0 15
udp-recv[mdns]: 00 0c 00 01 06 5f 61 64 69 73 6b c0 15 00 0c 00 01 0f 5f 63 6f 6d 70 61 6e 69 6f 6e 2d 6c 69 6e
udp-recv[mdns]: 6b c0 15 00 0c 00 01 0c 5f 73 6c 65 65 70 2d 70 72 6f 78 79 04 5f 75 64 70 c0 1a 00 0c 00 01 0d
udp-recv[mdns]: 5f 61 70 70 6c 65 2d 6d 6f 62 64 65 76 c0 15 00 0c 00 01 08 39 32 39 33 30 37 64 61 04 5f 73 75
udp-recv[mdns]: 62 0e 5f 61 70 70 6c 65 2d 6d 6f 62 64 65 76 32 c0 15 00 0c 00 01 0f 5f 61 70 70 6c 65 2d 70 61
udp-recv[mdns]: 69 72 61 62 6c 65 c0 15 00 0c 00 01 c0 ee 00 0c 00 01 00 00 11 91 00 0d 0a 4a 41 43 4f 42 2d 48
udp-recv[mdns]: 4f 4d 45 c0 ee c0 ee 00 0c 00 01 00 00 11 93 00 19 16 43 6f 6e 73 65 72 76 69 66 79 20 4d 61 63
udp-recv[mdns]: 42 6f 6f 6b 20 50 72 6f c0 ee c0 f9 00 0c 00 01 00 00 11 93 00 19 16 43 6f 6e 73 65 72 76 69 66
udp-recv[mdns]: 79 20 4d 61 63 42 6f 6f 6b 20 50 72 6f c0 f9 c1 11 00 0c 00 01 00 00 11 93 00 19 16 43 6f 6e 73
udp-recv[mdns]: 65 72 76 69 66 79 20 4d 61 63 42 6f 6f 6b 20 50 72 6f c1 11 00 00 29 05 a0 00 00 11 94 00 12 00
udp-recv[mdns]: 04 00 0e 00 24 ac de 48 00 11 22 78 4f 43 4f f4 3a  (561 bytes)

udp-recv[mdns]: 00 00 84 00 00 00 00 13 00 00 00 07 16 43 6f 6e 73 65 72 76 69 66 79 20 4d 61 63 42 6f 6f 6b 20
udp-recv[mdns]: 50 72 6f 04 5f 73 73 68 04 5f 74 63 70 05 6c 6f 63 61 6c 00 00 10 80 01 00 00 11 94 00 01 00 c0
udp-recv[mdns]: 23 00 0c 00 01 00 00 11 94 00 02 c0 0c 16 43 6f 6e 73 65 72 76 69 66 79 20 4d 61 63 42 6f 6f 6b
udp-recv[mdns]: 20 50 72 6f 0c 5f 64 65 76 69 63 65 2d 69 6e 66 6f c0 28 00 10 00 01 00 00 11 94 00 33 14 6d 6f
udp-recv[mdns]: 64 65 6c 3d 4d 61 63 42 6f 6f 6b 50 72 6f 31 33 2c 32 0a 6f 73 78 76 65 72 73 3d 31 39 12 65 63
udp-recv[mdns]: 6f 6c 6f 72 3d 31 35 37 2c 31 35 37 2c 31 36 30 16 43 6f 6e 73 65 72 76 69 66 79 20 4d 61 63 42
udp-recv[mdns]: 6f 6f 6b 20 50 72 6f 09 5f 73 66 74 70 2d 73 73 68 c0 28 00 10 80 01 00 00 11 94 00 01 00 c0 c7
udp-recv[mdns]: 00 0c 00 01 00 00 11 94 00 02 c0 b0 16 43 6f 6e 73 65 72 76 69 66 79 20 4d 61 63 42 6f 6f 6b 20
udp-recv[mdns]: 50 72 6f 04 5f 72 66 62 c0 28 00 10 80 01 00 00 11 94 00 01 00 c1 03 00 0c 00 01 00 00 11 94 00
udp-recv[mdns]: 02 c0 ec 16 43 6f 6e 73 65 72 76 69 66 79 20 4d 61 63 42 6f 6f 6b 20 50 72 6f 04 5f 73 6d 62 c0
udp-recv[mdns]: 28 00 10 80 01 00 00 11 94 00 01 00 c1 3a 00 0c 00 01 00 00 11 94 00 02 c1 23 c0 0c 00 21 80 01
udp-recv[mdns]: 00 00 00 78 00 1f 00 00 00 00 00 16 16 43 6f 6e 73 65 72 76 69 66 79 2d 4d 61 63 42 6f 6f 6b 2d
udp-recv[mdns]: 50 72 6f c0 2d c0 b0 00 21 80 01 00 00 00 78 00 08 00 00 00 00 00 16 c1 6c c0 ec 00 21 80 01 00
udp-recv[mdns]: 00 00 78 00 08 00 00 00 00 17 0c c1 6c c1 23 00 21 80 01 00 00 00 78 00 08 00 00 00 00 01 bd c1
udp-recv[mdns]: 6c 16 43 6f 6e 73 65 72 76 69 66 79 20 4d 61 63 42 6f 6f 6b 20 50 72 6f 0f 5f 63 6f 6d 70 61 6e
udp-recv[mdns]: 69 6f 6e 2d 6c 69 6e 6b c0 28 00 10 80 01 00 00 11 94 00 53 16 72 70 42 41 3d 32 30 3a 30 31 3a
udp-recv[mdns]: 43 35 3a 30 34 3a 46 45 3a 41 39 11 72 70 41 44 3d 66 37 38 38 34 61 31 62 61 63 65 30 0c 72 70
udp-recv[mdns]: 46 6c 3d 30 78 32 30 30 30 30 11 72 70 48 4e 3d 65 39 61 39 63 30 63 64 65 33 61 34 0a 72 70 56
udp-recv[mdns]: 72 3d 31 39 35 2e 32 09 5f 73 65 72 76 69 63 65 73 07 5f 64 6e 73 2d 73 64 04 5f 75 64 70 c0 2d
udp-recv[mdns]: 00 0c 00 01 00 00 11 94 00 02 c1 d8 c1 d8 00 0c 00 01 00 00 11 94 00 02 c1 c1 c1 6c 00 1c 80 01
udp-recv[mdns]: 00 00 00 78 00 10 fe 80 00 00 00 00 00 00 1c 0a 54 27 87 d8 c7 93 c1 6c 00 01 80 01 00 00 00 78
udp-recv[mdns]: 00 04 c0 a8 00 c2 c1 c1 00 21 80 01 00 00 00 78 00 08 00 00 00 00 d3 a0 c1 6c c0 0c 00 2f 80 01
udp-recv[mdns]: 00 00 11 94 00 09 c0 0c 00 05 00 00 80 00 40 c0 b0 00 2f 80 01 00 00 11 94 00 09 c0 b0 00 05 00
udp-recv[mdns]: 00 80 00 40 c0 ec 00 2f 80 01 00 00 11 94 00 09 c0 ec 00 05 00 00 80 00 40 c1 23 00 2f 80 01 00
udp-recv[mdns]: 00 11 94 00 09 c1 23 00 05 00 00 80 00 40 c1 c1 00 2f 80 01 00 00 11 94 00 09 c1 c1 00 05 00 00
udp-recv[mdns]: 80 00 40 c1 6c 00 2f 80 01 00 00 00 78 00 08 c1 6c 00 04 40 00 00 08 00 00 29 05 a0 00 00 11 94
udp-recv[mdns]: 00 12 00 04 00 0e 00 24 ac de 48 00 11 22 78 4f 43 4f f4 3a  (852 bytes)

udp-recv[mdns]: 2d 10 00 00 00 01 00 00 00 00 00 00 03 31 39 32 03 31 36 38 01 30 03 32 30 39 07 69 6e 2d 61 64
udp-recv[mdns]: 64 72 04 61 72 70 61 00 00 0c 00 01  (44 bytes)

#aliases announced
udp-send[mdns]: 00 00 84 00 00 00 00 01 00 00 00 00 03 5f 66 6b 04 5f 74 63 70 05 6c 6f 63 61 6c 00 00 0c 00 01
udp-send[mdns]: 00 00 00 00 00 31 20 34 66 62 31 61 31 35 35 35 33 33 36 35 37 33 32 33 32 32 30 32 30 32 30 66
udp-send[mdns]: 66 31 39 33 38 31 31 03 5f 66 6b 04 5f 74 63 70 05 6c 6f 63 61 6c 00  (87 bytes)

udp-recv[mdns]: 00 00 00 00 00 01 00 00 00 00 00 00 05 63 61 72 6c 61 05 6c 6f 63 61 6c 00 00 01 00 01  (29 bytes)

#invalid
udp-recv[mdns]: 00 01 01 54 77 74 a3 00 00 00 38 07 54 00 3f 3f 54 77 a7 cc 6c 38 3f f3 3f 80 08 09 dd 3f f3 70
udp-recv[mdns]: 20 35 30 20 35 37 20 36 38 20 33 30 20 64 66 20 00 80 08 09 dd 3f f3 70 f3 38 34 3f ff 00 e5

#invalid
udp-recv[mdns]: cd 44 ed 34 4d f9 ec d0 b5 00 8f 03 14 00 0b 0b 0a 60 c9 01 07 2f 00 2f 01 0b 67 c9 01 00 00 01
udp-recv[mdns]: b5 00 8f 03 14 00 0b 0b 0a 60 c9 01 07 2f 00 2f 01 0b 67 c9 01 00 00 01 5b 00 60 20 01 ed 00 00
udp-recv[mdns]: 0a 60 c9 01 07 2f 00 2f 01 0b 67 c9 01 00 00 01 5b 00 60 20 01 ed 00 00 00 60 25 05 60 00 01 02
udp-recv[mdns]: 01 0b 67 c9 01 00 00 01 5b 00 60 20 01

#invalid
udp-recv[mdns]: 00 01 01 5c ed 7c 77 00 00 00 44 07 5c 00 a9 a9 5c ed 7b d4 74 44 a9 cd d0 e4 08 38 d1 a9 cd 60
udp-recv[mdns]: 20 35 30 20 35 64 20 37 34 20 64 34 20 64 39 20 00 e4 08 38 d1 a9 cd 60 cd 44 a9 34 4d f9 ec d0
udp-recv[mdns]: 39 20 63 34 20 34 39 20 00 e4 08 38 d1 a9 cd 60 cd 44 a9 34 4d f9 ec d0 b5 00 8f 03 14 00 0b 0b
udp-recv[mdns]: 00 e4 08 38 d1 a9 cd 60 cd 44 a9 34 4d f9 ec d0 b5 00 8f 03 14 00 0b 0b 0a 60 c9 01 07 2f 00 2f
udp-recv[mdns]: cd 44 a9 34 4d f9 ec d0 b5 00 8f 03 14 00 0b 0b 0a 60 c9 01 07 2f 00 2f 01 0b 67 c9 01 00 00 01
udp-recv[mdns]: b5 00 8f 03 14 00 0b 0b 0a 60 c9 01 07 2f 00 2f 01 0b 67 c9 01 00 00 01 5b 00 60 20 01 a9 00 00
udp-recv[mdns]: 0a 60 c9 01 07 2f 00 2f 01 0b 67 c9 01 00 00 01 5b 00 60 20 01 a9 00 00 00 60 25 05 60 00 01 02
udp-recv[mdns]: 01 0b 67 c9 01 00 00 01 5b 00 60 20 01 a9 00 00 00 60 25 05 60 00 01 02 4b a4 68 09 25 04 02 c0
udp-recv[mdns]: 5b 00 60 20 01 a9 00 00 00 60 25 05 60 00 01 02 4b a4 68 09 25 04 02 c0 a9 07 68 a9 2d c0 04 04
udp-recv[mdns]: 00 60 25 05 60 00 01 02 4b a4 68 09 25 04 02 c0 a9 07 68 a9 2d c0 04 04 01 0a 20 00 00 18 d9 00
udp-recv[mdns]: 4b a4 68 09 25 04 02 c0 a9 07 68 a9 2d c0 04 04 01 0a 20 00 00 18 d9 00 18 04 78 00 00 80 00 f4
udp-recv[mdns]: a9 07 68 a9 2d c0 04 04 01 0a 20 00 00 18 d9 00 18 04 78 00 00 80 00 f4 01 48 2d 10 63 82 ec d0
udp-recv[mdns]: 01 0a 20 00 00 18 d9 00 18 04 78 00 00 80 00 f4 01 48 2d 10 63 82 ec d0 d0 e4 08 38 0a 0b b1 0a
udp-recv[mdns]: 18 04 78 00 00 80 00 f4 01

#invalid
udp-recv[mdns]: 00 01 01 5c 70 7c 77 00 00 00 44 07 5c 00 2c 2c 5c 70 7b d4 74 44 2c cd d0 f0 08 38 d1 2c cd 60
udp-recv[mdns]: 20 35 30 20 35 30 20 37 34 20 64 30 20 64 63 20 00 f0 08 38 d1 2c cd 60 cd 44 2c 34 4d f9 ec d0
udp-recv[mdns]: 63 20 63 34 20 34 39 20 00 f0 08 38 d1 2c cd 60 cd 44 2c 34 4d f9 ec d0 b5 7c 74 00 74 00 b5 00
udp-recv[mdns]: 00 f0 08 38 d1 2c cd 60 cd 44 2c 34 4d f9 ec d0 b5 7c 74 00 74 00 b5 00 64 6f 64 6f 7b 00 00 2f
udp-recv[mdns]: cd 44 2c 34 4d f9 ec d0 b5 7c 74 00 74 00 b5 00 64 6f 64 6f 7b 00 00 2f 70 00 8f 03 54 00 0b 0b
udp-recv[mdns]: b5 7c 74 00 74 00 b5 00 64 6f 64 6f 7b 00 00 2f 70 00 8f 03 54 00 0b 0b 0a 60 c9 01 07 6f 00 00
udp-recv[mdns]: 64 6f 64 6f 7b 00 00 2f 70 00 8f 03 54 00 0b 0b 0a 60 c9 01 07 6f 00 00 00 0b 67 c9 01 00 00 01
udp-recv[mdns]: 70 00 8f 03 54 00 0b 0b 0a 60 c9 01 07 6f 00 00 00 0b 67 c9 01 00 00 01 5b 00 60 20 01 2c 01 40
udp-recv[mdns]: 0a 60 c9 01 07 6f 00 00 00 0b 67 c9 01 00 00 01 5b 00 60 20 01 2c 01 40 45 60 25 05 60 00 01 02
udp-recv[mdns]: 00 0b 67 c9 01 00 00 01 5b 00 60 20

udp-recv[mdns]: 00 00 84 00 00 00 00 01 00 00 00 01 09 5f 73 65 72 76 69 63 65 73 07 5f 64 6e 73 2d 73 64 04 5f
udp-recv[mdns]: 75 64 70 05 6c 6f 63 61 6c 00 00 0c 00 01 00 00 11 94 00 0b 03 5f 66 6b 04 5f 74 63 70 c0 23 00
udp-recv[mdns]: 00 29 05 a0 00 00 11 94 00 12 00 04 00 0e 00 00 e2 33 8e 8a 13 25 3a 43 b2 b1 bf 1a

udp-recv[mdns]: 00 00 84 00 00 00 00 01 00 00 00 01 09 5f 73 65 72 76 69 63 65 73 07 5f 64 6e 73 2d 73 64 04 5f
udp-recv[mdns]: 75 64 70 05 6c 6f 63 61 6c 00 00 0c 00 01 00 00 11 94 00 0b 03 5f 66 6b 04 5f 74 63 70 c0 23 00
udp-recv[mdns]: 00 29 05 a0 00 00 11 94 00 12 00 04 00 0e 00 00 e2 33 8e 8a 13 25 3a 43 b2 b1 bf 1a

udp-recv[mdns]: 00 00 84 00 00 00 00 01 00 00 00 01 09 5f 73 65 72 76 69 63 65 73 07 5f 64 6e 73 2d 73 64 04 5f
udp-recv[mdns]: 75 64 70 05 6c 6f 63 61 6c 00 00 0c 00 01 00 00 11 94 00 0b 03 5f 66 6b 04 5f 74 63 70 c0 23 00
udp-recv[mdns]: 00 29 05 a0 00 00 11 94 00 12 00 04 00 0e 00 00 e2 33 8e 8a 13 25 16 14 86 df a4 9d

udp-recv[mdns]: 00 00 84 00 00 00 00 01 00 00 00 01 09 5f 73 65 72 76 69 63 65 73 07 5f 64 6e 73 2d 73 64 04 5f
udp-recv[mdns]: 75 64 70 05 6c 6f 63 61 6c 00 00 0c 00 01 00 00 11 94 00 0b 03 5f 66 6b 04 5f 74 63 70 c0 23 00
udp-recv[mdns]: 00 29 05 a0 00 00 11 94 00 12 00 04 00 0e 00 00 e2 33 8e 8a 13 25 3a 43 b2 b1 bf 1a

udp-recv[mdns]: 00 00 84 00 00 00 00 01 00 00 00 01 09 5f 73 65 72 76 69 63 65 73 07 5f 64 6e 73 2d 73 64 04 5f
udp-recv[mdns]: 75 64 70 05 6c 6f 63 61 6c 00 00 0c 00 01 00 00 11 94 00 0b 03 5f 66 6b 04 5f 74 63 70 c0 23 00
udp-recv[mdns]: 00 29 05 a0 00 00 11 94 00 12 00 04 00 0e 00 00 e2 33 8e 8a 13 25 3a 43 b2 b1 bf 1a
"""


class Packet:
    def __init__(self):
        self.data = []
        self.aliases = []
        self.from_device = False
        self.valid = True


inside = False
packets = []
aliases = []
packet = None
valid = True
for line in source.split("\n"):
    if line.startswith("#invalid"):
        valid = False
        continue
    if line.startswith("#aliases"):
        aliases = re.sub("#aliases\s+", "", line).split(",")
        continue
    l = re.sub("^.+:\s+", "", line)
    l = re.sub("\s+\(\d+\s+bytes\)", "", l)
    if l:
        if not packet:
            packet = Packet()
            packet.from_device = "udp-send" in line
            packet.valid = valid
            packet.aliases = aliases
            packets.append(packet)
            inside = True
            aliases = []
        row = ["0x" + b for b in l.split(" ")]
        packet.data.append(row)
    else:
        valid = True
        packet = None

print(
    """
#pragma once

typedef struct packet_t {
    uint8_t const *data;
    char const *name;
    size_t size;
    bool valid;
    bool from_device;
} packet_t;

"""
)

for i, packet in enumerate(packets):
    print("const uint8_t packet_%d[] = {" % (i))
    for row in packet.data:
        print("  " + ", ".join(row) + ",")
    print("};")

print("const packet_t packets[%d] = {" % (len(packets),))
for i, packet in enumerate(packets):
    print(
        '  { packet_%d, "packet_%d", sizeof(packet_%d), %s, %s },'
        % (
            i,
            i,
            i,
            "true" if packet.valid else "false",
            "true" if packet.from_device else "false",
        )
    )
print("};")

for i, packet in enumerate(packets):
    for alias in packet.aliases:
        print(
            'const packet_t packet_%s = { packet_%d, "packet_%d", sizeof(packet_%d), %s, %s };'
            % (
                alias,
                i,
                i,
                i,
                "true" if packet.valid else "false",
                "true" if packet.from_device else "false",
            )
        )

print()
