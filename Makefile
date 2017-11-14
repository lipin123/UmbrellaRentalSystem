Server : Network.o
	g++ -lm -o Server Network.o

Network : Network.cpp
	g++ -c -o Network.o Network.cpp
