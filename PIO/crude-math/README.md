# PIO/crude-math

Arithmetic done only with PIO shifts. C sends n = 0 to 9; the state machine returns `2n`, `4n`, `2n+1` and `4n+3`, showing each on four LEDs for about half a second, and C prints the four results.

## Wiring

LEDs on GP16 to GP19.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../README.md)).

```bash
cd PIO/crude-math
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/crude_math.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## Using it

Open the serial terminal first; the program waits for it before starting.

## What you should see

```
n=<n> x2=<a> x4=<b> x2+1=<c> x4+3=<d>
```
One line about every 3 seconds (about 2 s of LED display plus a 1 s sleep), n from 0 to 9, then repeating.

## Notes

Only the low 4 bits of each result show on the LEDs.
