Server : json.o Network.o sha256.o serverMain.o DB.o UserNetwork.o SpotNetwork.o
	g++ -o Server json.o Network.o sha256.o serverMain.o DB.o UserNetwork.o SpotNetwork.o -lpthread -lmysqlclient -g

json.o: jsoncpp.cpp
	g++ -o json.o jsoncpp.cpp -c -W -Wall -g

Network.o : Network.cpp
	g++ -c -o Network.o Network.cpp -W -Wall -g

UserNetwork.o : UserNetwork.cpp
	g++ -std=c++11 -c -o UserNetwork.o UserNetwork.cpp -g

SpotNetwork.o : SpotNetwork.cpp
	g++ -std=c++11 -c -o SpotNetwork.o SpotNetwork.cpp -g

sha256.o: sha256.cpp
	g++ -c -o sha256.o sha256.cpp -W -Wall -g

serverMain.o: serverMain.cpp 
	g++ -c serverMain.cpp -o serverMain.o -W -Wall -g

DB.o: MyDB.cpp
	g++ -std=c++11 -c MyDB.cpp -o DB.o -W -Wall -g

clean:
	rm *.o Server
