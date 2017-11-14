Server : Network.o json.o
	g++ -lm -o Server Network.o

Network : Network.cpp
	g++ -c -o Network.o Network.cpp

json.o: jsoncpp.cpp
	g++ -o json.o jsoncpp.cpp -c

clean:
	rm *.o Server
