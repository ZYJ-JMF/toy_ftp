objects = server.o handler.o util.o common.o

server : $(objects)
	cc -Wall -o server $(objects)

.PHONY : clean
clean :
	rm server $(objects)