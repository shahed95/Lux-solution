#ifndef SIMPLIFIED_GAME
#define SIMPLIFIED_GAME

#include "lux/kit.hpp"
#include "lux/define.cpp"
#include <string.h>
#include <vector>
#include <queue>
#include <set>
#include <stdio.h>
using namespace std;
using namespace lux;

class SimplifiedGame
{
public:
    vector<vector<pair<char, int>>> sMap;

    SimplifiedGame();

    SimplifiedGame(GameMap &gameMap, Player &player, Player &opponent);

    void createSimpleMap(GameMap &gameMap);

    void addResourceInMap(GameMap &gameMap);

    void addCitiesInMap(Player &player, bool isOpponent);

    bool isInside(int x, int y);

    int closeCityCount(int x, int y, int withinDistance);

    char getCellType(int x, int y);

    vector<vector<int>> bfsOnMap(vector<pair<int, int>> startingPos, vector<pair<int, int>> unreachablePos);

    vector<pair<int,int> > getAllposition(string type);
    vector<pair<int,int> > getAllposition(string type, int mod, int select);
    vector<pair<int, int>> getAllResourcePosition(int researchPoint);

    vector<vector<char>> createEmptyMap(int height, int width);


};

#endif