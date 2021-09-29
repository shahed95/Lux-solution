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
#include "InsideCityState.hpp"
#include "TargetResourceFindingState.hpp"

#include "GameData.cpp"
#include "GameAlgo.cpp"
#include "ClosestResourceFindingState.cpp"
#include "ClosestCityFindingState.cpp"
#include "ClosestBuildSpaceFindingState.cpp"
#include "BuildCityState.cpp"
#include "UnitExtraData.cpp"
#include "Cluster.cpp"
#include "InsideCityState.cpp"
#include "TargetResourceFindingState.cpp"



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
                else if (citytiles.canAct())
                {
                    actions.push_back(citytiles.research());
                }
            }
        }

        //2. Unit AI
        for (int i = 0; i < gameData->player.units.size(); i++)
        {
            Unit &unit = gameData->player.units[i];
            unit.prepare_act();
            if (unit.canAct())
                actions.push_back(unit.act());
            actions.push_back(Annotate::sidetext(unit.id + unit.getStateName()));
        }

        // -------------------------------------------------------------------------------------------------------//
        // you can add debug annotations using the methods of the Annotate class.
        // actions.push_back(Annotate::circle(0, 0));

        /** AI Code Goes Above! **/

        /** Do not edit! **/
        for (int i = 0; i < actions.size(); i++)
        {
            if (i != 0)
                cout << ",";
            cout << actions[i];
        }
        cout << endl;
        // end turn
        UnitExtraData::backupUnitExtraData(gameData->player);
        gameState.end_turn();
    }

    return 0;
}
