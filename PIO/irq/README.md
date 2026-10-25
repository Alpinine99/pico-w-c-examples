# PIO/irq

Examples of PIO **interrupts**. Two programs are built from this folder; a third, `rtos_optmzd`, is a separate project with its own build.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../README.md)).

```bash
cd PIO/irq
mkdir build && cd build
cmake ..
make -j4
```

| Program | Folder | Firmware |
|---|---|---|
| PIO interrupt handled in C | [`irq_from_c`](irq_from_c/README.md) | `build/irq_from_c/irq_from_c.uf2` |
| state machines signalling each other | [`irq_sm_2_sm`](irq_sm_2_sm/README.md) | `build/irq_sm_2_sm/irq_sm_2_sm.uf2` |
| PIO interrupt waking a FreeRTOS task | [`rtos_optmzd`](rtos_optmzd/README.md) | build separately from its own folder |
