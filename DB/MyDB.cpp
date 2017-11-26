#include<iostream>  
#include<cstdlib>
#include <string>
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

vector<rentalSpot> MyDB::SearchSpot(double lat, double lng, int range)
{
	
		vector<rentalSpot> re;
		int res = 0, resTableRow = 0, resTableCol = 0;
		string reqStr;

		reqStr = "call dist(" + to_string(lat) + "," + to_string(lng) + "," + to_string(range) + ");";
		res = mysql_query(connection, reqStr.c_str());
		if (res)
		{
			cout << "Query Error:" << mysql_error(connection);
			return re;
		}
		result = mysql_store_result(connection);
		resTableCol = mysql_num_fields(result);
		resTableRow = mysql_num_rows(result);


		if (result)
		{
			
			for (int p = 0; p < resTableRow; p++)
			{
				if (sql_row = mysql_fetch_row(result))
				{
					rentalSpot tem;
					vector<string> temStrVec;
					tem.rsID = atoi(sql_row[0]); //雨伞id

					//储存雨伞的编号
					SplitString(sql_row[1], temStrVec, ",");
					for (int i = 0; i < temStrVec.size(); i++)
						tem.umbrellaID.push_back(atoi(temStrVec[i].c_str()));

					//将vacancy列中的数据分割成行和列,并储存。
					vector <string>().swap(temStrVec);
					SplitString(sql_row[2], temStrVec, "x");
					tem.structure.row = atoi(temStrVec[0].c_str()); //租伞机器插槽的行
					tem.structure.col = atoi(temStrVec[1].c_str()); //租伞机器插槽的列

					string temvac = sql_row[3]; //保存插槽状态信息(是否可用)
					tem.vacancy.reserve(tem.vacancy.size() + temvac.length());
					tem.vacancy.insert(tem.vacancy.end(), temvac.c_str(), temvac.c_str() + temvac.length());

					tem.lat = atof(sql_row[4]); //保存纬度
					tem.lng = atof(sql_row[5]);	 //保存经度
					re.push_back(tem);
				}
				else
				{
					break;
				}
			}

		}
		return re;

	
}

vector<user> MyDB::UniSearch(userCol col, string val)
{
	int res = 0, resTableRow = 0;
	vector<user> re;
	string inqStr = "select * from user where ";
	
	switch (col)
	{
	case user_id:
		inqStr += "user_id=";
		break;
	case user_acc:
		inqStr += "user_acc=";
		break;
	case user_pw:
		inqStr += "user_pw=";
		break;
	case umbrella:
		inqStr += "umbrella=";
		break;
	case hash_code:
		inqStr += "hash_code=";
		break;
	default:
		break;
	}
	inqStr += val;

	res = mysql_query(connection, inqStr.c_str());
	if (res)
	{
		cout << "Query Error:" << mysql_error(connection);
		return re;
	}
	result = mysql_store_result(connection);
	resTableRow = mysql_num_rows(result);


	if (result)
	{
		for (int p = 0; p < resTableRow; p++)
		{
			if (sql_row = mysql_fetch_row(result))
			{
				user tem;
				tem.userID= atoi(sql_row[0]); //雨伞ID
				tem.userAcc = sql_row[1];
				tem.userPW = sql_row[2];
				tem.umbrella = atoi(sql_row[3]);
				tem.hashCode = sql_row[4];
				re.push_back(tem);
			}
			else
			{
				break;
			}
		}
	}
	return re;
}

vector<umbrellas> MyDB::UniSearch(umbrellaCol col, string val)
{

	int res = 0, resTableRow = 0;
	vector<umbrellas> re;
	string inqStr = "select * from umbrella where ";

	switch (col)
	{

	case umbrella_id:
		inqStr += "umbrella_id=";
		break;
	case umbrella_status:
		inqStr += "umbrella_status=";
		break;
	case borrower_id:
		inqStr += "borrower_id=";
		break;
	case slot_label:
		inqStr += "slot_label=";
		break;
	default:
		break;
	}
	inqStr += val;

	res = mysql_query(connection, inqStr.c_str());
	if (res)
	{
		cout << "Query Error:" << mysql_error(connection);
		return re;
	}
	result = mysql_store_result(connection);
	resTableRow = mysql_num_rows(result);

	if (result)
	{
		for (int p = 0; p < resTableRow; p++)
		{
			if (sql_row = mysql_fetch_row(result))
			{
				umbrellas tem;
				tem.umbrellaID = atoi(sql_row[0]); //雨伞ID
				tem.umbrellaStatus = atoi( sql_row[1]);
				tem.locationID = atoi(sql_row[2]);
				tem.slotLabel = atoi(sql_row[3]);
				re.push_back(tem);
			}
			else
			{
				break;
			}
		}
	}
	return re;
}

