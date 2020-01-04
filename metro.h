#pragma once
//#include"HeadFile.h"
#include <boost/graph/adjacency_list.hpp>
#include<iostream>
using namespace std;
using namespace boost;
typedef property<edge_weight_t, int> EdgeWeightProperty;
typedef boost::adjacency_list<listS, vecS, directedS, no_property,EdgeWeightProperty >MetroGraph;
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
    int sta_num;//վ���id
    int sys_id;//վ���sysid
    vector<Station> station_list; //վ���б���1���
    vector<Sstation> graph_station_list; //ϵͳվ���б���1���
    int line_num;//��1��ʼ
    int Same_Sta_weight;//���˵�cost
public:
    SearchSys(const string& city);
    ~SearchSys();
    //int Init_gph(const string& city); //���ļ��м���ͼ��Ϣ
    int Init_gph(const string& city); //���ļ��м���ͼ��Ϣ
    int Store_gph(const string& city); //��ͼ��Ϣ���浽�ļ�
    Path Find_the_route(const string& start_station, const string& end_station, int transform, string& order);  //Ѱ������վ��֮������·
    Path Traversal(const string& now_station, string& order);  //ȫ����
    Path Print_line(const string&, string& order);  //��ӡĳ����·�ϵ�վ������
};