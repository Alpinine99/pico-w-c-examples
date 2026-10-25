# pico_net

The simplest hotspot: starts the access point (`pico net`) and the DHCP server, then idles. Built from the `hotspot` folder as `pico_net.uf2`.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../README.md)).

Build from the `hotspot` folder (see its README); the output is `hotspot/build/pico_net.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## What you should see

```
[+] Hotspot active
```
printed once a USB terminal is connected.

## Notes

This program uses netmask `255.255.255.248` (/29) while the DHCP server hands out `192.168.4.16` to `.23`. With that mask those addresses are outside the gateway's subnet, so clients may get an address but be unable to reach the Pico (unverified). `pico_sniffer` uses `255.255.255.0`.
