#include "metro.h"

void SearchSys::save_path(Path& p, const string& filename) //打印单条路径
{
  fstream f(filename, ios::out);
  if(f.bad())
	{
      cout << "打开文件出错" << endl;
      throw "Can't open file";
	}
  for (vector<int>::iterator it = p.stnid.begin(); it != p.stnid.end(); ++it) //遍历变长数组
    f << station_list[*it].name << endl; //按数组内容索引站点名称
  f.close();
}

void SearchSys::save_path(Path *&p, const string& filename, int path_num) //输出多条路径
{
  fstream f(filename, ios::out);
  if(f.bad())
	{
      cout << "打开文件出错" << endl;
      throw "Can't open file";
	}
  for(int i = 0; i < path_num; i++)
    {
      f << '#' << i << endl;
      for (vector<int>::iterator it = p[i].stnid.begin(); it != p[i].stnid.end(); ++it)
        f << station_list[*it].name << endl; //按数组内容索引站点名称
      f << endl;
    }
  f.close();
}

void SearchSys::print_path(Path& p) //输出单条路径
{
  vector<int>::iterator it = p.stnid.begin();
  for (; it != p.stnid.end(); ++it)
    cout << station_list[*it].name << " -> "; //按数组内容索引站点名称
  cout << "end" << endl;
}
