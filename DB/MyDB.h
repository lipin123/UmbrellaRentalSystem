#include<iostream>  
#include<string>  
#include<mysql.h>  
#include<vector>
using namespace std;

class MyDB
{
public:
	MyDB();
	~MyDB();
	bool InitDB(string host, string user, string pwd, string db_name,int port);
	bool ExeSQL(string sql);
	vector<string> NextRow();
	void FreeResult();
private:
	MYSQL *connection;
	MYSQL_RES *result;
	MYSQL_ROW sql_row;
};