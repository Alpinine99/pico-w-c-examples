# servo

Sweeps a hobby servo from 0 to 180 degrees and back, one degree every 15 ms. The PWM runs at 50 Hz with a pulse of 0.5 ms (0°) to 2.5 ms (180°).

## Wiring

| Servo wire | Connect to |
|---|---|
| signal | GP16 |
| power | an external 5 V supply (not the 3V3 pin) |
| ground | GND, shared with the Pico |

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../README.md)).

```bash
cd servo
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/servo.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

## Notes

0.5 to 2.5 ms is wider than the 1 to 2 ms many servos expect; if yours buzzes at the ends, reduce `min_pulse` and `max_pulse` in `servo_write`.
