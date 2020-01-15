#include "metro.h"

void SearchSys::save_path(Path& p, const string& filename)
{
  fstream f(filename, ios::out);
  if(f.bad())
	{
      cout << "打开文件出错" << endl;
      throw "Can't open file";
	}
  for (vector<int>::iterator it = p.stnid.begin(); it != p.stnid.end(); it++)
    f << station_list[*it].name << endl;
  f.close();
}

void SearchSys::save_path(Path *&p, const string& filename, int path_num)
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
      for (vector<int>::iterator it = p[i].stnid.begin(); it != p[i].stnid.end(); it++)
        f << station_list[*it].name << endl;
      f << endl;
    }
  f.close();
}

void SearchSys::print_path(Path& p)
{
  vector<int>::iterator it = p.stnid.begin();
  for (; it != p.stnid.end(); it++)
    cout << station_list[*it].name << " -> ";
  cout << "end" << endl;
}
