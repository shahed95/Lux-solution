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

#include "GameData.hpp"
#include "GameAlgo.hpp"
#include "ClosestResourceFindingState.hpp"
#include "ClosestCityFindingState.hpp"
#include "ClosestBuildSpaceFindingState.hpp"
#include "BuildCityState.hpp"

#include "GameData.cpp"
#include "GameAlgo.cpp"
#include "ClosestResourceFindingState.cpp"
#include "ClosestCityFindingState.cpp"
#include "ClosestBuildSpaceFindingState.cpp"
#include "BuildCityState.cpp"

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
        vector<string> actions = vector<string>();
        actions.push_back(Annotate::sidetext("turn "+ to_string(gameState.turn)));
        //-------------------------------------------------------------------------------------------------------//
        // city Action AI
        //-------------------------------------------------------------------------------------------------------//

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

        //-------------------------------------------------------------------------------------------------------//
        // player Action AI
        //-------------------------------------------------------------------------------------------------------//

        
        for (int i = 0; i < gameData->player.units.size(); i++)
        {
            Unit &unit = gameData->player.units[i];
            if(unit.getState() == nullptr)
            {
                actions.push_back(Annotate::sidetext(unit.id+ "actions not found"));
                continue;
            }
            if (unit.canAct())
            {
                unit.prepare_act();
                actions.push_back(unit.act());
            }
            actions.push_back(Annotate::sidetext(unit.id+ unit.getStateName()));
            // save Unitstate to Game Data, it gets cleared in gameState.update()
            gameData->updateUnitState(unit);
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
        gameState.end_turn();
    }

    return 0;
}
