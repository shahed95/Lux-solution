#ifndef SIMPLIFIED_GAME
#define SIMPLIFIED_GAME

#include "lux/kit.hpp"
#include "lux/define.cpp"
#include <string.h>
#include <vector>
#include <set>
#include <stdio.h>
using namespace std;
using namespace lux;

class SimplifiedGame
{
public:
    vector<vector<pair<char, int>>> simpleMap;
    SimplifiedGame();
    SimplifiedGame(GameMap &gameMap, Player &player, Player &opponent);

    void createSimpleMap(GameMap &gameMap);

    void addResourceInMap(GameMap &gameMap);

    void addCitiesInMap(Player &player, bool isOpponent);
};

#endif