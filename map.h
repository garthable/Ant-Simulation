#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

#pragma once

struct pheromone
{
    int returnStrength;
    int foodStrength;
};

struct adj;

struct node
{
    int x;
    int y;
    int state;
    int foodAmount;
    int distance;
    bool visited;
    pheromone pher;
    std::vector<adj> adjNodes;
    
    node (int newX, int newY, int newState, int newFoodAmount)
    {
        x = newX;
        y = newY;
        state = newState;
        foodAmount = newFoodAmount;
    }
};

struct adj
{
    node* adjNode;
    int distance;
};

class map
{
    private:
        std::string mapFileName;
        std::stack<node*> changedMapData;

    public:
        std::vector<node*> points;

        map(std::string);
        ~map();

        void nukeMap();
        node* findNode(int, int);
        void generateNodes();
        void stitchNodes();
        void reducePheromone();
        
        char getASCII(int, int);
        void updateMap();

        void displayNodeData(int, int);
};