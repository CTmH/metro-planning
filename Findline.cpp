#include "metro.h"

Path SearchSys::Find_line_with_name(const string& linename) //根据线路名获得线路站点
{
  int line_id = 0;
  map<string, int>::iterator l_it;
  l_it = Line_nameToNum.find(linename);
  //处理站名
  if (l_it == Line_nameToNum.end())
    throw "Don't find this line!";
  else line_id = l_it->second; //获得相应的标号

  Path line;
  line.stnid = Line_list[line_id]; //以路径结构输出
  line.len = Line_list[line_id].size();
  return line;
}

int SearchSys::Find_line(const string& station_name, Path* &find_line_list) //根据站点名称获得相关线路的站点信息
{
  int st = 0, line_num = 0;
  map<string, int>::iterator l_it;
  l_it = Sta_nameToNum.find(station_name);
  if (l_it == Sta_nameToNum.end())
    throw "Don't find station!";
  else st = l_it->second; //获得相应的标号

  line_num = station_list[st].TransferID.size();
  find_line_list = new Path[line_num];
  vector<Sstation>::iterator itr = station_list[st].TransferID.begin();
  for(int line_id, i = 0; itr != station_list[st].TransferID.end(); ++itr, ++i) //以路径结构输出 可能有多条相关线路
    {
      line_id = itr->line;
      find_line_list[i].stnid = Line_list[line_id];
      find_line_list[i].len = Line_list[line_id].size();
    }
  return line_num;
}
