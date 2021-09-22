#include "GameData.hpp"

GameData *GameData::gameData = 0;

GameData::GameData()
{

}

GameData *GameData::getInstance()
{
    if (gameData == nullptr)
    {
        gameData = new GameData();
    }
    return gameData;
}

void GameData::updateGameData(kit::Agent gameState)
{
    if (gameState.turn == gameStateHistory.size())
    {
        this->gameState = gameState;
        gameStateHistory.push_back(this->gameState);
        player = gameState.players[gameState.id];
        opponent = gameState.players[(gameState.id + 1) % 2];
        gameMap = gameState.map;
        simpleMap = GameAlgo::createSimpleMap(gameMap,player,opponent);
        for (int i = 0; i < player.units.size(); i++)
        {
            Unit unit = player.units[i];
            if(!unit.canAct() && simpleMap[unit.pos.x][unit.pos.y] == '.')
            {
                simpleMap[unit.pos.x][unit.pos.y] = 'b';
            }
        }
    }
}