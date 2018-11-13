CXX=g++ -std=c++11
objects = main.o commandparser.o monitor.o logger.o
monitor2 : $(objects)
	g++ -o monitor2 $(objects) -pthread


$(objects)  : parameter.h
commandparser.o : commandparser.h
monitor.o : monitor.h
logger.o:logger.h

.PHONY : clean
clean :
	rm monitor2 $(objects)
