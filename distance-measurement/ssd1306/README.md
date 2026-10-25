# ssd1306 (distance + OLED)

The same PIO measurement as the `pio` program, plus an SSD1306 OLED that shows `dist: X cm`, refreshed on every reading.

## Wiring

| HC-SR04 pin | Pico pin |
|---|---|
| VCC | VBUS (5 V) |
| TRIG | GP17 |
| ECHO | GP16, **through a voltage divider** |
| GND | GND |

ECHO is a 5 V signal and Pico GPIOs are 3.3 V only. Use a divider (for example 1 kΩ and 2 kΩ) or a 3.3 V-compatible sensor such as the HC-SR04P.

OLED on I²C1:

| OLED pin | Pico pin |
|---|---|
| SDA | GP18 |
| SCL | GP19 |
| VCC / GND | 3V3 / GND |

Address `0x3C`, 400 kHz.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../README.md)). It also needs `PICO_LIB_PATH` pointing at a folder with `ssd1306/CMakeLists.txt` defining an `ssd1306` target (see the [`ssd1306`](../../ssd1306/README.md) example).

Build from the `distance-measurement` folder (see its README); the output is `build/ssd1306/pio-ssd1306.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## What you should see

The OLED shows the title `Distance measurement` and a live `dist: <n> cm` line; the serial terminal prints the same as the `pio` program.

## Notes

`distance.pio` is identical to the one in the `pio` folder.
