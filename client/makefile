objects = ser.o handler.o util.o

server : $(objects)
	cc -Wall -o server $(objects)

main.o : util.h
utils.o : util.h

.PHONY : clean
clean :
	rm server $(objects)