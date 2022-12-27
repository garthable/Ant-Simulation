#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>

#include "ant.h"

#pragma once

struct pheromone
{
    int type; //0 = none, 1 = home, 2 = food
    int strength;

    pheromone()
    {

    }
    pheromone(int newType, int newStrength)
    {
        type = newType;
        strength = newStrength;
    }
};

struct adj;

struct node
{
    int x;
    int y;
    int state;
    int foodAmount;
    pheromone pher;
    std::vector<bool> visited;
    std::vector<adj> adjNodes;
    
    node (int newX, int newY, int newState, int newFoodAmount, pheromone newPheromone)
    {
        x = newX;
        y = newY;
        state = newState;
        foodAmount = newFoodAmount;
        pher = newPheromone;
        pher = newPheromone;
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
        std::vector<node*> points;
        std::vector<ant> ants;
        std::string pherFN;
        std::string physFN;
        int xSize;
        int ySize;
        bool isPher;
        std::stack<node*> changedPher;
        std::stack<node*> changedPhys;

    public:
        map(std::string, std::string, int, int);
        ~map();

        void nukeMap();
        node* findNode(int, int);
        void generateNodes();
        void stitchNodes();
        void reducePheromone();
        
        char getASCII(int, int);
        void updatePher();
        void updatePhys();
        void update();

        void displayNodeData(int, int);
};