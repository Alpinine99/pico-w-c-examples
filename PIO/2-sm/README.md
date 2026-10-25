# PIO/2-sm

Two state machines each run a copy of the servo program, driving two servos on **GP26** and **GP27** in opposite directions.

## Wiring

| Servo | Signal pin |
|---|---|
| 1 | GP26 |
| 2 | GP27 |

Power both from an external 5 V supply with grounds shared with the Pico.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../README.md)).

```bash
cd PIO/2-sm
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/2_sm.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## What you should see

```
pw1=<n>, pw2=<n>, angle=<n>
```
printed continuously, so the terminal fills quickly.
