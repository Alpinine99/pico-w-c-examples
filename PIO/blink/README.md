# PIO/blink

Two LEDs alternate, driven entirely by a PIO state machine: GP16 is on for about half a second, then GP17 for about half a second, repeating. The CPU does nothing after setup.

## Wiring

LEDs on **GP16** and **GP17**, each through a resistor to GND.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../README.md)).

```bash
cd PIO/blink
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/pio_blink.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

## Notes

The state machine runs at 2 kHz; the timing comes from counted delay loops in `src/pio_blink.pio`.
