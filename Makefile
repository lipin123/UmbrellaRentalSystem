Server : Network.o json.o
	g++ -lm -o Server Network.o

Network : Network.cpp
	g++ -c -o Network.o Network.cpp

json.o: jsoncpp.cpp
	g++ -o json.o jsoncpp.cpp -c

sha256.o: sha256.cpp
	g++ -o sha256.o sha256.cpp

clean:
	rm *.o Server
