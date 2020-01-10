#ifndef METRO_H
#define METRO_H
//#include"HeadFile.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include<iostream>
#include <iostream> // for std::cout
#include <utility> // for std::pair
#include <algorithm> // for std::for_each
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
using namespace std;
using namespace boost;
typedef property<edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_list<listS, vecS, directedS, no_property,EdgeWeightProperty >MetroGraph;
typedef MetroGraph::edge_descriptor Edge;
typedef graph_traits<MetroGraph>::vertex_descriptor Vertex;
typedef property_map<MetroGraph, vertex_index_t>::type VertexMap;
static map<string, int> Line_nameToNum;//վ�����ֵ�id��ӳ��
static map<string, int> Sta_nameToNum;//��·����id��ӳ��

typedef struct graph_station
{
  int sysid;
  int line; //λ����һ��·
  int id;
} Sstation;

typedef struct station
{
  int id;
  string name;
  bool istransfer; //�ǲ��ǻ���վ
  int position_x, position_y;
  //int line; //���������·�ı��
  vector<Sstation> TransferID;//��0���
}Station;

typedef struct path
{
  int len; //������վ������
  vector<int> stnid; //;��վ���id
} Path;

class SearchSys
{
private:
  MetroGraph mtgph; //վ����·ͼ
  MetroGraph all_pairs_shorest_graph;
  int sta_num;//վ���id
  int sys_id;//վ���sysid
  vector<Station> station_list; //վ���б���1���
  vector<Sstation> graph_station_list; //ϵͳվ���б���1���
  int line_num;//��1��ʼ
  int Same_Sta_weight;//���˵�cost
  MetroGraph& get_all_pairs_shorest_graph(const MetroGraph& origin);
  Path find_spath(Vertex src, Vertex trg);
    public:
  SearchSys(const string& city);
  ~SearchSys();
  //int Init_gph(const string& city); //���ļ��м���ͼ��Ϣ
  int Init_gph(const string& city); //���ļ��м���ͼ��Ϣ
  int Store_gph(const string& city); //��ͼ��Ϣ���浽�ļ�
  Path Find_the_shrt_path(const string& start_station, const string& end_station);  //Ѱ������վ��֮������·
  Path Trave_metro(const string& src_station);  //ȫ����
  Path Print_line(const string&, string& order);  //��ӡĳ����·�ϵ�վ������
};
#endif
