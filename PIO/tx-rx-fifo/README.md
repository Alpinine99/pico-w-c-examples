# PIO/tx-rx-fifo

Demonstrates the PIO FIFOs. C pushes eight values into the TX FIFO, printing the TX and RX levels and noting when the FIFO is full; then it reads eight values back and notes when the RX FIFO is empty. The state machine shows each value on four LEDs.

## Wiring

LEDs on GP16 to GP19.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../README.md)).

```bash
cd PIO/tx-rx-fifo
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/tx_rx_fifo.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## Using it

Open the serial terminal first; the program waits for it before starting.

## What you should see

```
[*] writing data ...
[!] TX fifo not full          (or "[!] TX fifo full" before a write)
[index=0 value=7] TX=<n> RX=<n>
...
[*] reading data ...
[!] RX fifo not empty         (or "[!] RX fifo empty" before a read)
[index=0 value=<n>] TX=<n> RX=<n>
...
```

## Notes

The PIO loops run 5 times each (`set x, 4` with `jmp x--` exits at 0), moving 10 words per round, while the C code sends and reads 8, so the printed levels will not line up one-to-one with the C loops.
