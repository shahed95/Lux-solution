#ifndef GAMEALGO_H
#define GAMEALGO_H
#define vpii vector<pair<int, int>>
#pragma once

#include "lux/kit.hpp"
#include "lux/define.cpp"
#include <string.h>
#include <vector>
#include <set>
#include <stdio.h>
#include "SimplifiedGame.hpp"
#include "SimplifiedGame.cpp"

using namespace std;
using namespace lux;

class GameAlgo
{
private:
    static GameAlgo *gameAlgo;
    GameAlgo();
    static SimplifiedGame sGame;
public:
    static vector<vector<int>> distanceFromSourceBFS(vpii startingPos, vpii unreachablePos);
    static vector<pair<int, int>> getAllposition(string type);
    static vector<vector<pair<int, int>>> getClusters();
    static int countNearbyCell(int x, int y, int withinDistance, string types);

};
#endif