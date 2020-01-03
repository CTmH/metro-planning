#include "metro.h"

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

typedef graph_traits<MetroGraph>::vertex_descriptor Vertex;
typedef std::pair<int,int> E;
typedef property_map<MetroGraph, vertex_index_t>::type IndexMap;

Path SearchSys :: Find_the_shrt_path(const string& start_station, const string& end_station, int transform, string& order)  //寻找两个站点之间的最短路
{
  int src = get_st_id(start_station);
  int trg = get_st_id(end_station);
  IndexMap index = get(vertex_index, mtgph);
  // vector for storing distance property
  vector<int> d(num_vertices(mtgph));
  // get the first vertex
  Vertex s = station_list[src].graph_st_list[0];
  // invoke variant 2 of Dijkstra's algorithm
  //dijkstra_shortest_paths(mtgph, s, distance_map(&d[0]));

  vector<Vertex> p(num_vertices(mtgph), graph_traits<MetroGraph>::null_vertex()); //the predecessor 数组
  dijkstra_shortest_paths(mtgph, s, distance_map(&d[0]).visitor(make_predecessor_recorder(&p[0])));

  graph_traits<MetroGraph>::vertex_iterator vi;
  vector<int> shrt;
  Path shrt_path;
  for(Vertex st = trg, int i = 0; p[st] != graph_traits<Graph>::null_vertex(); st = p[st], i++)
    shrt[i] = st;

  for(int rlen = 0, int last = -1, i--; i >= 0; i--)
    {
      if(graph_station[shrt[i]].id != last)
        shrt_path.stnid[rlen++] = graph_station[shrt[i]].id;
      last = graph_station[shrt[i]].id;
    }
  shrt_path.len = rlen;
  return shrt_path;
}
