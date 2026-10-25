# PIO/toggle-switch

Each press of a push button toggles four LEDs between the patterns `1010` and `0101`. Starts at `1010`.

## Wiring

| Part | Pin |
|---|---|
| LEDs | GP16 to GP19 |
| push button | GP14, other side to 3V3 (internal pull-down enabled) |

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../README.md)).

```bash
cd PIO/toggle-switch
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/toggle.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.
