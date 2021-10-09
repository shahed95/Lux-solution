
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
    unitMap = GameAlgo::createUnitMap(gameMap,player,opponent);

    // create "distance from target" arrays which will help units to decide which cell to move
    updateTakableResource();

    distfromCities = GameAlgo::makeDistfromCities(simpleMap,false);
    distfromOpponentCities = GameAlgo::makeDistfromCities(simpleMap,true);
    distfromResource = GameAlgo::makeDistfromResource(simpleMap, takableResource, player, opponent);
    distfromDots = GameAlgo::makeDistfromDots(simpleMap);
    distfromGoodDots = GameAlgo::makeDistfromGoodDots(simpleMap);
    distfromPlayer = GameAlgo::makeDistfromPlayer(simpleMap, player);
    distfromOpponent = GameAlgo::makeDistfromOpponent(simpleMap, opponent);
    distfromDotsNeg = distfromDots;
    for(int i=0; i<distfromDotsNeg.size(); i++)
    {
        for(int j=0; j<distfromDotsNeg.size(); j++)
        {
            distfromDotsNeg[i][j] *=-1;
        }
    }
    if (gameState.turn == 0)
        initClusters();

    updateClusters();
}

void GameData::updateTakableResource()
{
    if (gameState.turn == 0)
        takableResource += "w";
    if (takableResource.size() == 1 && player.researchPoints >= 40)
        takableResource += "c";
    if (takableResource.size() == 2 && player.researchPoints >= 160)
        takableResource += "u";
}

void GameData::initClusters()
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

void GameData::updateClusters()
{
    for (int i = 0; i < resourceClusters.size(); i++)
    {
        resourceClusters[i].updateCluster(gameData->simpleMap, gameData->unitMap, takableResource);
        distfromResourceCluster[i] = GameAlgo::bfsOnMap(resourceClusters[i].cells, GameAlgo::getAllposition("zyb", simpleMap), simpleMap);
    }
}
