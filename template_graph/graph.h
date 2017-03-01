//
//  graph.h
//  template_graph
//
//  Created by Nicholas Rogers on 5/30/16.
//  Copyright © 2016 Nicholas Rogers. All rights reserved.
//

#ifndef graph_h
#define graph_h

#include <cstdlib>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <queue>
#include <algorithm>
#include <iostream>
#include <sstream>

template <class T>
class graph
{
public:
    
    typedef std::size_t size_type;
    
    // pre: none
    // post: creates an empty graph
    graph()
    {
        _v = std::vector<std::list<size_type>>();
        _data = std::unordered_map<T, size_type>();
        _data_id = std::unordered_map<size_type, T>();
        _empty_spaces = std::queue<size_type>();
        _empty_spaces.push(0);
    }
    
    // pre: none
    // post: returns this graph's memory to the heap
//    ~graph()
//    {
//    }
    
    // pre: none
    // post: returns the number of vertices in this graph
    std::size_t n() const
    {
        return _v.size();
    }
    
    // pre: none
    // post: returns the number of edges in this graph
    std::size_t m() const
    {
        std::size_t ans(0);
        for (auto e: _v)
            ans += e.size();
        
        return ans/2;
    }
    
    // pre: none
    // post: if a is not a vertex in this graph, add a as a vertex, else do nothing
    void add_vertex(const T & a = T())
    {
        if (_data.find(a) != _data.end())
            return;
        
        size_type spot = _empty_spaces.front();
        _empty_spaces.pop();
        
        auto list = std::list<size_type>();
        if (spot >= _v.size())
            _v.push_back(list);
        else
            _v[spot] = list;
        
        if (_empty_spaces.empty()) // When we delete vertices, we will push their previous locations onto the queue
            _empty_spaces.push(_v.size());
        
        _data[a] = spot;
        _data_id[spot] = a;
    }
    
    // pre: if {a, b} are vertices in this graph, create edge between them; if they are not, then add {a, b} to this graph and create edge between {a, b}.
    // post: adds {a, b} as edges in this graph if they are not in this graph, else do nothing
    void add_edge(const T & a = T(), const T & b = T())
    {
        if (_data.find(a) == _data.end())
            add_vertex(a);
        if (_data.find(b) == _data.end())
            add_vertex(b);
        
        size_type aIndex = _data[a];
        size_type bIndex = _data[b];
        
        _v[aIndex].push_front(bIndex);
        _v[bIndex].push_front(aIndex);
    }
    
    // pre: {a, b} are existing vertices
    // post: removes edge {a, b} from this graph if they exist, otherwise do nothing
    void remove_edge(const T & a, const T & b)
    {
        assert(_data->find(a) != _data->end() && _data->find(b) != _data->end());
        if (is_edge(a, b))
        {
            size_type aIndex = _data[a];
            size_type bIndex = _data[b];
            _data[aIndex].remove(bIndex);
            _data[bIndex].remove(aIndex);
        }
    }
    
    // pre: none
    // post: returns true iff {a, b} are edges in this graph
    bool is_edge(const T & a, const T & b) const
    {
        if (_data->find(a) == _data->end() || _data->find(b) == _data->end())
            return false;
        
        size_type aIndex = _data[a];
        size_type bIndex = _data[b];
        
        for (auto e: _v[aIndex])
            if (e == bIndex)
                return true;
        
        return false;
    }
    
    // pre: a is a vertex in this graph
    // post: returns all values for vertices connected to vertex a where the path contains only one edge
    std::unordered_set<T> neighbor_values(const T & a)
    {
        assert(_data.find(a) != _data.end());
        size_type aIndex = _data[a];
        std::unordered_set<T> set;
        
        for (auto e: _v[aIndex])
            set.insert(_data_id[e]);
        
        return set;
    }
    
    // pre: source and destination are both vertices in this graph
    // post: returns a path of items T from the source to destination if one exists, else return an empty vector
    std::vector<T> shortest_path(const T & src, const T & dst)
    {
        std::queue<size_type> q;
        std::vector<bool> visited(n(), false);
        std::unordered_map<size_type, size_type> parents;
        std::vector<T> path;
        
        assert(_data.find(src) != _data.end() && _data.find(dst) != _data.end());
        
        size_type srcIndex = _data[src];
        size_type dstIndex = _data[dst];
        
        visited[srcIndex] = true;
        q.push(srcIndex);
        
        while (!q.empty())
        {
            size_type f = q.front();
            q.pop();
            
            for (auto e: _v[f])
            {
                if (!visited[e])
                {
                    visited[e] = true;
                    q.push(e);
                    parents[e] = f;
                }
            }
        }
        
        size_type parent(dstIndex);
        
        while (parent != srcIndex)
        {
            if (parent != dstIndex)
                path.push_back(_data_id[parent]);
            parent = parents[parent];
        }
        
        std::reverse(path.begin(), path.end());
        
        return path;
    }
    
    // pre: none
    // post: returns an output stream containing this graph
    void write(std::ostream & os)
    {
        os << n() << " " << m() << std::endl;
        for (size_type i = 0; i < _v.size(); i++)
        {
            std::list<size_type> n = _v[i];
            for (auto e: n)
                if (e > i)
                    os << i << " " << _data_id[i] << " " << e << std::endl;
        }
    }
    
    // pre: none
    // post: replaces this graph with data from a graph file
    void read(std::istream & is)
    {
        _v.clear();
        _data.clear();
        _data_id.clear();
        _empty_spaces = std::queue<size_type>();
        _empty_spaces.push(0);
        
        size_type n, m;
        T type;
        
        is >> n >> m;
        
        _v = std::vector<std::list<size_type>>(n);
        
        std::string line;
        while (std::getline(std::cin, line))
        {
            std::istringstream ss(line);
            size_type index;
            T item;
            
            
        }
    }
    
private:
    
    std::vector<std::list<size_type>>       _v;
    std::unordered_map<T, size_type>        _data;
    std::unordered_map<size_type, T>        _data_id; // Favor speed over memory efficiency
    std::queue<size_type>                   _empty_spaces;
};

template <class T>
std::ostream & operator <<(std::ostream & os, graph<T> & g)
{
    g.write(os);
    return os;
}

template <class T>
std::istream & operator >>(std::istream & is, graph<T> & g)
{
    g.read(is);
    return is;
}

#endif /* graph_h */
