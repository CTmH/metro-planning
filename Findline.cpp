#include "metro.h"

Path SearchSys::Find_line_with_name(const string& linename)
{
  int line_id = 0;
  map<string, int>::iterator l_it;
  l_it = Line_nameToNum.find(linename);
  if (l_it == Line_nameToNum.end())
    throw "Don't find this line!";
  else line_id = l_it->second;

  Path line;
  line.stnid = Line_list[line_id];
  line.len = Line_list[line_id].size();
  return line;
}

int SearchSys::Find_line(const string& station_name, Path* &find_line_list)
{
  int st = 0, line_id = 0, line_num = 0;
  map<string, int>::iterator l_it;
  l_it = Sta_nameToNum.find(station_name);
  if (l_it == Sta_nameToNum.end())
    throw "Don't find station!";
  else st = l_it->second;

  line_num = station_list[st].TransferID.size();
  find_line_list = new Path[line_num];
  vector<Sstation>::iterator itr = station_list[st].TransferID.begin();
  for(int i = 0; itr != station_list[st].TransferID.end(); itr++, i++)
    {
      line_id = itr->line;
      find_line_list[i].stnid = Line_list[line_id];
      find_line_list[i].len = Line_list[line_id].size();
    }
  return line_num;
}
