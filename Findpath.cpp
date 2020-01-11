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

    Path SearchSys::Find_the_shrt_path(const string& start_station, const string& end_station)  //Ѱ������վ��֮������·
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

        return find_spath(src, trg);
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

        vector<Vertex> p(num_vertices(mtgph), graph_traits<MetroGraph>::null_vertex()); //the predecessor ����
        dijkstra_shortest_paths(mtgph, s, distance_map(&d[0]).visitor(make_predecessor_recorder(&p[0])));

        graph_traits<MetroGraph>::vertex_iterator vi;
        vector<Vertex> shrt;
        Path shrt_path;
        Vertex st = station_list[trg].TransferID[0].sysid;
        int i;
        for (i = 0; p[st] != graph_traits<MetroGraph>::null_vertex(); st = p[st])
            shrt[i++] = st;
        int rlen = 0;
        int last = -1;
        for (i = i - 1; i >= 0; i--)
        {
            if (graph_station_list[shrt[i]].id != last)
                shrt_path.stnid[rlen++] = graph_station_list[shrt[i]].id;
            last = graph_station_list[shrt[i]].id;
        }
        shrt_path.len = rlen;
        return shrt_path;
    }