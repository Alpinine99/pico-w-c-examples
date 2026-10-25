# counter

C sends the numbers 0 to 15 through the PIO TX FIFO; the state machine shows each on four LEDs, reads the pin states back, and returns them through the RX FIFO. C prints both values.

## Wiring

LEDs on GP16 to GP19.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../../README.md)).

Build from the `PIO/binary-led` folder (see its README); the output is `build/counter/counter.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## What you should see

```
d_in=<sent> d_out=<read back>
```
A new value every 250 ms, wrapping after 15.
