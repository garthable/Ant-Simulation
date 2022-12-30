#include <string>
#include <fstream>
#include <vector>

#pragma once
#include "map.h"

struct target
{
    int x;
    int y;
    int priority = -1;
    bool visited = false;
};

class ant
{
    private:
        int x;
        int y;
        int mode; //0 seek food, 1 bring food back
        int radius;
        int food;
        int lifeSpan;
        int colonyLocation[2];
        target* currTarget = nullptr;
        std::vector<target> targets;
        std::vector<std::vector<node>> path;
    public:
        ant(int, int);
        void move(map*);
        bool isTarget(int, int);
        void targetUpdate(map*);
        void pickUpFood(map*);
};