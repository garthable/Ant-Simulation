#include "map.h"

map::map(std::string newMapFileName)
{
    mapFileName = newMapFileName;
}

map::~map()
{
    for (int i = 0; i < points.size(); i++)
        delete points[i];
    std::cout << "Deleted map!" << std::endl;
}

void map::generateNodes()
{
    std::string currLine;
    std::fstream mapFile;

    int nodeState = 0;
    int foodAmount = 0;

    int y = 0;

    node *node_ptr;

    char nodeRep;

    mapFile.open(mapFileName);

    while(getline(mapFile, currLine))
    {
        for (int x = 0; x < currLine.length(); x++)
        {
            nodeRep = currLine.at(x);

            if (nodeRep == '-') //Empty (-)
                nodeState = 0;
            else if (nodeRep == '#') //Contains wall (#)
                nodeState = 1;
            else if (nodeRep == 'a') //Contains ant (a)
                nodeState = 2;
            else if (isdigit(nodeRep)) //Contains food (number)
            {
                nodeState = 3;
                foodAmount = int(nodeRep);
            }

            node_ptr = new node(x, y, nodeState, foodAmount);
            points.push_back(node_ptr); 
            foodAmount = 0;
        }
        y++;
    }
    for (int i = 0; i < points.size(); i++)
    {
        if (points[i] == nullptr)
        {
            std::cout << "generateNodes ERROR: Node not generated!" << std::endl;
            break;
        }
    }
    mapFile.close();
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

void map::updateMap()
{
    std::fstream file;
    std::string line;
    std::string newLine;
    file.open(mapFileName);
    while (!changedMapData.empty())
    {
        //Resets
        file.clear();
        file.seekg(0);
        newLine = "";

        //Gets line located at y
        file.seekg(changedMapData.top()->y);
        getline(file, line);

        //Restructures line at y
        newLine.append(line.substr(0, changedMapData.top()->x-1));
        newLine = newLine + getASCII(changedMapData.top()->x, changedMapData.top()->y);
        newLine.append(line.substr(changedMapData.top()->x+1, line.length()));

        //Sets newLine to line
        file << newLine;
        changedMapData.pop();
    }
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