vector<rentalSpot> MyDB::UniSearch(rentalSpotCol col, string val)
{
	int res = 0, resTableRow = 0;
	vector<rentalSpot> re;
	string inqStr = "select * from rental_spot where ";

	switch (col)
	{
	case rs_id:
		inqStr = inqStr+"rs_id="+val;
		break;
	case umbrellas_id:
		inqStr = inqStr + "umbrellas_id='" + val + "';";
		break;
	case structure:
		inqStr = inqStr + "structure='" + val + "';";
		break;
	case vacancy:
		inqStr = inqStr+ "vacancy='" + val + "';";
		break;
	case latitude:
		inqStr = inqStr + "latitude=" + val;
		break;
	case longitude:
		inqStr = inqStr + "longitude=" + val;
		break;
	default:
		break;
	}
	
	res = mysql_query(connection, inqStr.c_str());
	if (res)
	{
		cout << "Query Error:" << mysql_error(connection);
		return re;
	}
	result = mysql_store_result(connection);
	resTableRow = mysql_num_rows(result);

	if (result)
	{
		for (int p = 0; p < resTableRow; p++)
		{
			if (sql_row = mysql_fetch_row(result))
			{
				rentalSpot tem;
				vector<string> temStrVec;
				tem.rsID = atoi(sql_row[0]); //雨伞id

											 //储存雨伞的编号
				SplitString(sql_row[1], temStrVec, ",");
				for (int i = 0; i < temStrVec.size(); i++)
					tem.umbrellaID.push_back(atoi(temStrVec[i].c_str()));

				//将vacancy列中的数据分割成行和列,并储存。
				vector <string>().swap(temStrVec);
				SplitString(sql_row[2], temStrVec, "x");
				tem.structure.row = atoi(temStrVec[0].c_str()); //租伞机器插槽的行
				tem.structure.col = atoi(temStrVec[1].c_str()); //租伞机器插槽的列

				string temvac = sql_row[3]; //保存插槽状态信息(是否可用)
				tem.vacancy.reserve(tem.vacancy.size() + temvac.length());
				tem.vacancy.insert(tem.vacancy.end(), temvac.c_str(), temvac.c_str() + temvac.length());

				tem.lat = atof(sql_row[4]); //保存纬度
				tem.lng = atof(sql_row[5]);	 //保存经度
				re.push_back(tem);
			}
			else
			{
				break;
			}
		}
	}
	return re;
}

bool MyDB::UniInsert(user row)
{
	string inqStr = "call Uni_insert('user',\"";

	inqStr = inqStr + to_string(row.userID) + ",'" + row.userAcc + "','" + row.userPW + "'," + to_string(row.umbrella) + ",'" + row.hashCode + "'";
	inqStr += "\");";
	return ExeSQL(inqStr);
}

bool MyDB::UniInsert(umbrellas row)
{
	string inqStr = "call Uni_insert('umbrella','";

	inqStr = inqStr + to_string(row.umbrellaID) + "," + to_string(row.umbrellaStatus) + "," + to_string(row.locationID) + "," + to_string(row.slotLabel);
	inqStr += "');";

	return ExeSQL(inqStr);
}

bool MyDB::UniInsert(rentalSpot row)
{
	string inqStr = "call Uni_insert('rental_spot',\"";

	string umbID, structure, vacancy;
	for (int i = 0; i < row.umbrellaID.size(); i++)
	{
		umbID += to_string(row.umbrellaID[i]);
		if (i + 1 < row.umbrellaID.size())
			umbID += ",";
	}
	structure = to_string(row.structure.row) + "x" + to_string(row.structure.col);
	for (int x = 0; x < row.vacancy.size(); x++)
	{
		vacancy += row.vacancy[x];
	}
	inqStr = inqStr + to_string(row.rsID) + ",'" + umbID + "','" + structure + "','" + vacancy + "'," + to_string(row.lat) + "," + to_string(row.lng);
	inqStr += "\");";

	return ExeSQL(inqStr);
}

bool MyDB::UniDelete(userCol col, string val)
{
	string inqStr = "call Uni_delete('user',\"";
	switch (col)
	{
	case user_id:
		inqStr = inqStr+ "user_id="+val;
		break;
	case user_acc:
		inqStr = inqStr+ "user_acc='"+val+"'";
		break;
	case user_pw:
		inqStr = inqStr+ "user_pw='"+val+"'";
		break;
	case umbrella:
		inqStr += "umbrella="+val;
		break;
	case hash_code:
		inqStr += "hash_code='"+val+"'";
		break;
	default:
		break;
	}
	inqStr += "\");";

	return ExeSQL(inqStr);
}

bool MyDB::UniDelete(umbrellaCol col, string val)
{
	string inqStr = "call Uni_delete('umbrella',\"";
	switch (col)
	{

	case umbrella_id:
		inqStr = inqStr + "umbrella_id=" + val;
		break;
	case umbrella_status:
		inqStr = inqStr+ "umbrella_status="+val;
		break;
	case borrower_id:
		inqStr = inqStr+ "borrower_id="+val;
		break;
	case slot_label:
		inqStr = inqStr+ "slot_label="+val;
		break;
	default:
		break;
	}
	inqStr += "\");";

	return ExeSQL(inqStr);
}

