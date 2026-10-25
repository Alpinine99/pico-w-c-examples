# PIO

Examples of the RP2040's **programmable I/O (PIO)** blocks. A PIO program is written in a `.pio` file, compiled by `pioasm` into a generated header during the build, and started from a short C `main`. After setup the state machine runs by itself with no CPU involvement.

The "human-speed" examples slow the state machine to **2 kHz** (`clk_sys / 2000`) so you can watch LEDs change.

Common wiring used across these examples:

| Part | Pins |
|---|---|
| LEDs (through resistors) | GP16 to GP19 |
| push buttons | GP14 (and GP15 or GP13), wired between the pin and 3V3 |
| servos | GP26 (and GP27) |

Most buttons are configured with the internal pull-down enabled. `irq/irq_sm_2_sm` does not enable one, so it needs external pull-downs.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../README.md)).

Each folder below is built on its own (`cd` into it, then `mkdir build && cd build && cmake .. && make -j4`).

| Folder | What it does |
|---|---|
| [`blink`](blink/README.md) | two LEDs alternate, driven entirely by PIO |
| [`binary-led`](binary-led/README.md) | 4-bit binary counter on LEDs (two versions) |
| [`pbno-counter`](pbno-counter/README.md) | button press advances an LED counter |
| [`toggle-switch`](toggle-switch/README.md) | button toggles an LED pattern |
| [`servo`](servo/README.md) | 50 Hz servo signal from PIO |
| [`2-sm`](2-sm/README.md) | two servos on two state machines |
| [`2-pbno`](2-pbno/README.md) | two-button LED control (three programs) |
| [`crude-math`](crude-math/README.md) | shift-based arithmetic in PIO |
| [`tx-rx-fifo`](tx-rx-fifo/README.md) | TX/RX FIFO behaviour demo |
| [`irq`](irq/README.md) | PIO interrupts to C and between state machines |
| [`irq/rtos_optmzd`](irq/rtos_optmzd/README.md) | PIO interrupt waking a FreeRTOS task (separate project) |
