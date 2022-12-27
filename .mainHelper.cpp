#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
#include <random>

#include "fileManager.h"
#include "map.h"

void runSimulation(map& simMap)
{
    simMap.generateNodes();
    simMap.stitchNodes();
}

//Turns string into int
int spStoi(std::string input)
{
    if (!isdigit(input.at(0)))
        return -1;
    else
        return stoi(input.substr(0, 1));
}