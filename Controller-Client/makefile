CXX = gcc
CXXFLAG = -g -Wall -MMD	# may only work just for C++, but worth trying
OBJECTS = controller.o client-ip.o main.o	#variable (MACRO) - Only need to make sure this line is updated to work.
DEPENDS = ${OBJECTS:o=.d}	# generates the dependency files and name them the same as teh object file with .d extention
EXEC = prog

prog : ${OBJECTS}
	${CXX} ${CXXFLAG} ${OBJECTS} -o ${EXEC}
	
	
clean :
	rm -rf m${OBJECTS} ${EXEC} 
	
-include ${DEPENDS} #replaces all the explicit build rules manually entered
