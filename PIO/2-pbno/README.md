# PIO/2-pbno

LED control with **two push buttons** read by a PIO state machine. Each subfolder is a separate program built from this folder.

## Wiring

| Part | Pins |
|---|---|
| LEDs (through resistors) | GP16 to GP19 |
| push buttons | GP14 and GP15, each between the pin and 3V3 (internal pull-downs enabled) |

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../README.md)).

```bash
cd PIO/2-pbno
mkdir build && cd build
cmake ..
make -j4
```

| Program | Folder | Firmware |
|---|---|---|
| two LED patterns | [`led-control`](led-control/README.md) | `build/led-control/led_control.uf2` |
| counter with reset | [`led-counter-with-reset`](led-counter-with-reset/README.md) | `build/led-counter-with-reset/led_counter_with_reset.uf2` |
| counter up/down | [`led-counter-up-down`](led-counter-up-down/README.md) | `build/led-counter-up-down/led_counter_up_down.uf2` |

## Note

These programs read both buttons with `in pins, 2` and compare the result with small constants, but the init code never calls `sm_config_set_in_shift`. The SDK default shifts **right**, which puts the pin bits at the top of the ISR, so the comparisons may never match. If the buttons do nothing, add `sm_config_set_in_shift(&config, false, false, 32);` (shift left) to the program's init function. This is unverified; I did not run these on hardware.
