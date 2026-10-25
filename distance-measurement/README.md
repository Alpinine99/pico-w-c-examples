# distance-measurement

Measures distance with an **HC-SR04 ultrasonic sensor**, three ways. Each subfolder is a separate program built from this folder.

## Wiring (all three)

| HC-SR04 pin | Pico pin |
|---|---|
| VCC | VBUS (5 V) |
| TRIG | GP17 |
| ECHO | GP16, **through a voltage divider** |
| GND | GND |

ECHO is a 5 V signal and Pico GPIOs are 3.3 V only. Use a divider (for example 1 kΩ and 2 kΩ) or a 3.3 V-compatible sensor such as the HC-SR04P.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../README.md)).

```bash
cd distance-measurement
mkdir build && cd build
cmake ..
make -j4
```

| Program | Folder | Firmware |
|---|---|---|
| polling, no PIO | [`no-pio`](no-pio/README.md) | `build/no-pio/no-pio.uf2` |
| PIO + interrupt | [`pio`](pio/README.md) | `build/pio/pio.uf2` |
| PIO + OLED | [`ssd1306`](ssd1306/README.md) | `build/ssd1306/pio-ssd1306.uf2` |

The `ssd1306` program needs `PICO_LIB_PATH` (see its README). Configure fails if it is unset, so to build only the others, comment out `add_subdirectory(ssd1306)` in `CMakeLists.txt`.

Flash and view output as described in the [root README](../README.md).

## Formula

Distance in centimetres is `echo time / 2 * 0.0334 cm/µs`. The constant `0.0334` assumes 334 m/s; the usual figure at 20 °C is 343 m/s (`0.0343`), so readings are about 3% low.
