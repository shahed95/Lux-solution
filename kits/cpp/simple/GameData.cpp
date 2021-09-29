
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
    initDistfromCities();
    initDistfromDots1();
    initDistfromResource2();

    updateCluster();
}

void GameData::initDistfromCities()
{
    distfromCities = GameAlgo::createDistanceArray("y", "zb", simpleMap);
}

void GameData::initDistfromDots1()
{
    distfromDots = GameAlgo::createDistanceArray(".", "yzb", simpleMap);
}

void GameData::initDistfromDots2()
{
}

void GameData::initDistfromResource1()
{
    string needResource = (player.researchPoints >= 200) ? "ucw" : ((player.researchPoints >= 50) ? "cw" : "w");
    distfromResource = GameAlgo::createDistanceArray(needResource, "zb", simpleMap);
}

void GameData::initDistfromResource2()
{
    string needResource = (player.researchPoints >= 200) ? "ucw" : ((player.researchPoints >= 50) ? "cw" : "w");

    auto tempMap = simpleMap;
    auto oppMap = tempMap;

    for (auto u : opponent.units)
        oppMap[u.pos.x][u.pos.y] = 'o';

    auto distFromOpponent = GameAlgo::createDistanceArray("o", "yb", oppMap);

    for (int i = 1; i < tempMap.size() - 1; i++)
    {
        for (int j = 1; j < tempMap.size() - 1; j++)
        {
            if (distFromOpponent[i][j] < 10)
                continue;
            if (tempMap[i][j] != 'c' && tempMap[i][j] != 'u' && tempMap[i][j] != 'w')
                continue;

            int citycells = 0;
            if (tempMap[i - 1][j] == 'y')
                citycells++;
            if (tempMap[i + 1][j] == 'y')
                citycells++;
            if (tempMap[i][j - 1] == 'y')
                citycells++;
            if (tempMap[i][j + 1] == 'y')
                citycells++;

            if (citycells >= 2)
            {
                tempMap[i][j] = 'b';
                if (tempMap[i - 1][j] != 'y')
                    tempMap[i - 1][j] = 'b';
                if (tempMap[i + 1][j] != 'y')
                    tempMap[i + 1][j] = 'b';
                if (tempMap[i][j - 1] != 'y')
                    tempMap[i][j - 1] = 'b';
                if (tempMap[i][j + 1] != 'y')
                    tempMap[i][j + 1] = 'b';
            }
        }
    }

    distfromResource = GameAlgo::createDistanceArray(needResource, "zb", tempMap);
}

void GameData::updateCluster()
{
    if (gameState.turn == 0)
    {
        vector<vector<char>> tempMap = simpleMap;

        string resourceOrder = "wcu";
        for (auto r : resourceOrder)
        {
            for (int i = 0; i < tempMap.size(); i++)
            {
                for (int j = 0; j < tempMap[i].size(); j++)
                {
                    if (tempMap[i][j] == r)
                    {
                        resourceClusters.push_back(Cluster(tempMap, tempMap[i][j], {i, j}));
                        for (auto u : resourceClusters.back().cells)
                            tempMap[u.first][u.second] = '.';
                    }
                }
            }
        }
        distfromResourceCluster.resize(resourceClusters.size());
    }

    for (int i = 0; i < resourceClusters.size(); i++)
    {
        resourceClusters[i].updateCluster(gameData->simpleMap);
        distfromResourceCluster[i] = GameAlgo::bfsOnMap(resourceClusters[i].cells, GameAlgo::getAllposition("zb", simpleMap), simpleMap);
        if (resourceClusters[i].type == 'c')
            resourceClusters[i].islocked = (player.researchPoints >= 50) ? 0 : 1;
        if (resourceClusters[i].type == 'u')
            resourceClusters[i].islocked = (player.researchPoints >= 200) ? 0 : 1;
    }
}
