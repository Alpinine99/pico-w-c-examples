# irq_from_c

A PIO state machine debounces a button and raises PIO interrupt 0. A **C interrupt handler** toggles an LED and prints a message.

## Wiring

| Part | Pin |
|---|---|
| button | GP14, other side to 3V3 |
| LED | GP16 |

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../../README.md)).

Build from the `PIO/irq` folder (see its README); the output is `build/irq_from_c/irq_from_c.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## What you should see

```
waiting for interrupt     (every second)
interrupt fired!          (each button press)
```

## Notes

- `irq_init` calls `pio_sm_set_consecutive_pindirs(pio, sm, but_pin, 1, true)`, which sets the **button pin as an output**. Every other example passes `false` for button pins. With a button wired to 3V3 this drives against the button; change it to `false` (from reading the code, not run on hardware).
- The handler calls `printf`, which USB stdio does not support safely from an interrupt; fine for a demo.
