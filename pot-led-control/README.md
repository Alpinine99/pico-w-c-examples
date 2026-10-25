# pot-led-control

Reads a potentiometer with the 12-bit ADC every 100 ms and sets an LED's brightness with 1 kHz PWM.

## Wiring

| Part | Pin |
|---|---|
| potentiometer wiper | GP26 (ADC0) |
| potentiometer ends | 3V3(OUT) and AGND |
| LED (through a resistor) | GP17 |

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../README.md)).

```bash
cd pot-led-control
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/pot_led.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## Using it

Turn the potentiometer; the LED brightens and dims.

## What you should see

```
pot_read=<0-4095> led_write=<0-999>
```
