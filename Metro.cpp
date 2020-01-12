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
	Same_Sta_weight = 0;

	sta_num = 0;
	sys_id = 0;
	line_num = 0;
	Station temp;
	station_list.push_back(temp);
	Sstation temp1;
	graph_station_list.push_back(temp1);
	vector <int> temp2;
	Line_list.push_back(temp2);//使得下标和line的id对应上
	Init_gph(city);
	
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
  
	int sta_id, sta_x, sta_y, line_sta_num;
	string sta_name, line_name;
	char input_type;
	int line_num = 0, sys_id = 0, sta_num = 0;
	while (in >> input_type)
	{
		if (input_type == '#')//录入站点坐标、id、名称
		{
			in >> sta_id >> sta_name >> sta_x >> sta_y;
			sta_num++; 
			sys_id = sta_num;//sysid同步
			Sta_nameToNum.insert(map<string, int>::value_type(sta_name, sta_num));
			Station temp;
			station_list.push_back(temp);
			station_list[sta_num].id = sta_id;//id = sta_num
			station_list[sta_num].name = sta_name;
			station_list[sta_num].position_x = sta_x;
			station_list[sta_num].position_y = sta_y;
			station_list[sta_num].istransfer = false;//表明未确定
			Sstation temps;
			graph_station_list.push_back(temps);
		}
		else if (input_type == '%')//上一步完成后graph_station_list数目应该和station_list一样。
		{

			vector <int> this_line;//从下标0开始计数
			line_num++;
			in >> line_name >> line_sta_num;
			Line_nameToNum.insert(map<string, int>::value_type(line_name, line_num));//建立从线名到线ID的映射
			for (int i = 0; i < line_sta_num; i++)//将每个line的sta放入sta_num
			{
				int temp_station_id;
				in >> temp_station_id;//每条线的sta的id

				this_line.push_back(temp_station_id);
				if (station_list[temp_station_id].istransfer == false)//未确定的时候id = sysid
				{
					station_list[temp_station_id].istransfer = true;//false
					graph_station_list[temp_station_id].id = temp_station_id;
					graph_station_list[temp_station_id].line = Line_nameToNum[line_name];
					graph_station_list[temp_station_id].sysid = temp_station_id;
					Sstation temps = graph_station_list[temp_station_id];
					station_list[temp_station_id].TransferID.push_back(temps);//将graph引用
				}
				else if (station_list[temp_station_id].istransfer == true)//该站点是换乘站点
				{
					//Sta[temp_station_id].istransfer = 1;//是换乘站点
					//新建一个站点
					sys_id++;
					Sstation temp;
					temp.id = temp_station_id;
					temp.line = Line_nameToNum[line_name];
					temp.sysid = sys_id;
					graph_station_list.push_back(temp);
					Sstation temps = graph_station_list[sys_id];
					station_list[temp_station_id].TransferID.push_back(temps);//将graph引用

					int full_num = station_list[temp_station_id].TransferID.size();
					for (int i = 0; i < full_num-1; i++)//给换乘站点之间加边
					{
						add_edge(station_list[temp_station_id].TransferID[i].sysid, station_list[temp_station_id].TransferID[full_num - 1].sysid, Same_Sta_weight, mtgph);
					}
				}
			}
			Line_list.push_back(this_line);
		}
		else if (input_type == '@')//处理连线
		{
			int station_from, station_to;
			string line_name;
			in >> station_from >> station_to >> line_name;
			int same_sta_from_num = station_list[station_from].TransferID.size();
			int sta_from_sysid, sta_to_sysid;
			for (int i = 0; i < same_sta_from_num; i++)//查找from站点的sysid
			{
				if (station_list[station_from].TransferID[i].line == Line_nameToNum[line_name])
				{
					sta_from_sysid = station_list[station_from].TransferID[i].sysid;
					break;
				}
			}
			int same_sta_to_num = station_list[station_to].TransferID.size();
			for (int i = 0; i < same_sta_to_num; i++)//查找to站点的sysid
			{
				if (station_list[station_to].TransferID[i].line == Line_nameToNum[line_name])
				{
					sta_to_sysid = station_list[station_to].TransferID[i].sysid;
					break;
				}
			}
			add_edge(sta_from_sysid, sta_to_sysid, 1, mtgph);//普通站点权重为1
		}
	}
	all_pairs_shorest_graph = get_all_pairs_shorest_graph(mtgph);
}
Path SearchSys::Print_line(const string& station_name, string& order)
{
	if (Line_nameToNum.count(station_name) == 0)
		cout << "no this key" << endl;
	else
	{
		Path retur;
		int line_id = Line_nameToNum[station_name];
		retur.len = Line_list[line_id].size();
		retur.stnid = Line_list[line_id];
		return retur;
	}

}

int main()
{
	string city;
	city.append("BEIJING");
	SearchSys SYS(city);
	//SYS.test();
}