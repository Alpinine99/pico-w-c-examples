# pico-w-c-examples

Small, self-contained **Raspberry Pi Pico W** examples in C, built on the **Pico SDK** and the
**FreeRTOS kernel**. This page covers setting those two up once. Every example folder has its
own `README.md` with wiring, build and usage instructions.

Each folder is an independent CMake project; there is no top-level build.

## 1. Toolchain (Debian/Ubuntu)

```bash
sudo apt install build-essential git python3 cmake \
     gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
```

The projects declare CMake 3.12 to 3.15 as the minimum, but the Pico SDK itself needs a
reasonably recent CMake, so use your distribution's current package or `pip install cmake`.

## 2. Pico SDK

Clone it **with submodules** (USB, the Wi-Fi chip driver and lwIP live there):

```bash
mkdir -p ~/pico && cd ~/pico
git clone --recurse-submodules https://github.com/raspberrypi/pico-sdk.git
```

If you cloned without `--recurse-submodules`, run `git submodule update --init` inside
`pico-sdk`.

## 3. FreeRTOS kernel

```bash
cd ~/pico
git clone https://github.com/FreeRTOS/FreeRTOS-Kernel.git
```

Every example includes the RP2040 port from this path, so check that it exists in the
release you cloned:

```bash
ls ~/pico/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake
```

If the file is missing, check out a FreeRTOS-Kernel release that includes the RP2040 port.

## 4. Environment variables

Add these to `~/.bashrc` (or your shell profile) so every build finds them:

```bash
export PICO_SDK_PATH=$HOME/pico/pico-sdk
export FREERTOS_KERNEL_PATH=$HOME/pico/FreeRTOS-Kernel
```

Optional, only for some examples (their README says which):

| Variable | Used by |
|---|---|
| `PICO_LIB_PATH` | `ssd1306`, `file_system`, `distance-measurement/ssd1306`: a folder holding your own checkouts of an SSD1306 library and a littlefs library |
| `WIFI_SSID`, `WIFI_PASSWD` | `wifi`: the network the Pico joins |

## 5. How the setup is used by each project

Every project's top-level `CMakeLists.txt` does the same four things:

1. sets `PICO_BOARD` to `pico_w`,
2. includes `pico_sdk_import.cmake` from `$PICO_SDK_PATH/external/`,
3. includes `FreeRTOS_Kernel_import.cmake` from `$FREERTOS_KERNEL_PATH/portable/ThirdParty/GCC/RP2040/`,
4. calls `pico_sdk_init()` and adds its source folders.

The program links `pico_stdlib`, `FreeRTOS-Kernel` and `FreeRTOS-Kernel-Heap4`, and enables
USB stdio (UART stdio is turned off).

Each example carries its own `FreeRTOSConfig.h`. The shared settings are: preemptive
scheduler, **dual-core SMP** (`configNUM_CORES 2`), 1 kHz tick, 128 KB heap, and
**stack-overflow checking disabled**. Task stack sizes passed to `xTaskCreate` are counted in
**words**, so `256` means 1 KB.

## 6. Building and flashing any example

```bash
cd <example-folder>
mkdir build && cd build
cmake ..
make -j4
```

The `.uf2` output location is given in each example's README. To flash: hold **BOOTSEL**
while plugging in USB, wait for the `RPI-RP2` drive, and copy the `.uf2` onto it.

Examples print over USB serial:

```bash
screen /dev/ttyACM0 115200
```

Many of them wait for a terminal to connect before doing anything, so open one first.

## Troubleshooting the setup

| Symptom | Likely cause |
|---|---|
| CMake cannot find `pico_sdk_import.cmake` | `PICO_SDK_PATH` is not exported in this shell, or points at the wrong folder. |
| CMake cannot find `FreeRTOS_Kernel_import.cmake` | `FREERTOS_KERNEL_PATH` is unset, or the release you cloned has no RP2040 port at that path (step 3). |
| Missing headers for USB, lwIP or the Wi-Fi chip | The SDK was cloned without submodules (step 2). |
| `arm-none-eabi-gcc` not found | The toolchain packages from step 1 are not installed. |
| Configure fails in `ssd1306`, `file_system` or `distance-measurement/ssd1306` | `PICO_LIB_PATH` is unset; see those examples' READMEs. |
