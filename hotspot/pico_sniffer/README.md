# pico_sniffer

Access point plus a packet sniffer. It prints a colour-coded line for every IPv4 packet and mirrors every frame (truncated to 512 bytes) as UDP to a PC, where Wireshark can display it.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../README.md)).

Build from the `hotspot` folder (see its README); the output is `hotspot/build/pico_sniffer.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## Using it

1. Open the serial terminal **first**: the access point only starts once a USB terminal is connected.
2. Join the `pico net` network from the PC that will run Wireshark. The PC needs the address `192.168.4.17` (`PC_IP`), which is the second DHCP lease, so connect it second or give it that static address.
3. On the PC, run `./live-capture-wireshark.sh` from the `hotspot` folder (see its README).

## What you should see

One line per packet:

```
<ms> <src ip:port> -> <dst ip:port> <TCP|UDP|ICMP|...> len=<n> flags=<SAFR letters for TCP>
```

## Notes

- `main.c` calls `strdup` for the colour code of every printed packet and never frees it. This leaks memory per packet and will eventually exhaust the heap on a busy network. Using `const char *` pointing at string literals fixes it.
- `mirror_queue` is created inside `mirror_task`, but the sniffer hooks use it; this relies on that task running first (unverified).
- Frames are dropped silently when the queues are full.
- Code: `sniffer/sniffer.c` installs the network-interface hooks.
