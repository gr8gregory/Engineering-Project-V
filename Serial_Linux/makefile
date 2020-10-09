# _*_ MakeFile _*_

SOURCES = serial.c 
OBJS = ${SOURCES:.c =.o}
CC = gcc
PACKAGE = serial

all : ${OBJS}
	${CC} -o ${PACKAGE} ${OBJS}

.c.o:
	${CC} -c $<

clean:
	rm -f *.o

# end of file

