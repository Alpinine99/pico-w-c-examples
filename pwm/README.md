# pwm

Fades an LED up and down using the hardware PWM block: a 1 MHz counter wrapping at 999 gives a **1 kHz** signal, and the duty cycle steps from 0 to 100% and back in 10% steps every 100 ms.

## Wiring

LED on **GP17** through a 220 to 330 Ω resistor to GND.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../README.md)).

```bash
cd pwm
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/pwm.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.
