# led-counter-with-reset

A 4-bit counter on the LEDs: one button steps the counter and the other resets it. The exact button-to-function mapping depends on the bit order of the two-button read (not verified).

## Wiring

LEDs on GP16 to GP19; buttons on GP14 and GP15 (to 3V3, internal pull-downs).

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../../README.md)).

Build from the `PIO/2-pbno` folder (see its README); the output is `build/led-counter-with-reset/led_counter_with_reset.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

## Notes

See the shift-direction note in the [`PIO/2-pbno` README](../README.md).
