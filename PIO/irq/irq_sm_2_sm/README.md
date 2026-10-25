# irq_sm_2_sm

Four state machines on one PIO block. One reads three buttons and raises PIO interrupt flags 0 to 2; three LED state machines each wait for a flag and toggle their LED. No C code runs after setup.

## Wiring

| Part | Pins |
|---|---|
| buttons | GP13, GP14, GP15, each between the pin and 3V3, with **external pull-downs** (the code does not enable internal ones) |
| LEDs | GP16, GP17, GP18 |

As written, flag 0 (button GP13) toggles the LED on GP17, flag 1 (GP14) toggles GP16, and flag 2 (GP15) toggles GP18.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../../README.md)).

Build from the `PIO/irq` folder (see its README); the output is `build/irq_sm_2_sm/irq_sm_2_sm.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

## Notes

Uses all four state machines of `pio0`. The button read uses `in pins, 3` without setting the shift direction; see the note in the [`PIO/2-pbno` README](../../2-pbno/README.md) (unverified).
