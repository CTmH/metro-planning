#include <boost/graph/metric_tsp_approx.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include "metro.h"

using namespace boost;

void SearchSys::get_all_pairs_shorest_graph()
{
  //MetroGraph& shortest_graph = *(new MetroGraph);
  //property_map < MetroGraph, edge_weight_t >::type w = get(edge_weight, shortest_graph);
  VertexMap v_origin_map = get(vertex_index, mtgph);
  int v_size = num_vertices(mtgph);
  vector<vector<int>> D(v_size, vector<int>(v_size, 0));

  johnson_all_pairs_shortest_paths(mtgph, D);

  for (int i = 0; i < v_size; i++)
    for (int j = 0; j < v_size; j++)
      if (D[i][j] != (std::numeric_limits<int>::max)() && i != j)
        {
          //cout << i << " -> " << j << " " << D[i][j] << endl;
          add_edge(v_origin_map[i], v_origin_map[j], D[i][j], all_pairs_shorest_graph);
        }
  return;
}

Path SearchSys::Trave_metro(const string& src_station)
{
  int src = 0;
  map<string, int>::iterator l_it = Sta_nameToNum.find(src_station);
  if (l_it == Sta_nameToNum.end())
    throw "Don't find src_station!";
  else src = station_list[l_it->second].TransferID[0].sysid;
  vector<Vertex> travel_trail;
  //cout << "src = " << src << endl;

  metric_tsp_approx_from_vertex(all_pairs_shorest_graph, src,
                                get(edge_weight, all_pairs_shorest_graph), get(vertex_index, all_pairs_shorest_graph),
                                tsp_tour_visitor<back_insert_iterator<vector<Vertex> > >
                                (back_inserter(travel_trail)));
  /*
    metric_tsp_approx_from_vertex(mtgph, src,
    get(edge_weight, mtgph), get(vertex_index, mtgph),
    tsp_tour_visitor<back_insert_iterator<vector<Vertex> > >
    (back_inserter(travel_trail)));
  */
  Path travel_path, tmp;

  int last = -1, tmp_last = -1;
  for (vector<Vertex>::iterator itr = travel_trail.begin(); itr != travel_trail.end(); ++itr)
    {
      //cout << station_list[graph_station_list[*itr].id].name << endl;
      if (graph_station_list[*itr].id != last)
        {

          if (last != -1)
            {
              //cout << last << " - " << *itr << endl;
              tmp = find_spath(graph_station_list[last].id, graph_station_list[*itr].id);
              /*
                for(int i = 0; i < tmp.len; i++)
                travel_path.stnid.push_back(tmp.stnid[i]);
              */
              vector<int>::iterator itmp = tmp.stnid.begin();
              for (; itmp != tmp.stnid.end(); ++itmp)
                {
                  if(graph_station_list[*itmp].id != tmp_last)
                    travel_path.stnid.push_back(*itmp);
                  tmp_last = graph_station_list[*itmp].id;
                }
              //print_path(tmp);
              //travel_path.stnid[plen++] = *itmp;
            }
          else travel_path.stnid.push_back(graph_station_list[*itr].id);
          //else travel_path.stnid[plen++] = graph_station_list[*itr].id;
        }
      last = graph_station_list[*itr].id;
      //cout << *itr << " ";
    }
  //travel_path.len = plen;
  travel_path.len = travel_path.stnid.size();
  return travel_path;
}
