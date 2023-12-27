#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "parser.h"

using namespace std;

vector<Configuration> IniParser(string nameFile) {
    ifstream iniFile;
    iniFile.open(nameFile);
    if (!iniFile.is_open()) {
        cout << "The file was not open!" << '\n';
        exit(1);
    }

    vector<Configuration> configs;
    string line;
    Configuration currentConfig; //текущая конфигурация
    while (getline(iniFile, line)) {
        if (line[0] == '[') {//пропускается так как секция ini-файла
            continue;
        } else if (line.substr(0, 2) == "id") {
            currentConfig.id = stoi(line.substr(5, line.size() - 5));
        } else if (line.substr(0, 6) == "parent") {
            currentConfig.parents.push_back(stoi(line.substr(9, line.size() - 9)));
        } else if (line.substr(0, 4) == "path") {
            currentConfig.path = line.substr(8, line.size() - 10);
        } else {
            configs.push_back(currentConfig);
            currentConfig.parents.clear();
        }
    }
    configs.push_back(currentConfig);
    
    iniFile.close();
    return configs;
}