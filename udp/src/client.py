import socket

size = 8192

for i in range(0, 51):
    try:
        msg = str(i).encode('utf-8')
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.sendto(msg, ('localhost', 9876))
        print(sock.recv(size))
        sock.close()

    except Exception as e:
        print(e)
        print("cannot reach the server")
