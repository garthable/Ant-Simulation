#include ".mainHelper.cpp"

//Runs simulation
//Usage:
//./program_name
//If you would like to set a new directory for settings use
//./program_name settingsFileName
    //argv[0] = program name
    //argv[1] = settings
int main(int argc, char *argv[])
{
    std::string settingsFile = "~settings.txt";
    std::string line;
    int input;

    if (argc > 1)
        settingsFile = argv[1];

    fileManager fM(settingsFile);

    if (fM.updateSettings() == -1)
        return -1;
    
    map simMap(fM.getPherMap(), fM.getPhysMap(), fM.getMapSizeX(), fM.getMapSizeY());

    //UI
    while (true)
    {
        std::cout << "====== MAIN MENU ======" << std::endl;
        std::cout << "1. Run Simulation" << std::endl;
        std::cout << "2. Nuke Maps" << std::endl;
        std::cout << "3. Get node data" << std::endl;
        std::cout << "4. Quit" << std::endl;

        getline(std::cin, line);
        input = spStoi(line);

        switch (input)
        {
            case -1: //Char in input
                std::cout << "ERROR: Input provided contained a character! Please only enter integers." << std::endl;
                break;
            case 1: //Run simulation
                runSimulation(simMap);
                break;
            case 2: //Nuke maps
                simMap.nukeMap();
                break;
            case 3: //Display node
                int x;
                int y;
                getline(std::cin, line);
                x = stoi(line);
                getline(std::cin, line);
                y = stoi(line);
                simMap.displayNodeData(x, y);
                break;
            case 4: //Exits program
                std::cout << "Goodbye!" << std::endl;
                return 1;
                break;
            default:
                std::cout << "ERROR: Unknown integer! Please only enter integers displayed in menu." << std::endl;
                break;
        }
    }
    return 0;
}