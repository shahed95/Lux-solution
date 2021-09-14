#include "SimplifiedGame.h"

SimplifiedGame::SimplifiedGame()
{
}
SimplifiedGame::SimplifiedGame(GameMap &gameMap, Player &player, Player &opponent)
{
    createSimpleMap(gameMap);
    addResourceInMap(gameMap);
    addCitiesInMap(player, false);
    addCitiesInMap(opponent, true);
}

void SimplifiedGame::createSimpleMap(GameMap &gameMap)
{
    for (int i = 0; i < gameMap.height; i++)
    {
        vector<pair<char, int>> temp(gameMap.width, {'.', 0});
        simpleMap.push_back(temp);
    }
}

void SimplifiedGame::addResourceInMap(GameMap &gameMap)
{
    for (int y = 0; y < gameMap.height; y++)
    {
        for (int x = 0; x < gameMap.width; x++)
        {
            Cell *cell = gameMap.getCell(x, y);
            if (cell->hasResource())
                simpleMap[x][y] = {cell->getResourceType(), cell->resource.amount}; // 'w', 'c', 'u'
        }
    }
}

void SimplifiedGame::addCitiesInMap(Player &player, bool isOpponent)
{
    int id = 0;
    for (auto city : player.cities)
    {
        id++;
        for (auto citytiles : city.second.citytiles)
        {
            simpleMap[citytiles.pos.x][citytiles.pos.y] = {'y', isOpponent ? (id * 2 + 1) : (id * 2)};
        }
    }
}