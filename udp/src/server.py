import socket

size = 8192

server_ip = 'localhost'
server_port = 9876
sock_listen = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock_listen.bind((server_ip, server_port))

counter = 0

print("udp server is running")
try:
    while True:
        data, address = sock_listen.recvfrom(size)
        data = data.decode('utf-8')
        data = str(counter) + " " + data
        if counter < 50:
            counter += 1
        else:
            counter = 0
        print(data)
        sock_listen.sendto(data.encode('utf-8'), address)
except Exception as e:
    print(e)
finally:
    sock_listen.close()
    print("udp server closed")