bool MyDB::UniDelete(rentalSpotCol col, string val)
{
	string inqStr = "call Uni_delete('rental_spot',\"";

	switch (col)
	{
	case rs_id:
		inqStr = inqStr + "rs_id=" + val;
		break;
	case umbrellas_id:
		inqStr = inqStr + "umbrella_id='" + val + "'";
		break;
	case structure:
		inqStr = inqStr + "structure='" + val + "'";
		break;
	case vacancy:
		inqStr = inqStr + "vacancy='" + val + "'";
		break;
	case latitude:
		inqStr = inqStr + "latitude=" + val;
		break;
	case longitude:
		inqStr = inqStr + "longitude=" + val;
		break;
	default:
		break;
	}
	inqStr += "\");";

	return ExeSQL(inqStr);
}

bool MyDB::UniUpdate(userCol col, string val, userCol conCol, string conVal)
{
	string inqStr = "call Uni_update('user',\"";
	switch (col)
	{
	case user_id:
		inqStr = inqStr + "user_id=" + val;
		break;
	case user_acc:
		inqStr = inqStr + "user_acc='" + val + "'";
		break;
	case user_pw:
		inqStr = inqStr + "user_pw='" + val + "'";
		break;
	case umbrella:
		inqStr += "umbrella=" + val;
		break;
	case hash_code:
		inqStr += "hash_code='" + val + "'";
		break;
	default:
		break;
	}
	inqStr += "\",\"";
	switch (conCol)
	{
	case user_id:
		inqStr = inqStr + "user_id=" + conVal;
		break;
	case user_acc:
		inqStr = inqStr + "user_acc='" + conVal + "'";
		break;
	case user_pw:
		inqStr = inqStr + "user_pw='" + conVal + "'";
		break;
	case umbrella:
		inqStr += "umbrella=" + conVal;
		break;
	case hash_code:
		inqStr += "hash_code='" + conVal + "'";
		break;
	default:
		break;
	}
	inqStr += "\");";
	return ExeSQL(inqStr);
}

bool MyDB::UniUpdate(umbrellaCol col, string val, umbrellaCol conCol, string conVal)
{
	string inqStr = "call Uni_update('umbrella',\"";
	switch (col)
	{
	case umbrella_id:
		inqStr = inqStr + "umbrella_id=" + val;
		break;
	case umbrella_status:
		inqStr = inqStr + "umbrella_status=" + val;
		break;
	case borrower_id:
		inqStr = inqStr + "borrower_id=" + val;
		break;
	case slot_label:
		inqStr = inqStr + "slot_label=" + val;
		break;
	default:
		break;
	}
	inqStr += "\",\"";
	switch (conCol)
	{
	case umbrella_id:
		inqStr = inqStr + "umbrella_id=" + conVal;
		break;
	case umbrella_status:
		inqStr = inqStr + "umbrella_status=" + conVal;
		break;
	case borrower_id:
		inqStr = inqStr + "borrower_id=" + conVal;
		break;
	case slot_label:
		inqStr = inqStr + "slot_label=" + conVal;
		break;
	default:
		break;
	}
	inqStr += "\");";
	return ExeSQL(inqStr);
}

bool MyDB::UniUpdate(rentalSpotCol col, string val, rentalSpotCol conCol, string conVal)
{
	string inqStr = "call Uni_update('rental_spot',\"";
	switch (col)
	{
	case rs_id:
		inqStr = inqStr + "rs_id=" + val;
		break;
	case umbrellas_id:
		inqStr = inqStr + "umbrella_id='" + val + "'";
		break;
	case structure:
		inqStr = inqStr + "structure='" + val + "'";
		break;
	case vacancy:
		inqStr = inqStr + "vacancy='" + val + "'";
		break;
	case latitude:
		inqStr = inqStr + "latitude=" + val;
		break;
	case longitude:
		inqStr = inqStr + "longitude=" + val;
		break;
	default:
		break;
	}
	inqStr += "\",\"";
	switch (conCol)
	{
	case rs_id:
		inqStr = inqStr + "rs_id=" + conVal;
		break;
	case umbrellas_id:
		inqStr = inqStr + "umbrella_id='" + conVal + "'";
		break;
	case structure:
		inqStr = inqStr + "structure='" + conVal + "'";
		break;
	case vacancy:
		inqStr = inqStr + "vacancy='" + conVal + "'";
		break;
	case latitude:
		inqStr = inqStr + "latitude=" + conVal;
		break;
	case longitude:
		inqStr = inqStr + "longitude=" + conVal;
		break;
	default:
		break;
	}
	inqStr += "\");";

	return ExeSQL(inqStr);
}

//分割字符串
void MyDB::SplitString(const std::string & s, std::vector<std::string>& v, const std::string & c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}


	

