#include <string>
#include <stdlib>
#include <iostream> // for std::cout
#include <utility> // for std::pair
#include <algorithm> // for std::for_each
#include <boost/graph>
using namespace std;
using namespace boost;

typedef property<edge_weight_t, int> EdgeWeightProperty;
typedef adjacency_list<listS, vecS, undirectedS, no_property, EdgeWeightProperty> MetroGraph;

typedef struct graph_station
{
  int sysid;
  int line; //位于哪一线路
  int id;
} Sstation;

typedef struct station
{
  int id;
  string name;
  bool istransfer; //是不是换乘站
  int position_x, position_y;
  vector<int> graph_st_list;
} Station;

typedef struct path
{
  int len; //经过的站点数量
  vector<int> stnid; //途经站点的id
} Path;

class SearchSys
{
 private:
  MetroGraph mtgph; //站点线路图
  vector<Station> station_list; //站点列表
  vector<Sstation> graph_station_list; //系统站点列表
  int get_st_id(const string& city);

 public:
  SearchSys();
  ~SearchSys();
  int Init_gph(const string& city); //从文件中加载图信息
  int Store_gph(const string& city); //将图信息保存到文件
  Path Find_the_route(const string& start_station, const string& end_station, int transform, string& order);  //寻找两个站点之间的最短路
  Path Traversal(const string& now_station, string& order);  //全遍历
  Path Print_line(const string&, string& order);  //打印某条线路上的站点序列
};

int SearchSys::get_st_id(const string& city)
{
  for(int i = 0; i < len(station_list); i++)
    {
      if(city == station_list[i].name)
        return i;
    }
  return -1;
}
