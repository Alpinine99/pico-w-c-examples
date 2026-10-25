# led-control

Two buttons choose between two LED patterns: one button shows `0101`, the other shows `1010`.

## Wiring

LEDs on GP16 to GP19; buttons on GP14 and GP15 (to 3V3, internal pull-downs).

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../../README.md)).

Build from the `PIO/2-pbno` folder (see its README); the output is `build/led-control/led_control.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

## Notes

See the shift-direction note in the [`PIO/2-pbno` README](../README.md).
