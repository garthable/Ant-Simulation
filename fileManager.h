#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "map.h"

#pragma once

class fileManager
{
    private:
        std::string setFN;

        std::string pherMapFN;
        std::string physMapFN;

        std::string dataFN;
        std::string saveFN;

        int mapSizeX;
        int mapSizeY;

    public:
        fileManager(std::string);
        int updateSettings();

        std::string getPherMap();
        std::string getPhysMap();
        std::string getData();
        std::string getSave();
        int getMapSizeX();
        int getMapSizeY();
};