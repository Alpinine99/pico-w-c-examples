# no-pio

Measures distance by polling in a FreeRTOS task: pulses TRIG for 10 µs, busy-waits for ECHO to go high and low with `gpio_get`, and times it with `get_absolute_time`. One reading every 500 ms.

## Wiring

| HC-SR04 pin | Pico pin |
|---|---|
| VCC | VBUS (5 V) |
| TRIG | GP17 |
| ECHO | GP16, **through a voltage divider** |
| GND | GND |

ECHO is a 5 V signal and Pico GPIOs are 3.3 V only. Use a divider (for example 1 kΩ and 2 kΩ) or a 3.3 V-compatible sensor such as the HC-SR04P.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../README.md)).

Build from the `distance-measurement` folder (see its README); the output is `build/no-pio/no-pio.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## What you should see

```
distance=<cm> us=<echo microseconds>
```

## Notes

There are no timeouts: if the sensor is disconnected or ECHO never changes, the task waits forever.
