# file_system

Mounts a littlefs filesystem in on-board flash, prints its size, creates `/var`, lists the root directory, then writes, reads back and deletes a test file.

## Wiring

None.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../README.md)). It also needs `PICO_LIB_PATH`: a folder that contains `littlefs/CMakeLists.txt` from your littlefs checkout. That file must define the targets `littlefs-lib` and `stdinit-lib` and provide the `pico_hal.h` header. This matches the `lurk101/pico-littlefs-lib` API, but the library is not included here.

     ```bash
     export PICO_LIB_PATH=$HOME/pico/lib
     ```

```bash
cd file_system
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/pico-fs.uf2`.

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
File system analysis
fs mounted
FS: block <n> size <n> used <n>
files and directories list
[name] ... [type] ... [size] ...
written data to file
data: Hello mate how are you man?
```

## Notes

Things to check (from reading the code, not run on hardware):

- `pico_write(file, buf, sizeof(buf)+1)` writes one byte past `buf`, and the result is compared with `strlen(buf)+1`, so it prints `write failed` even though data was written.
- `"write-test.txt"` has no leading slash, so the file is created in the root, not in `/var`.
- `pico_mount(false)` does not format. On a fresh board mounting fails until the filesystem is formatted once by passing `true` (unverified).
- A 1 KB task stack is tight for littlefs plus `printf`; raise it if the program crashes.
- The program first tries to delete a directory `/boot`, so on a fresh filesystem you will probably see `failed to remove directory /boot`; that message is expected when `/boot` does not exist (unverified).
