## UDP
### UDP client&UDP server
Modified UDP Server and UDP Client in python3 **not default python2**. 

So please make sure you use python3 while running server.py and client.py.

Use fork() to create a new process for receiving reply from server. Thus the **order problem** can be seen.

Settings: localhost, port 9876 as default. Can be modified.

### Question1:How to write a chat program with UDP?
To write a simple chat program, firstly we need a server and some clients. If one client want to send message to another client, The it needs to send packets containing chat info and target client to server. The server receives packets from the sender, analyse it and then send it to the target client.

To avoid problems in the order of message, we can add timestamp info to message so that the server can analyse the message and send them in the right order.

By the way, it is said that QQ was first written with UDP.
### Question2:Can we use the UDP to transfer a file? If so, how?
Yes.

The key problems for UDP to transfer a file are order and packet-loss. For the first problem, we can define a sequece number when sending packets. Then the receiver will be able to deal with the packets correctly despite the order of arriving. As for packet-loss, we can set a rule for the sender and receiver to check whether the receiver get the packet. For example, the receiver sends a message to the sender every time it gets a packet, and the sender sends next packet only after it receives that message. If the sender doesn't get that message in a while, it resends the packet.(Undoubtedly it will be slower by doing so, then we can adjust the threshold of message, e.g. send that success message every n packets)
