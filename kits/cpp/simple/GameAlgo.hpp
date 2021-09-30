#ifndef GAME_ALGO
#define GAME_ALGO
#pragma once

#include <queue>
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
    static DIRECTIONS moveDirection(Unit *unit, vector<vector<int>> &distArray, vector<vector<char>> &simpleMap);
    static pair<int,int> getPosition(int x, int y, DIRECTIONS d);
    static vector<vector<int>> createDistanceArray(string sources, string blocks, vector<vector<char>> &simpleMap);
    static vector<vector<int>> initDistfromCities(vector<vector<char>>&simpleMap);
    static vector<vector<int>> initDistfromDots1(vector<vector<char>>&simpleMap);
    static vector<vector<int>> initDistfromResource1(vector<vector<char>>&simpleMap, string withResource,Player &player, Player &opponent);
    static vector<vector<int>> initDistfromResource2(vector<vector<char>>&simpleMap, string withResource,Player &player, Player &opponent);

};

#endif