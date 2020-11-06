CXX = gcc
CXXFLAG = -g -Wall -MMD	# may only work just for C++, but worth trying
OBJECTS = Send.o server-ip.o serial.o #variable (MACRO) - Only need to make sure this line is updated to work.
DEPENDS = ${OBJECTS:o=.d}	# generates the dependency files and name them the same as teh object file with .d extention
EXEC = prog

prog : ${OBJECTS}
	${CXX} ${CXXFLAG} ${OBJECTS} -o ${EXEC}
	
	
clean :
	rm -rf ${OBJECTS} ${EXEC} 
	
-include ${DEPENDS} #replaces all the explicit build rules manually entered
