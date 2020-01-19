#include <boost/graph/metric_tsp_approx.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include "metro.h"

using namespace boost;

void SearchSys::get_all_pairs_shorest_graph()
{
  VertexMap v_origin_map = get(vertex_index, mtgph); //地铁线路图的点集
  int v_size = num_vertices(mtgph);
  vector<vector<int>> D(v_size, vector<int>(v_size, 0)); //线路图的最小代价矩阵

  johnson_all_pairs_shortest_paths(mtgph, D); //计算线路图的最小代价矩阵

  for (int i = 0; i < v_size; i++)
    for (int j = 0; j < v_size; j++)
      if (D[i][j] != (std::numeric_limits<int>::max)() && i != j)
        {
          //转换为最短通路图
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
  else src = station_list[l_it->second].TransferID[0].sysid; //获得站点在站点图中的标号
  vector<Vertex> travel_trail; //路径
  
  //获得在最短通路图上的TSP路径
  metric_tsp_approx_from_vertex(all_pairs_shorest_graph, src,
                                get(edge_weight, all_pairs_shorest_graph), get(vertex_index, all_pairs_shorest_graph),
                                tsp_tour_visitor<back_insert_iterator<vector<Vertex> > >
                                (back_inserter(travel_trail)));
  Path travel_path, tmp;

  int last = -1, tmp_last = -1;
  for (vector<Vertex>::iterator itr = travel_trail.begin(); itr != travel_trail.end(); ++itr)
    {
      if (graph_station_list[*itr].id != last)
        {

          if (last != -1)
            {
              //获得两个TSP路径点之间的实际路径
              tmp = find_spath(graph_station_list[last].id, graph_station_list[*itr].id);
              
              vector<int>::iterator itmp = tmp.stnid.begin();
              //拼接路径
              for (; itmp != tmp.stnid.end(); ++itmp)
                {
                  if(graph_station_list[*itmp].id != tmp_last)
                    travel_path.stnid.push_back(*itmp);
                  tmp_last = graph_station_list[*itmp].id;
                }
            }
          else travel_path.stnid.push_back(graph_station_list[*itr].id);
        }
      last = graph_station_list[*itr].id;
    }

  travel_path.len = travel_path.stnid.size();
  return travel_path;
}
