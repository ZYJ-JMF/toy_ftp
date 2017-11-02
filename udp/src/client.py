import socket
import os

server_ip = "192.241.151.16"
server_port = 9876

size = 8192

sock_send = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
pid = os.fork()

if pid == 0:
    for i in range(0, 51):
        try:
            msg = str(i).encode('utf-8')
            sock_send.sendto(msg, (server_ip, server_port))
        except Exception as e:
            print(e)
else:
    while True: 
        print(sock_send.recv(size))
    sock_listen.close()
    sock_send.close()


