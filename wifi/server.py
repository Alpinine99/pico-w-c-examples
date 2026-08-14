import socket
from time import sleep


def main(port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind(("0.0.0.0", port))
    sock.listen(1)
    print("[server] Waiting for connection ... ");
    while True:
        try:
            client, addr = sock.accept()
            data = client.recv(1024)
            print(f"[client: {addr}] data: {data.decode()}")
            client.send("Hello client message received".encode())
            print("[server] request responded")
            client.close()
        except Exception as e:
            print(f"[error] {e}")

main(8080)

