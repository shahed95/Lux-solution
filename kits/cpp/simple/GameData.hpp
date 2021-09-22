#ifndef GAMEDATA_H
#define GAMEDATA_H
#pragma once

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
    vector<vector<int>> distfromCities;
    vector<vector<int>> distfromDots;
    vector<vector<int>> distfromResource;

    static GameData *getInstance();
    void updateGameData(kit::Agent gameState);
};
#endif