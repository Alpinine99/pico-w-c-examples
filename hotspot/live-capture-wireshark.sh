#!/usr/bin/env bash
#
# pico_sniff.sh — receives mirrored UDP frames from the Pico W and
# streams them live into Wireshark via a named pipe (FIFO).
#
set -euo pipefail

# ---- Config ----
LISTEN_PORT=8080
FIFO_PATH="/tmp/picofeed.fifo"
LISTENER_PY="./listener.py"

# ---- Dependency checks ----
command -v python3 >/dev/null 2>&1 || { echo "python3 not found"; exit 1; }
command -v wireshark >/dev/null 2>&1 || { echo "wireshark not found"; exit 1; }
command -v mkfifo >/dev/null 2>&1 || { echo "mkfifo not found"; exit 1; }

# ---- FIFO setup ----
[[ -p "$FIFO_PATH" ]] && rm -f "$FIFO_PATH"
mkfifo "$FIFO_PATH"

LISTENER_PID=""

cleanup() {
  echo ""
  echo "[*] Cleaning up..."
  if [[ -n "$LISTENER_PID" ]] && kill -0 "$LISTENER_PID" 2>/dev/null; then
    kill "$LISTENER_PID" 2>/dev/null || true
    wait "$LISTENER_PID" 2>/dev/null || true
  fi
  rm -f "$FIFO_PATH"
}

trap cleanup EXIT INT TERM

# ---- Start listener in background (blocks on FIFO open until Wireshark connects) ----
echo "[*] Starting UDP listener on port $LISTEN_PORT -> $FIFO_PATH"
python3 "$LISTENER_PY" "$LISTEN_PORT" "$FIFO_PATH" &
LISTENER_PID=$!

# ---- Launch Wireshark reading from the FIFO ----
echo "[*] Launching Wireshark (waiting for Pico traffic)..."
wireshark -k -i "$FIFO_PATH"

