import socket
import struct
import sys
import time

PORT = int(sys.argv[1])
FIFO = sys.argv[2]

PCAP_GLOBAL_HDR = struct.pack('<IHHIIII',
    0xa1b2c3d4,  # magic number
    2, 4,        # version major, minor
    0,           # timezone
    0,           # sigfigs
    65535,       # snaplen
    1            # LINKTYPE_ETHERNET
)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind(("0.0.0.0", PORT))

# Opening the FIFO for writing blocks until Wireshark opens it for reading.
# This is intentional — it's how the two processes sync up.
with open(FIFO, "wb", buffering=0) as f:
    f.write(PCAP_GLOBAL_HDR)
    while True:
        data, addr = sock.recvfrom(65535)
        ts = time.time()
        ts_sec = int(ts)
        ts_usec = int((ts - ts_sec) * 1_000_000)
        rec_hdr = struct.pack('<IIII', ts_sec, ts_usec, len(data), len(data))
        try:
            f.write(rec_hdr)
            f.write(data)
        except BrokenPipeError:
            sys.exit(0)

