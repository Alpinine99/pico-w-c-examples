# PIO/binary-led

A 4-bit binary counter shown on four LEDs, in two versions. Each subfolder is a separate program built from this folder.

## Wiring

LEDs on **GP16 to GP19** (GP16 is the lowest bit), each through a resistor to GND.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../../README.md)).

```bash
cd PIO/binary-led
mkdir build && cd build
cmake ..
make -j4
```

| Program | Folder | Firmware |
|---|---|---|
| C-driven counter | [`counter`](counter/README.md) | `build/counter/counter.uf2` |
| PIO-only counter | [`counter1`](counter1/README.md) | `build/counter1/counter1.uf2` |
