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


// singleton
// simplifies full game details with a simple class

class SimplifiedGame
{
private:
    static SimplifiedGame *simplifiedGame;
    SimplifiedGame();

public:
    static const int unvisited = 999999;
    static const int unreachable = 999998;

    vector<vector<char>> sMap;
    vector<vector<vector<char>>> sMapHistory;

    static SimplifiedGame *getInstance();
    void updateGame(GameMap &gameMap, Player &player, Player &opponent);

    vector<vector<char>> createEmptyMap(int height, int width); 
    void addResourceInMap(GameMap &gameMap);
    void addCitiesInMap(Player &player, bool isOpponent);

    bool isInside(int x, int y);
    char getCell(int x, int y);
    int countNearbyCell(int x, int y, int withinDistance, string types);

    vector<vector<int>> bfsOnMap(vector<pair<int, int>> startingPos, vector<pair<int, int>> unreachablePos);
    vector<vector<int>> dijkstraOnMap(vector<pair<int, int>> startingPos, vector<pair<int, int>> unreachablePos);
    vector<pair<int, int>> getAllposition(string type);
};

#endif