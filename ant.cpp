#include <random>
#include <cmath>

#include "ant.h"

ant::ant(int newX, int newY)
{
    x = newX;
    y = newY;
}

int distance(int x0, int x1, int y0, int y1)
{
    int x = abs(x0 - x1);
    int y = abs(y0 - y1);
    return sqrt((x*x) + (y*y));
}

bool ant::isTarget(int x, int y)
{
    for (int i = 0; i < targets.size(); i++)
        if (x == targets[i].x && y == targets[i].y)
            return true;
    return false;
}

void ant::targetUpdate(map* simMap)
{
    for (int i = 0; i < simMap->points.size(); i++)
    {
        int d = distance(x, simMap->points[i]->x, y, simMap->points[i]->y);
        if (isTarget(simMap->points[i]->x, simMap->points[i]->y))
            continue;
        if (d <= radius && (simMap->points[i]->pher.foodStrength > 0 || simMap->points[i]->foodAmount > 0))
        {
            target temp;
            temp.x = simMap->points[i]->x;
            temp.y = simMap->points[i]->y;
            targets.push_back(temp);
        }
    }

    for (int i = 0; i < targets.size(); i++)
    {
        int d = distance(x, targets[i].x, y, targets[i].y);

        //Solve for priority
        if (targets[i].visited)
            targets[i].priority = -1;
        else if (simMap->findNode( targets[i].x,  targets[i].y)->foodAmount > 0)
            targets[i].priority = 0;
        else
            targets[i].priority = d;
    }
    if (targets.size() == 0)
        currTarget == nullptr;
    else
        currTarget = hasTarget(targets);
}

void ant::pickUpFood(map* simMap)
{
    srand(time(0));
    node* currNode = simMap->findNode(x, y);
    for (int i = 0; i < currNode->adjNodes.size(); i++)
    {
        if (currNode->adjNodes[i].adjNode->foodAmount > 0)
        {
            // if (rand() % 3 == 1)
                currNode->adjNodes[i].adjNode->foodAmount--;
            food++;
        }
    }
}

target* hasTarget(std::vector<target> targets)
{
    target *currTarget = nullptr;
    for (int i = 0; i < targets.size(); i++)
        if (currTarget == nullptr || (targets[i].priority > 0 && currTarget->priority > targets[i].priority))
            currTarget = &targets[i];
        else
            continue;
    return currTarget;
}

void ant::move(map* simMap)
{
    targetUpdate(simMap);
    //Implement dijestra search or a* search
    pickUpFood(simMap);
}