import socket

size = 8192

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('localhost', 9876))

counter = 0

print("udp server is running")
try:
    while True:
        data, address = sock.recvfrom(size)
        data = data.decode('utf-8')
        data = str(counter) + " " + data
        if counter < 50:
            counter += 1
        else:
            counter = 0
        print(data)
        sock.sendto(data.encode('utf-8'), address)
except Exception as e:
    print(e)
finally:
    sock.close()
    print("udp server closed")
