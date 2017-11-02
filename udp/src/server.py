import socket

size = 8192

sock_listen = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock_listen.bind(('localhost', 9876))

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
