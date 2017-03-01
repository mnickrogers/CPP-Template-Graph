//
//  main.cpp
//  template_graph
//
//  Created by Nicholas Rogers on 5/30/16.
//  Copyright © 2016 Nicholas Rogers. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "graph.h"

int word_deviation(const std::string & w1, const std::string & w2);

int main(int argc, const char * argv[])
{
    graph<std::string> g;
    
    std::vector<std::string> wordVec;
    std::fstream is("/Users/nicholasrogers/Documents/five_letter_dictionary.txt");
    if (is.fail())
    {
        std::cout << "Failed to load file" << std::endl;
        exit(1);
    }
    
    while (!is.eof())
    {
        std::string s;
        is >> s;
        if (!s.empty())
        {
            wordVec.push_back(s);
        }
    }
    is.close();
    
    for (auto e1: wordVec)
        for (auto e2: wordVec)
            if (word_deviation(e1, e2) == 1 && e1 != e2)
                g.add_edge(e1, e2);
    
    auto list = g.shortest_path("amigo", "signs");
    for (auto e: list)
        std::cout << e << std::endl;
    
    return 0;
}

int word_deviation(const std::string & w1, const std::string & w2)
{
    int deviation(0);
    
    for (std::size_t i = 0; i < w1.size(); i++)
        if (w1[i] != w2[i])
            deviation++;
    
    return deviation;
}
