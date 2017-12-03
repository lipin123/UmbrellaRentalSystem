Server : json.o Network.o sha256.o serverMain.o DB.o UserNetwork.o SpotNetwork.o
	g++ -o Server json.o Network.o sha256.o serverMain.o DB.o UserNetwork.o SpotNetwork.o -lpthread -lmysqlclient

json.o: jsoncpp.cpp
	g++ -o json.o jsoncpp.cpp -c -W -Wall

Network.o : Network.cpp
	g++ -c -o Network.o Network.cpp -W -Wall

UserNetwork.o : UserNetwork.cpp
	g++ -std=c++11 -c -o UserNetwork.o UserNetwork.cpp

SpotNetwork.o : SpotNetwork.cpp
	g++ -std=c++11 -c -o SpotNetwork.o SpotNetwork.cpp

sha256.o: sha256.cpp
	g++ -c -o sha256.o sha256.cpp -W -Wall

serverMain.o: serverMain.cpp 
	g++ -c serverMain.cpp -o serverMain.o -W -Wall

DB.o: MyDB.cpp
	g++ -std=c++11 -c MyDB.cpp -o DB.o -W -Wall

clean:
	rm *.o Server
