Server : Network.o json.o sha256.o UserNetwork.o
	g++ -lm -o 	Server	Network.o json.o UserNetwork.o sha256.o

json.o: jsoncpp.cpp
	g++ -o json.o jsoncpp.cpp -c

Network.o : Network.cpp json.o
	g++ -c -o Network.o Network.cpp json.o

sha256.o: sha256.cpp
	g++ -c -o sha256.o sha256.cpp

UserNetwork.o : UserNetwork.cpp
	g++ -c -o UserNetwork.o UserNetwork.cpp

clean:
	rm *.o Server
