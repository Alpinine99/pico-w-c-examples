# pot-servo

A potentiometer position sets a servo angle. The ADC reading (0 to 4095) is mapped to a pulse of 500 to 2500 µs at 50 Hz, updated every 10 ms.

## Wiring

| Part | Pin |
|---|---|
| potentiometer wiper | GP26 (ADC0), ends to 3V3(OUT) and AGND |
| servo signal | GP16 |
| servo power | external 5 V, grounds shared |

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../README.md)).

```bash
cd pot-servo
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/pot-servo.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.
