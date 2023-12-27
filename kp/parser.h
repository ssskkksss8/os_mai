#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <vector>
#include <iostream>

class Configuration {
    public:
        int id;
        std::vector<int> parents; //вектор родительских идентификаторов
        std::string path;
        
        void printConfiguration() {
            for (int i = 0; i < parents.size(); ++i)
                std::cout << id << '\n' << parents[i] << '\n' << path <<"\n\n";
        }
};

std::vector<Configuration> IniParser(std::string);
#endif