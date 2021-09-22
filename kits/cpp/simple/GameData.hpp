#ifndef GAMEDATA_H
#define GAMEDATA_H
#pragma once

#include "lux/kit.hpp"
#include "lux/define.cpp"
#include <string.h>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <stdio.h>
#include "GameAlgo.cpp"
using namespace std;
using namespace lux;

// singleton Class
// common access point
// knows everything

class GameData
{
private:
    static GameData *gameData;
    GameData();

public:
    vector<kit::Agent> gameStateHistory;
    kit::Agent gameState;

    Player player;
    Player opponent;
    GameMap gameMap;

    vector<vector<char>> simpleMap;

    static GameData *getInstance();
    void updateGameData(kit::Agent gameState);
};
#endif