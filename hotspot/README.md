# hotspot

Turns the Pico W into a Wi-Fi **access point** with a built-in DHCP server. Three programs are built from this folder: a plain hotspot, one that counts traffic per client, and one that sniffs and mirrors packets to a PC.

## Wiring

None. Uses the on-board Wi-Fi.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../README.md)).

```bash
cd hotspot
mkdir build && cd build
cmake ..
make -j4
```

This produces three firmware files in `build/`: `pico_net.uf2`, `pico_monitor.uf2` and `pico_sniffer.uf2`. Flash the one you want.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## Using it

Network settings (fixed in `CMakeLists.txt` and the `main.c` files):

| Setting | Value |
|---|---|
| SSID | `pico net` |
| password | `password123` (change it in `CMakeLists.txt`) |
| Pico address | `192.168.4.1` |
| DHCP leases | `192.168.4.16` to `.23` (8 clients) |

Connect a phone or laptop to the `pico net` network. See each program's folder for details:

| Folder | Program |
|---|---|
| [`pico_net`](pico_net/README.md) | plain access point |
| [`pico_monitor`](pico_monitor/README.md) | per-client byte counters |
| [`pico_sniffer`](pico_sniffer/README.md) | packet log and mirror to a PC |

### Capturing in Wireshark (for `pico_sniffer`)

Run the helper from this folder on the PC (needs `python3`, `wireshark` and `mkfifo`):

```bash
cd hotspot
./live-capture-wireshark.sh
```

It creates `/tmp/picofeed.fifo`, starts `listener.py` on UDP port 8080, and opens Wireshark reading the FIFO. `listener.py <port> <fifo>` wraps each mirrored UDP datagram in a pcap record (link type Ethernet). The script calls `./listener.py` by relative path, so run it from this folder. Allow inbound UDP 8080 on the PC.

The PC must have the address `192.168.4.17` (the second DHCP lease, or a static address), or change `PC_IP` in `pico_sniffer/main.c`.

## Notes

- `dnsserver/` is compiled into all three programs but never started.
- `dhcpserver/` is from the MicroPython project (MIT licence, see `dhcpserver/LICENSE`); `dnsserver/` is from Raspberry Pi (BSD-3-Clause).
- `listener.py` was tested by sending a synthetic frame and reading the FIFO: it emits a valid pcap global header and record. The shell script and Wireshark were not run.
