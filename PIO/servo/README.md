# PIO/servo

Generates a 50 Hz servo signal in PIO on **GP26** and sweeps the servo from 0 to 180 degrees and back. C sends a new pulse width (500 to 2500 µs) through the TX FIFO once per 20 ms frame.

## Wiring

| Servo wire | Connect to |
|---|---|
| signal | GP26 |
| power | external 5 V |
| ground | GND, shared with the Pico |

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../README.md)).

```bash
cd PIO/servo
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/pio_servo.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

## Notes

The PIO program builds the constants 20000 (frame length) and 500 itself with `in` and `set` instructions, because `SET` only takes 5-bit values. The state machine runs at 2 MHz.
