# PIO/pbno-counter

Each press-and-release of a push button advances a 4-bit counter on four LEDs. The PIO program does the debouncing and counting.

## Wiring

| Part | Pin |
|---|---|
| LEDs | GP16 to GP19 |
| push button | GP14, other side to 3V3 (internal pull-down enabled) |

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../README.md)).

```bash
cd PIO/pbno-counter
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/pbno_counter.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

## Notes

The name comes from "push button, normally open".
