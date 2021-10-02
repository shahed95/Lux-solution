
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
    string withResource = (player.researchPoints >= 180) ? "ucw" : ((player.researchPoints >= 40) ? "cw" : "w");
    
    distfromCities = GameAlgo::makeDistfromCities(simpleMap);
    distfromResource = GameAlgo::makeDistfromResource2(simpleMap,withResource,player,opponent);
    distfromDots = GameAlgo::makeDistfromDots1(simpleMap);
    distfromGoodDots = GameAlgo::makeDistfromGoodDots(simpleMap);
    distfromPlayer = GameAlgo::makeDistfromPlayer(simpleMap,player);
    distfromOpponent = GameAlgo::makeDistfromOpponent(simpleMap,opponent);

    distfromDotsNeg = distfromDots;
    for(int i=0; i<distfromDotsNeg.size(); i++)
    {
        for(int j=0; j<distfromDotsNeg.size(); j++)
        {
            distfromDotsNeg[i][j] *=-1;
        }
    }

 
    updateCluster();
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
        distfromResourceCluster[i] = GameAlgo::bfsOnMap(resourceClusters[i].cells, GameAlgo::getAllposition("zyb", simpleMap), simpleMap);
        if (resourceClusters[i].type == 'c')
            resourceClusters[i].islocked = (player.researchPoints >= 40) ? 0 : 1;
        if (resourceClusters[i].type == 'u')
            resourceClusters[i].islocked = (player.researchPoints >= 180) ? 0 : 1;
    }
}
