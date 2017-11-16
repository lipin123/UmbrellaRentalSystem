Server : UserNetwork.o json.o Network.o sha256.o serverMain.o
	g++ -o Server UserNetwork.o json.o Network.o sha256.o serverMain.o -lpthread

json.o: jsoncpp.cpp
	g++ -o json.o jsoncpp.cpp -c -W -Wall

Network.o : Network.cpp
	g++ -c -o Network.o Network.cpp -W -Wall

sha256.o: sha256.cpp
	g++ -c -o sha256.o sha256.cpp -W -Wall

UserNetwork.o : UserNetwork.cpp
	g++ -std=c++11 -c -o UserNetwork.o UserNetwork.cpp -W -Wall

serverMain.o: serverMain.cpp 
	g++ -c serverMain.cpp -o serverMain.o -W -Wall

clean:
	rm *.o Server
