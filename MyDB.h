#ifndef __MYDB__
#define __MYDB__


#include<iostream>  
#include<mysql.h>  
#include<vector>

using namespace std;

enum userCol
{
	user_id,
	user_acc,
	user_pw,
	umbrella,
	hash_code
};
enum umbrellaCol
{
	umbrella_id,
	umbrella_status,
	borrower_id,
	slot_label
};

enum rentalSpotCol
{
	rs_id,
	umbrellas_id,
	structure,
	vacancy,
	latitude,
	longitude
};

struct slotArrangement
{
	int row;
	int col;

};
struct rentalSpot
{
	int rsID;
	vector<int> umbrellaID;
	slotArrangement structure;
	vector<char> vacancy;
	double lat;
	double lng;
};
struct umbrellas
{
	int umbrellaID;
	int umbrellaStatus;
	int locationID;
	int slotLabel;
};
struct user
{
	int userID;
	string userAcc;
	string userPW;
	int umbrella;
	string hashCode;
};
struct coordinates
{
	int lat;
	int lng;
};

class MyDB
{
private:
	MYSQL *connection;
	MYSQL_RES *result;
	MYSQL_ROW sql_row;
public:
	MyDB();
	~MyDB();
	bool InitDB(string host, string user, string pwd, string db_name,int port);
	bool ExeSQL(string sql);
	vector<string> NextRow();
	void FreeResult();
	void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);
	vector<rentalSpot> SearchSpot(double lat, double lng, int range);
	vector<user> UniSearch(userCol col,string val);
	vector<umbrellas> UniSearch(umbrellaCol col,string val);
	vector<rentalSpot> UniSearch(rentalSpotCol col, string val);
	int affectedRows();

	bool UniInsert(user row);
	bool UniInsert(umbrellas row);
	bool UniInsert(rentalSpot row);

	bool UniDelete(userCol col, string val);
	bool UniDelete(umbrellaCol col, string val);
	bool UniDelete(rentalSpotCol col, string val);

	bool UniUpdate(userCol col, string val, userCol conCol, string conVal);
	bool UniUpdate(umbrellaCol col, string val, umbrellaCol conCol, string conVal);
	bool UniUpdate(rentalSpotCol col, string val, rentalSpotCol conCol, string conVal);
};

#endif
