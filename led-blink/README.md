# led-blink

Two FreeRTOS tasks share one `led_task` function, each with its own pin and delay, so two LEDs blink independently.

## Wiring

| LED | Pin | Period |
|---|---|---|
| yellow | GP17 | 1000 ms on / 1000 ms off |
| red | GP16 | 600 ms on / 600 ms off |

Wire each LED through a 220 to 330 Ω resistor to GND.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../README.md)).

```bash
cd led-blink
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/led_blink.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## What you should see

```
[*] led [17] on
[!] led [17] off
[*] led [16] on
...
```
