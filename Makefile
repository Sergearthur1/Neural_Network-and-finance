CC=g++
CFLAGS= -std=c++11 -O2
LIBR= -I/usr/local/include/eigen3
prog.exe: Example.o NeuralNetwork.o testfinal.o marche.hpp genetic.hpp
	$(CC) -o prog.exe Example.o NeuralNetwork.o testfinal.o $(LIBR) $(CFLAGS)
	
marche.exe: marche.hpp testmarket.cpp
	$(CC) testmarket.cpp -o marche.exe $(CFLAGS)
	
testNN.exe: testNN.o NeuralNetwork.o Example.o
	$(CC) -o testNN.exe testNN.o NeuralNetwork.o Example.o $(LIBR) $(CFLAGS)
	
testNN.o: NeuralNetwork.hpp
	$(CC) -c testNN.cpp $(LIBR) $(CFLAGS)
	
NeuralNetwork.o: NeuralNetwork.hpp NeuralNetwork.cpp Example.o
	$(CC) -c NeuralNetwork.cpp $(LIBR) $(CFLAGS)

Example.o: Example.hpp Example.cpp 
	$(CC) -c Example.cpp $(LIBR) $(CFLAGS)
	
testfinal.o: testfinal.cpp Example.hpp NeuralNetwork.hpp marche.hpp genetic.hpp
	$(CC) -c testfinal.cpp $(LIBR) $(CFLAGS)

clean:
	rm *.o
	
clear_file:
	rm *.dat
	
