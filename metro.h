#include <string>
#include<stdlib.h>
#include <boost/graph/adjacency_list.hpp>
#include<iostream>

using namespace std;
using namespace boost;
typedef boost::adjacency_list<listS, vecS, undirectedS> MetroGraph;

class SearchSys
{
 private:
  MetroGraph mtgph;//记录地铁线路的邻接表
  int sta_num;
  char input_type;
  struct station Sta[1000];//由站点组成的集合,从1开始编号
  int line_num;//从1开始
 public:
  SearchSys();
  ~SearchSys();
  //int Init_gph(const string& city); //从文件中加载图信息
  int Init_gph(); //从文件中加载图信息
  int Store_gph(const string& city); //将图信息保存到文件
  int Find_the_route(const string& start_station, const string& end_station, int transform, string& order);  //寻找两个站点之间的最短路
  int Traversal(const string& now_station, string& order);  //全遍历
  int Print_line(const string&, string& order);  //打印某条线路上的站点序列
};

struct station
{
    int id;
    string name;
    int istransfer; //是不是换乘站
    int position_x, position_y;
    int line; //存放所属线路的编号
};

