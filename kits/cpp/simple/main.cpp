#include "lux/kit.hpp"
#include "lux/define.cpp"
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cstdio>
#include <queue>

using namespace std;
using namespace lux;

#include "Cluster.hpp"
#include "GameData.hpp"
#include "GameAlgo.hpp"
#include "ClosestResourceFindingState.hpp"
#include "ClosestCityFindingState.hpp"
#include "ClosestBuildSpaceFindingState.hpp"
#include "BuildCityState.hpp"
#include "UnitExtraData.hpp"
#include "TargetResourceFindingState.hpp"

#include "GameData.cpp"
#include "GameAlgo.cpp"
#include "ClosestResourceFindingState.cpp"
#include "ClosestCityFindingState.cpp"
#include "ClosestBuildSpaceFindingState.cpp"
#include "BuildCityState.cpp"
#include "UnitExtraData.cpp"
#include "Cluster.cpp"
#include "TargetResourceFindingState.cpp"

// Code writing goal
// 1. easy to extend 2. less modification 3. easy to experiment/adjust

// Gamedata is singleton class where all the data are organised and stored.
// GameAlgo is static class where all the common algorithms are added.

// Units are stateful
// Unit's state is declared in UnitExtraData class.
// each state has its own state class.

// some unit have one target : usually the closest target
// some unit have two target : first target(intermidiate) and second(main) target
// some unit which target for bigger cluster go there via an intermidiate cluster

int main()
{
    kit::Agent gameState = kit::Agent();
    // initialize
    gameState.initialize();
    GameData *gameData = GameData::getInstance();

    while (true)
    {
        gameState.update();
        gameData->updateGameData(gameState);
        UnitExtraData::updateUnitExtraData(gameData->player);

        vector<string> actions = vector<string>();
        actions.push_back(Annotate::sidetext("turn " + to_string(gameState.turn)));

        //1. City AI
        int unitBuilable = gameData->player.cityTileCount - gameData->player.units.size();
        for (auto city : gameData->player.cities)
        {
            for (auto citytiles : city.second.citytiles)
            {
                if (citytiles.canAct() && unitBuilable > 0)
                {
                    actions.push_back(citytiles.buildWorker());
                    unitBuilable--;
                }
                else if (citytiles.canAct() && gameData->player.researchPoints < 200)
                {
                    actions.push_back(citytiles.research());
                    gameData->tilesReasearchRate++;
                }
            }
        }

        //2. Unit AI

        vector<pair<int, int>> temp;
        for (int i = 0; i < gameData->player.units.size(); i++)
        {
            Unit &unit = gameData->player.units[i];
            temp.push_back({unit.getState()->getPriority() , i});
        }

        sort(temp.begin(), temp.end());

        for (int i = 0; i < gameData->player.units.size(); i++)
        {
            Unit &unit = gameData->player.units[i];
            
            if (unit.canAct())
                actions.push_back(unit.act());
            actions.push_back(Annotate::sidetext(unit.id + unit.getStateName()));
        }

        //3. API code

        for (int i = 0; i < actions.size(); i++)
        {
            if (i != 0)
                cout << ",";
            cout << actions[i];
        }
        cout << endl;
        UnitExtraData::backupUnitExtraData(gameData->player);
        gameState.end_turn();
    }

    return 0;
}
