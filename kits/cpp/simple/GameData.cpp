
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
    if (gameState.turn != gameStateHistory.size())
        return;

    this->gameState = gameState;
    gameStateHistory.push_back(this->gameState);

    // save basic data
    player = gameState.players[gameState.id];
    opponent = gameState.players[(gameState.id + 1) % 2];
    gameMap = gameState.map;

    // create a simplified game map with resource, city, and blocked cells
    simpleMap = GameAlgo::createSimpleMap(gameMap, player, opponent);

    // create "distance from target" arrays which will help units to decide which cell to move
    distfromCities = GameAlgo::createDistanceArray("y", "zb", simpleMap);
    distfromDots = GameAlgo::createDistanceArray(".", "yzb", simpleMap);
    string needResource = (player.researchPoints >= 200) ? "ucw" : ((player.researchPoints >= 50) ? "cw" : "w");
    distfromResource = GameAlgo::createDistanceArray(needResource, "zb", simpleMap);

    updateCluster();
}

void GameData::updateCluster()
{
    if (gameState.turn == 0)
    {
        vector<vector<char>> tempMap = simpleMap;
        for (int i = 0; i < tempMap.size(); i++)
        {
            for (int j = 0; j < tempMap[i].size(); j++)
            {
                if (tempMap[i][j] == 'w' || tempMap[i][j] == 'c' || tempMap[i][j] == 'u')
                {
                    resourceClusters.push_back(Cluster(tempMap, tempMap[i][j], {i, j}));
                    for (auto u : resourceClusters.back().cells)
                        tempMap[u.first][u.second] = '.';
                }
            }
        }
    }

    for (auto &u : resourceClusters)
        u.updateCluster(gameData->simpleMap);
}
