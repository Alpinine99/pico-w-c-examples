# digital-in

A button task polls GP18 every 100 ms. On each **release** it gives a semaphore, and an LED task reacts by toggling between a red and a yellow LED.

## Wiring

| Part | Pin |
|---|---|
| button | GP18 (other side to 3V3) |
| red LED | GP16 |
| yellow LED | GP17 |

The code does not enable an internal pull resistor, so add an **external pull-down** (about 10 kΩ from GP18 to GND).

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../README.md)).

```bash
cd digital-in
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/dig_in.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## Using it

Press and release the button. The LED changes when you **release** it.

## What you should see

```
[button] pressed
[button] released
[led] red on
[led] yellow on
```

## Notes

The semaphore starts at 1, so the red LED turns on once at boot, before any button press.
