#include "fileManager.h"

//fileManager constructor
fileManager::fileManager(std::string settingsFN)
{
    setFN = settingsFN;
}
//Updates settings for program
int fileManager::updateSettings()
{
    std::ifstream fstream;
    fstream.open(setFN);

    const int lineCount = 7;

    std::string lines[lineCount];

    if (!fstream.is_open())
    {
        std::cout << "ERROR: Settings file could not open. Please resolve issue. Closing program." << std::endl;
        return -1;
    }

    for (int i = 0; i < lineCount; i++)
    {
        getline(fstream, lines[i]);
        if (lines[i] == "")
            continue;
        lines[i] = lines[i].substr(lines[i].find(":") + 2, lines[i].size());
    }

    pherMapFN = lines[0];
    physMapFN = lines[1];
    dataFN = lines[2];
    saveFN = lines[3];
    mapSizeX = stoi(lines[5]);
    mapSizeY = stoi(lines[6]);

    fstream.close();
    fstream.open(pherMapFN);
    if (!fstream.is_open())
    {
        std::cout << "ERROR: PherMap file could not open. Please resolve issue. Closing program" << std::endl;
        return -1;
    }
    fstream.close();
    fstream.open(physMapFN);
    if (!fstream.is_open())
    {
        std::cout << "ERROR: PhysMap file could not open. Please resolve issue. Closing program" << std::endl;
        return -1;
    }
    fstream.close();

    return 1;
}

std::string fileManager::getPherMap()
{
    return pherMapFN;
}

std::string fileManager::getPhysMap()
{
    return physMapFN;
}

std::string fileManager::getData()
{
    return dataFN;
}

std::string fileManager::getSave()
{
    return saveFN;
}

int fileManager::getMapSizeX()
{
    return mapSizeX;
}

int fileManager::getMapSizeY()
{
    return mapSizeY;
}