#ifndef GAME_ALGO
#define GAME_ALGO

#include "lux/kit.hpp"
#include "lux/define.cpp"
#include <string.h>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <stdio.h>
using namespace std;
using namespace lux;


// Game Algos, independent functions
// static class

class GameAlgo
{
private:
    

public:
    static const int unvisited = 999999;
    static const int unreachable = 999998;
    static const char outside = '*';

    static vector<vector<char>> createSimpleMap(GameMap &gameMap, Player &player, Player &opponent);
    static char getCell(int x, int y, vector<vector<char>> &simpleMap);
    static vector<vector<int>> bfsOnMap(vector<pair<int, int>> startingPos, vector<pair<int, int>> unreachablePos, vector<vector<char>> &simpleMap);
    static vector<vector<int>> dijkstraOnMap(vector<pair<int, int>> startingPos, vector<pair<int, int>> unreachablePos, vector<vector<char>> &simpleMap);
    static vector<pair<int, int>> getAllposition(string type, vector<vector<char>> &simpleMap);
    static DIRECTIONS moveDirection(Unit unit, vector<vector<int>> &distArray, vector<vector<char>> &simpleMap);
    static pair<int,int> getPosition(int x, int y, DIRECTIONS d);
};

#endif