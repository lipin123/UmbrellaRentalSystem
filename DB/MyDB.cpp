#include<iostream>  
#include<cstdlib>
#include "MyDB.h"

using namespace std;

MyDB::MyDB()
{
	connection = mysql_init(NULL);  
	if (connection == NULL)
	{
		cout << "Error:" << mysql_error(connection);
		return;
	}
}

MyDB::~MyDB()
{
	if (connection != NULL)
	{
		mysql_close(connection);
	}
}

bool MyDB::InitDB(string host, string user, string pwd, string db_name,int port)
{ 
	connection = mysql_real_connect(connection, host.c_str(),
		user.c_str(), pwd.c_str(), db_name.c_str(), port, NULL, 0);

	mysql_query(connection, "SET NAMES GBK");
	if (connection == NULL)
	{
		cout << "Error:" << mysql_error(connection);
		return false;
	}
	return true;
}

bool MyDB::ExeSQL(string sql)
{
	int res;
	res = mysql_query(connection, sql.c_str());
	if (res)
	{
		cout << "Query Error:" << mysql_error(connection);
		return false;
	}
	result = mysql_store_result(connection);
	return true;
}

vector<string> MyDB::NextRow() //Extract the next line of data
{
	if (result)
	{
		vector<string> re;
		if (sql_row = mysql_fetch_row(result))
		{
			int i=mysql_field_count(connection);
			for (int p = 0; p < i; p++)
			{
				re.push_back(sql_row[p]);
			}
			return re;
		}
		else
			return re;
	}

}

void MyDB::FreeResult() //Release the memory occupied by the result set
{
	if (result != NULL)
		mysql_free_result(result);
}
	

