#include <boost/graph/metric_tsp_approx.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include "metro.h"

using namespace boost;

MetroGraph& SearchSys::get_all_pairs_shorest_graph(const MetroGraph& origin)
{
  MetroGraph& shortest_graph = *(new MetroGraph);
  property_map < MetroGraph, edge_weight_t >::type w = get(edge_weight, shortest_graph);
  VertexMap v_origin_map = get(vertex_index, origin);
  int v_size = num_vertices(origin);
  vector<vector<int>> D;

  johnson_all_pairs_shortest_paths(origin, D);

  for(int i = 0; i < v_size; i++)
    for(int j = 0; j < v_size; j++)
      if(D[i][j] != (std::numeric_limits<int>::max)())
        add_edge(v_origin_map[i], v_origin_map[j], D[i][j], shortest_graph);

  return shortest_graph;
}

Path SearchSys::Trave_metro(const string& src_station)
{
  int src = 0;
  map<string, int>::iterator l_it = Sta_nameToNum.find(src_station);
  if(l_it == Sta_nameToNum.end())
    throw "Don't find src_station!";
  else src = l_it->second;
  vector<Vertex> travel_trail;
  metric_tsp_approx_from_vertex(all_pairs_shorest_graph, src,
                                get(edge_weight, mtgph), get(vertex_index, mtgph),
                                tsp_tour_visitor<back_insert_iterator<vector<Vertex> > >
                                (back_inserter(travel_trail)));
  Path travel_path,tmp;
  int plen = 0;
  int last = -1;
  for (vector<Vertex>::iterator itr = travel_trail.begin(); itr != travel_trail.end(); ++itr)
    {
      /*
      if(graph_station_list[*itr].id != last)
        travel_path.stnid[plen++] = graph_station_list[*itr].id;
      */
      if(graph_station_list[*itr].id != last)
        {
          if(last != -1)
            {
              tmp = find_spath(last, graph_station_list[*itr].id);
              vector<int>::iterator itmp = tmp.stnid.begin();
              for(itmp++; itmp != tmp.stnid.end(); itmp++)
                travel_path.stnid[plen++] = *itmp;
            }
          else travel_path.stnid[plen++] = graph_station_list[*itr].id;
        }
      last = graph_station_list[*itr].id;
      //cout << *itr << " ";
    }
  travel_path.len = plen;
  return travel_path;
}
