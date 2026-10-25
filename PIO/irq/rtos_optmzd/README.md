# rtos_optmzd

A button press raises a PIO interrupt; the interrupt handler wakes a FreeRTOS task with `vTaskNotifyGiveFromISR`. Meanwhile a higher-priority task prints once a second, showing the system keeps running.

## Wiring

Button on **GP15**, other side to 3V3 (internal pull-down enabled).

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../../README.md)). This is a **separate project** (its own `CMakeLists.txt` and `FreeRTOSConfig.h`); it is not built by the parent `PIO/irq` folder.

```bash
cd PIO/irq/rtos_optmzd
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/rtos_optmzd.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## What you should see

```
printT task is running ...       (every second)
 [!] irq fired                   (on a button press)
 [*] irq handled                 (the notified task running)
```

## Notes

- `main` stores `_pio = &pio;`, a pointer to a **local variable of `main()`**, and the interrupt handler dereferences it. Once the scheduler starts, FreeRTOS reuses `main()`'s stack for interrupts, so the pointer can read garbage. Using `pio0` directly (or a global) is safer. From reading the code; not run on hardware.
- The `FreeRTOSConfig.h` here differs from the others: it enables task notifications and sets interrupt priorities.
