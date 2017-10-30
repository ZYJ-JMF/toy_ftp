objects = client.o handler.o util.o common.o

server : $(objects)
	cc -Wall -o client $(objects)

.PHONY : clean
clean :
	rm client $(objects)
