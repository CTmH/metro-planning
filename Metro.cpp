#include <boost/graph/adjacency_list.hpp>
#include"metro.h"
using namespace std;
map<string, int>  SearchSys::Line_nameToNum;
map<string, int>  SearchSys::Sta_nameToNum;
SearchSys::SearchSys(const string& city, int trans_cost = 0)
{
  Same_Sta_weight = trans_cost;

  sta_num = 0;
  sys_id = 0;
  line_num = 0;

  Init_gph(city);

  dijkstra_tree_list.insert(dijkstra_tree_list.begin(), graph_station_list.size(), NULL);
}
SearchSys::~SearchSys()
{
  for (vector<vector<Vertex>*>::iterator itr = dijkstra_tree_list.begin(); itr != dijkstra_tree_list.end(); ++itr)
    delete *itr;
}
int SearchSys::Init_gph(const string& city)
{
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

  if (!in.is_open() || in.bad()) //未能打开文件
	{
      cout << "fail to open the file." << endl;
      exit(0);
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
          sys_id = sta_num;//sysid同步
          Sta_nameToNum.insert(map<string, int>::value_type(sta_name, sta_num));
          Station temp;
          temp.id = 0;
          temp.istransfer = false;
          temp.position_y = 0;
          temp.position_x = 0;
          station_list.push_back(temp);
          station_list[sta_num].id = sta_id - 1;//id = sta_num
          station_list[sta_num].name = sta_name;
          station_list[sta_num].position_x = sta_x;
          station_list[sta_num].position_y = sta_y;
          station_list[sta_num].istransfer = false;//表明未确定
          Sstation temps;
          temps.id = 0;
          temps.sysid = 0;
          temps.line = 0;
          graph_station_list.push_back(temps);
          sta_num++;
		}
      else if (input_type == '%')//上一步完成后graph_station_list数目应该和station_list一样。
		{

          vector <int> this_line;//从下标0开始计数

          in >> line_name >> line_sta_num;
          Line_nameToNum.insert(map<string, int>::value_type(line_name, line_num));//建立从线名到线ID的映射
          line_num++;
          for (int j = 0; j < line_sta_num; j++)//将每个line的sta放入sta_num
			{
              int temp_station_id;
              in >> temp_station_id;//每条线的sta的id
              temp_station_id -= 1;
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
                  for (int i = 0; i < full_num - 1; i++)//给换乘站点之间加边
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
          in >> station_from >> station_to >> line_name;
          station_from -= 1; station_to -= 1;
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
  get_all_pairs_shorest_graph();
  return 0;
}
