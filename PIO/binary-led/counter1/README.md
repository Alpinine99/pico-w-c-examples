# counter1

A free-running 4-bit counter written entirely in PIO. After setup the C code does nothing: the state machine counts through 0 to 15 on its own, about every half second.

## Wiring

LEDs on GP16 to GP19.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../../README.md)).

Build from the `PIO/binary-led` folder (see its README); the output is `build/counter1/counter1.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.
