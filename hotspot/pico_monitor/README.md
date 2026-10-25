# pico_monitor

Access point plus a per-client traffic counter. It hooks the access point's network interface, counts bytes by MAC address, and redraws a table every 5 seconds.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../README.md)).

Build from the `hotspot` folder (see its README); the output is `hotspot/build/pico_monitor.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## Using it

Open the serial terminal **first**: the access point only starts once a USB terminal is connected. Then join the `pico net` network and generate some traffic.

## What you should see

```
+---------------------+---------+---------+
|   MAC               |RX       |TX       |
+---------------------+---------+---------+
|aa:bb:cc:dd:ee:ff    |1234     |5678     |
+---------------------+---------+---------+
```
RX is bytes from the client to the Pico; TX is bytes from the Pico to the client.

## Notes

- Up to 8 MAC addresses are tracked (`MAX_CLIENTS`). Destination addresses of outgoing frames are counted too, so broadcast and multicast addresses use up slots; once full, new addresses are silently not counted.
- Same /29 netmask caveat as `pico_net` (see its README).
- Code: `monitor/monitor.c` installs the hooks, `main.c` prints the table.
