#include "map.h"

map::map(std::string newPherFN, std::string newPhysFN, int newXSize, int newYSize)
{
    pherFN = newPherFN;
    physFN = newPhysFN;
    xSize = newXSize;
    ySize = newYSize;
}

map::~map()
{
    for (int i = 0; i < points.size(); i++)
        delete points[i];
    std::cout << "Deleted map!" << std::endl;
}

void map::nukeMap()
{
    std::ofstream fout_pher;
    std::ofstream fout_phys;

    fout_pher.open(pherFN);
    fout_phys.open(physFN);

    for (int y = 0; y < ySize; y++)
    {
        for (int x = 0; x < xSize; x++)
        {
            fout_pher << "0";
            fout_phys << "-";
        }
        fout_pher << std::endl;
        fout_phys << std::endl;
    }

    for (int i = 0; i < points.size(); i++)
        delete points[i];
}

void map::generateNodes()
{
    std::string linePher;
    std::string linePhys;
    std::fstream pher;
    std::fstream phys;

    pheromone *pher_ptr = nullptr;
    int nodeState;
    int foodAmount = 0;

    node *node_ptr;

    char pherC;
    char physC;

    pher.open(pherFN);
    phys.open(physFN);

    for (int y = 0; y < ySize; y++)
    {
        getline(pher, linePher);
        getline(phys, linePhys);
        for (int x = 0; x < xSize; x++)
        {
            pherC = linePher.at(x);
            physC = linePhys.at(x);

            if (pherC = '0') //Empty
                pher_ptr = new pheromone(0, 0);
            else if (pherC == '-') //Contains home pheromone
                pher_ptr = new pheromone(1, 9);
            else if (pherC == '+') //Contains food pheromone
                pher_ptr = new pheromone(2, 9);

            if (physC == '-') //Empty
                nodeState = 0;
            else if (physC == '#') //Contains wall
                nodeState = 1;
            else if (physC == 'a') //Contains ant
                nodeState = 2;
            else if (isdigit(physC)) //Contains food
            {
                nodeState = 3;
                foodAmount = int(physC);
            }

            node_ptr = new node(x, y, nodeState, foodAmount, *pher_ptr);
            points.push_back(node_ptr);
            delete pher_ptr;    
            foodAmount = 0;
        }
    }
    for (int i = 0; i < points.size(); i++)
    {
        if (points[i] == nullptr)
        {
            std::cout << "generateNodes ERROR: Node not generated!" << std::endl;
            break;
        }
        else
            std::cout << "Point: " << points[i]->x << " " << points[i]->y << " Exists at " << i << " in points vector" << std::endl;
    }
    pher.close();
    phys.close();
}

node* map::findNode(int xLoc, int yLoc)
{
    for (int i = 0; i < points.size(); i++)
        if (points[i]->x == xLoc && points[i]->y == yLoc)
        {
            return points[i];
        }
        else
            continue;
    return nullptr;
}

void connect(node* currNode, node* adjNode, int distance)
{
    if (currNode != nullptr && !(currNode->state == 0 || currNode->state == 2 || currNode->state == 3))
        return;
    else if (adjNode != nullptr && (adjNode->state == 0 || adjNode->state == 2 || adjNode->state == 3))
    {
        adj newAdj;
        newAdj.adjNode = adjNode;
        newAdj.distance = distance;
        currNode->adjNodes.push_back(newAdj);
        // std::cout << "Connected point " << currNode->x << " " << currNode->y << " with point " << adjNode->x << " " << adjNode->y << std::endl;
    }
}

void map::stitchNodes()
{
    int xOffsets[8] = {1, 1, 1, 0, 0, -1, -1, -1};
    int yOffsets[8] = {1, -1, 0, 1, -1, 0, 1, -1};
    int distances[8] = {14, 14, 10, 10, 10, 10, 14, 14};

    node* currNode;
    node* adjNode;
    for (int i = 0; i < points.size(); i++)
    {
        currNode = points[i];
        for (int g = 0; g < 8; g++)
        {
            adjNode = findNode(currNode->x + xOffsets[g], currNode->y + yOffsets[g]);
            connect(currNode, adjNode, distances[g]);
        }
    }
}

void map::reducePheromone()
{

}

char map::getASCII(int xFind, int yFind)
{
    node *nodePtr = nullptr;
    nodePtr = findNode(xFind, yFind);
    if (nodePtr == nullptr)
    {
        std::cout << "getASCII ERROR: Could not find node!" << std::endl;
        return 'E';
    }
    int state = nodePtr->state;
    switch (state)
    {
        case 0:
            return '-';
            break;
        case 1:
            return '#';
            break;
        case 2:
            return 'a';
            break;
        case 3:
            return nodePtr->foodAmount;
            break;
        default:
            return 'E';
            break;
    }
    return 'E';
}

void map::updatePher()
{
    std::fstream file;
    std::string line;
    std::string newLine;
    char insertChar;
    file.open(pherFN);
    while (!changedPher.empty())
    {
        //Resets
        file.clear();
        file.seekg(0);
        newLine = "";

        switch(changedPher.top()->pher.type)
        {
            case 0:
                insertChar = '0';
            case 1:
                insertChar = '-';
            case 2:
                insertChar = '+';
        }

        //Gets line located at y
        file.seekg(changedPher.top()->y);
        getline(file, line);

        //Restructures line at y
        newLine.append(line.substr(0, changedPher.top()->x-1));
        newLine = newLine + insertChar;
        newLine.append(line.substr(changedPher.top()->x+1, line.length()));

        //Sets newLine to line
        file << newLine;
        changedPher.pop();
    }
}
void map::updatePhys()
{
    std::fstream file;
    std::string line;
    std::string newLine;
    file.open(physFN);
    while (!changedPhys.empty())
    {
        //Resets
        file.clear();
        file.seekg(0);
        newLine = "";

        //Gets line located at y
        file.seekg(changedPhys.top()->y);
        getline(file, line);

        //Restructures line at y
        newLine.append(line.substr(0, changedPhys.top()->x-1));
        newLine = newLine + getASCII(changedPhys.top()->x, changedPhys.top()->y);
        newLine.append(line.substr(changedPhys.top()->x+1, line.length()));

        //Sets newLine to line
        file << newLine;
        changedPhys.pop();
    }
}

void map::update()
{
    updatePher();
    updatePhys();
}

void map::displayNodeData(int xFind, int yFind)
{
    node* tarNode = findNode(xFind, yFind);
    if (tarNode == nullptr)
    {
        std::cout << "displayNodeData ERROR: Could not find node at " << xFind << " " << yFind << std::endl;
        return;
    }
    int adjX;
    int adjY;
    std::cout << "Target Node: " << getASCII(xFind, yFind) << " " << xFind << " " << yFind << std::endl;
    for (int i = 0; i < tarNode->adjNodes.size(); i++)
    {
        adjX = tarNode->adjNodes[i].adjNode->x;
        adjY = tarNode->adjNodes[i].adjNode->y;
        std::cout << "Adj Node: " << getASCII(adjX, adjY) << " " << adjX << " " << adjY << std::endl;
    }
}