
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
        
        // save basic data
        player = gameState.players[gameState.id];
        opponent = gameState.players[(gameState.id + 1) % 2];
        gameMap = gameState.map;

        // create a simplified game map with resource, city, and blocked cells
        simpleMap = GameAlgo::createSimpleMap(gameMap,player,opponent);
        for (int i = 0; i < player.units.size(); i++)
        {
            Unit &unit = player.units[i];
            if(!unit.canAct() && simpleMap[unit.pos.x][unit.pos.y] == '.')
            {
                simpleMap[unit.pos.x][unit.pos.y] = 'b';
            }
        }


        // create "distance from target" arrays which will help units to decide which cell to move 
        distfromCities = GameAlgo::createDistanceArray("y","zb",simpleMap);
        distfromDots = GameAlgo::createDistanceArray(".","yzb",simpleMap);
        string needResource = (player.researchPoints >= 200) ? "ucw" : ((player.researchPoints >= 50) ? "cw" : "w");
        distfromResource = GameAlgo::createDistanceArray(needResource,"zb",simpleMap);
    
        // give each unit a state, it gets cleared in every turn, so need to save in map

        for (int i = 0; i < player.units.size(); i++)
        {
            Unit &unit = player.units[i]; // using reference is important here
            if(unitStateMap.count(unit.id) == 0) // new unit
            {
                unitStateMap[unit.id] = new ClosestCityFindingState();
            }
            unit.TransitionTo(unitStateMap[unit.id]);
        }
    }
}


void GameData::updateUnitState(Unit &unit)
{
    if(unit.getState() == nullptr)
    {
        return;    
    }
    unitStateMap[unit.id] = unit.getState();
}