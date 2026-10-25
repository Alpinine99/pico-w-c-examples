# wifi

Joins your Wi-Fi network (WPA2-AES, 30 s timeout), connects to a TCP server, sends `Hello mate from pico w side!`, prints the reply and disconnects.

## Wiring

None. Uses the on-board Wi-Fi.

## Build

Needs `PICO_SDK_PATH` and `FREERTOS_KERNEL_PATH` (see the [root README](../README.md)). Set the network name and password **before** running `cmake`; they are compiled into the firmware at configure time:

```bash
export WIFI_SSID="my-network"
export WIFI_PASSWD="my-password"
```

Re-run `cmake ..` after changing them. Anyone with the `.uf2` file can read the password with `strings`.

```bash
cd wifi
mkdir build && cd build
cmake ..
make -j4
```

The firmware is written to `build/src/wifi_con.uf2`.

## Flash and run

Hold **BOOTSEL** while plugging the board in over USB, wait for the `RPI-RP2` drive to appear, and copy the `.uf2` file onto it. The board reboots into the program.

Output is printed over USB serial. Open a terminal on the board's port:

```bash
screen /dev/ttyACM0 115200
```

## Using it

1. Edit the server address in `src/main.c` (`inet_addr("192.168.41.6")`, port 8080) to your PC's IP address.
2. On the PC run the test server and allow TCP 8080 through the firewall:

   ```bash
   python3 server.py
   ```

   It accepts one connection at a time, prints what it received and replies `Hello client message received`.
3. Flash the Pico and open the serial terminal.

## What you should see

```
[+] Connecting to wifi network: <ssid> ...
[+] Connected. IP: [<address>]
[+] connecting to server ...
[+] connected to server
[+] request sent to server
[*] data: Hello client message received
[+] session ended
```
On failure you will see `[!] Wifi connection failed!` or `[!] Connect failed`.

## Notes

The task has a 10240-word (40 KB) stack and waits for the USB terminal before connecting. The Python test server (`server.py`) was run and confirmed to echo correctly.
