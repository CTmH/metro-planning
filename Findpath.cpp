#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "metro.h"
using namespace boost;

template <class PredecessorMap>
class record_predecessors : public dijkstra_visitor<>
{
public:
  record_predecessors(PredecessorMap p)
    : m_predecessor(p) { }

  template <class Edge, class Graph>
  void edge_relaxed(Edge e, Graph& g) {
    // set the parent of the target(e) to source(e)
    put(m_predecessor, target(e, g), source(e, g));
  }
protected:
  PredecessorMap m_predecessor;
};

template <class PredecessorMap> ::record_predecessors<PredecessorMap>
make_predecessor_recorder(PredecessorMap p)
{
  return ::record_predecessors<PredecessorMap>(p);
}

//typedef graph_traits<MetroGraph>::vertex_descriptor Vertex;
typedef std::pair<int, int> E;
typedef property_map<MetroGraph, vertex_index_t>::type IndexMap;

Path SearchSys::Find_the_shrt_path(const string& start_station, const string& end_station)  //寻找两个站点之间的最短路
{
  int src = 0, trg = 0;
  map<string, int>::iterator l_it;
  l_it = Sta_nameToNum.find(start_station);
  if (l_it == Sta_nameToNum.end())
    throw "Don't find start_station!";
  else src = l_it->second;

  l_it = Sta_nameToNum.find(end_station);
  if (l_it == Sta_nameToNum.end())
    throw "Don't find end_station!";
  else trg = l_it->second;
  Path shrt_path = find_spath(src, trg);
  shrt_path.stnid.insert(shrt_path.stnid.begin(),src);
  return shrt_path;
}

Path SearchSys::find_spath(Vertex src, Vertex trg)
{
  IndexMap index = get(vertex_index, mtgph);
  // vector for storing distance property
  vector<Vertex> d(num_vertices(mtgph));
  // get the first vertex
  Vertex s = station_list[src].TransferID[0].sysid;
  // invoke variant 2 of Dijkstra's algorithm
  //dijkstra_shortest_paths(mtgph, s, distance_map(&d[0]));

  vector<Vertex> p(num_vertices(mtgph), graph_traits<MetroGraph>::null_vertex()); //the predecessor 数组
  dijkstra_shortest_paths(mtgph, s, distance_map(&d[0]).visitor(make_predecessor_recorder(&p[0])));

  vector<Vertex> shrt;
  Path shrt_path;
  Vertex st = station_list[trg].TransferID[0].sysid;
  int i;
  for (i = 0; p[st] != graph_traits<MetroGraph>::null_vertex(); st = p[st])
    shrt.push_back(st);
  int rlen = 0;
  int last = -1;
  //for(vector<Vertex>::iterator itr = shrt.begin(); itr != shrt.end(); itr++)
  //cout << station_list[graph_station_list[*itr].id].name << " ->";
  //cout << endl;
  for(vector<Vertex>::reverse_iterator itr = shrt.rbegin(); itr != shrt.rend(); itr++)
    {
      if (graph_station_list[*itr].id != last)
        shrt_path.stnid.push_back(graph_station_list[*itr].id);
      last = graph_station_list[*itr].id;
    }
  shrt_path.len = shrt_path.stnid.size();
  return shrt_path;
}
