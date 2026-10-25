# pio

A PIO state machine sends the trigger and times the echo at **10 MHz**. When the echo ends it raises PIO IRQ 0; the interrupt handler wakes a FreeRTOS task with a notification, and the task reads the count from the RX FIFO and prints it.

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

Build from the `distance-measurement` folder (see its README); the output is `build/pio/pio.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## What you should see

```
ticks=<n> time_us=<n> dist_cm=<n>
```

## Notes

- Each counted loop is 3 PIO instructions = 0.3 µs, so `time_us = ticks * 0.3 / 2` (round trip halved).
- The PIO program re-triggers immediately after each echo, faster than the roughly 60 ms spacing HC-SR04 datasheets recommend (unverified effect on readings).
- The interrupt handler uses globals (`_pio`, `_sm`) and `vTaskNotifyGiveFromISR`.
