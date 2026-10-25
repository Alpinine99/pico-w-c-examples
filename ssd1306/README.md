# ssd1306

Drives a 128x64 SSD1306 OLED over I²C: draws text, a line and a numbered checklist, powers the display off and on, then shows text in four different fonts.

## Wiring

| OLED pin | Pico pin |
|---|---|
| SDA | GP18 |
| SCL | GP19 |
| VCC / GND | 3V3 / GND |

I²C1, address `0x3C`, 400 kHz, internal pull-ups enabled.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../README.md)). It also needs `PICO_LIB_PATH`: a folder that contains `ssd1306/CMakeLists.txt` from your SSD1306 library checkout. That file must define a target named `ssd1306`, and the library must provide `ssd1306.h` and the fonts under `example/` (`BMSPA_font.h`, `acme_5_outlines_font.h`, `crackers_font.h`, `bubblesstandard_font.h`). The API matches the `daschr/pico-ssd1306` library, but the library is not included here.

     ```bash
     export PICO_LIB_PATH=$HOME/pico/lib
     ```

```bash
cd ssd1306
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/ssd1306-display.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.
