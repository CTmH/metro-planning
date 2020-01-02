/*
1.private添加
  int sta_num;
  char input_type;
  struct station Sta[1000];//由站点组成的集合,从1开始编号
  int line_num;
2.修改为int Init_gph();在读取txt的时候写入站点信息。
3. 修改Station.istransfer为int值，便于确定是否换乘
*/
//#include"HeadFile.h"
#include <boost/graph/adjacency_list.hpp>
#include"metro.h"
#include<iostream>
#include<fstream>
using namespace std;

SearchSys::SearchSys(const string& city)
{
	sta_num = 0;
	sys_id = 0;
	line_num = 0;
	Init_gph(city);
	Same_Sta_weight = 0;
}
SearchSys::~SearchSys()
{

}
int SearchSys::Init_gph(const string& city)
{
	//transform(city.begin(), city.end(), city.begin(), ::toupper);
	fstream in;
	if (city == "BEIJING")//选择城市
	{
		cout << "openning..." << endl;
		in.open("beijing-subway.txt", ios::in);
	}
	else if (city.c_str() == "SHANGHAI")
	{
		in.open("shanghai-subway.txt", ios::in);
	}
	if (!in.is_open())
	{
		cout << "fail to open the file." << endl;
		return 0;
	}

	int sta_id, sta_x, sta_y, Line, line_sta_num;
	string sta_name, line_name;
	bool transfer;

	while (in >> input_type)
	{
		if (input_type == '#')//录入站点坐标、id、名称
		{
			in >> sta_id >> sta_name >> sta_x >> sta_y;
			sta_num++;
			sys_id = sta_num;//sysid同步
			Sta_nameToNum.insert(map<string, int>::value_type(sta_name, sta_num));
			Sta[sta_num].id = sta_id;//id = sta_num
			Sta[sta_num].name = sta_name;
			Sta[sta_num].position_x = sta_x;
			Sta[sta_num].position_y = sta_y;
			Sta[sta_num].istransfer = -1;//表明未确定
		}
		else if (input_type == '%')
		{
			line_num++;
			in >> line_name >> line_sta_num;
			Line_nameToNum.insert(map<string, int>::value_type(line_name, line_num));//建立从线名到线ID的映射
			for (int i = 0; i < line_sta_num; i++)//将每个line的sta放入sta_num
			{
				int temp_station_id;
				in >> temp_station_id;//每条线的sta的id
				if (Sta[temp_station_id].istransfer == -1)//未确定的时候id = sysid
				{
					Sta[temp_station_id].istransfer = 0;//false
					struct st temptrans;
					temptrans.line = line_num;
					temptrans.sysid = temp_station_id;
					Sta[temp_station_id].TransferID.push_back(temptrans);
				}
				else if (Sta[temp_station_id].istransfer == 0)//该站点是换乘站点
				{
					//Sta[temp_station_id].istransfer = 1;//是换乘站点
					//新建一个站点
					sys_id++;
					struct st temptrans;
					temptrans.line = line_num;
					temptrans.sysid = sys_id;
					Sta[temp_station_id].TransferID.push_back(temptrans);
					int full_num = Sta[temp_station_id].TransferID.size();
					for (int i = 0; i < full_num-1; i++)//给换乘站点之间加边
					{
						add_edge(Sta[temp_station_id].TransferID[i].sysid, Sta[temp_station_id].TransferID[full_num - 1].sysid, Same_Sta_weight, mtgph);
					}
				}
			}
		}
		else if (input_type == '@')//处理连线
		{
			int station_from, station_to;
			string line_name;
			in >> station_from >> station_to >> line_name;
			int same_sta_from_num = Sta[station_from].TransferID.size();
			int sta_from_sysid, sta_to_sysid;
			for (int i = 0; i < same_sta_from_num; i++)//查找from站点的sysid
			{
				if (Sta[station_from].TransferID[i].line == Line_nameToNum[line_name])
				{
					sta_from_sysid = Sta[station_from].TransferID[i].sysid;
					break;
				}
			}
			int same_sta_to_num = Sta[station_to].TransferID.size();
			for (int i = 0; i < same_sta_to_num; i++)//查找to站点的sysid
			{
				if (Sta[station_to].TransferID[i].line == Line_nameToNum[line_name])
				{
					sta_to_sysid = Sta[station_to].TransferID[i].sysid;
					break;
				}
			}
			add_edge(sta_from_sysid, sta_to_sysid, 1, mtgph);//普通站点权重为1
		}
	}
}

int main()
{
	string city;
	city.append("BEIJING");
	SearchSys SYS(city);

}