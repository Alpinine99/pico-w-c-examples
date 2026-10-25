# user-in

An interactive prompt over USB serial. It asks for a name and an age, then prints a summary and whether you are an adult (18 or older).

## Wiring

None.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../README.md)).

```bash
cd user-in
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/user-in.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## Using it

Open the serial terminal, type your answers and press Enter.

- Name: up to 9 characters.
- Age: up to 3 digits; a value of 0 or less (or non-digits) shows `Invalid input, Enter age:` and asks again.
- Backspace and Delete are handled.

## What you should see

```
Whats your name? <you type>
Hello <name>, how old are you? <you type>
======================
NAME: ...
AGE: ...
ADULT: YES
======================
```

## Notes

Input longer than the field limit is not discarded; the extra characters are read by the next prompt. The program waits for the USB terminal to connect before it prints anything.
