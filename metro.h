#pragma once
//#include"HeadFile.h"
#include <boost/graph/adjacency_list.hpp>
#include<iostream>
using namespace std;
using namespace boost;
typedef property<edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_list<listS, vecS, directedS, no_property,EdgeWeightProperty >MetroGraph;
static map<string, int> Line_nameToNum;//站点名字到id的映射
static map<string, int> Sta_nameToNum;//线路名到id的映射
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
    //int line; //存放所属线路的编号
    vector<Sstation> TransferID;//从0编号
}Station;

typedef struct path
{
    int len; //经过的站点数量
    vector<int> stnid; //途经站点的id
} Path;

class SearchSys
{
private:
    MetroGraph mtgph; //站点线路图
    int sta_num;//站点的id
    int sys_id;//站点的sysid
    vector<Station> station_list; //站点列表，从1编号
    vector<Sstation> graph_station_list; //系统站点列表，从1编号
    int line_num;//从1开始
    int Same_Sta_weight;//换乘的cost
public:
    SearchSys(const string& city);
    ~SearchSys();
    //int Init_gph(const string& city); //从文件中加载图信息
    int Init_gph(const string& city); //从文件中加载图信息
    int Store_gph(const string& city); //将图信息保存到文件
    Path Find_the_route(const string& start_station, const string& end_station, int transform, string& order);  //寻找两个站点之间的最短路
    Path Traversal(const string& now_station, string& order);  //全遍历
    Path Print_line(const string&, string& order);  //打印某条线路上的站点序列
};