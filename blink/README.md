# blink

Blinks the Pico W's **on-board LED** every 500 ms. The LED is wired through the Wi-Fi chip, so the program uses `cyw43_arch_gpio_put` instead of a normal GPIO.

## Wiring

None. It uses the board's own LED.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../README.md)).

```bash
cd blink
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/blink.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## What you should see

```
[*] led on
[!] led off
```
repeating every 500 ms.

## Notes

The program links `pico_cyw43_arch_none`: Wi-Fi hardware is initialised only to reach the LED. The `lwipopts.h` in `src/` is not used by this example.
