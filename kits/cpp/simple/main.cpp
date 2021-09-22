#include "lux/kit.hpp"
#include "lux/define.cpp"
#include <string.h>
#include <vector>
#include <set>
#include <stdio.h>
#include "GameData.hpp"
#include "GameData.cpp"
#include "ClosestResourceFindingState.hpp"
#include "ClosestCityFindingState.hpp"
#include "ClosestBuildSpaceFindingState.hpp"
#include "BuildCityState.hpp"

using namespace std;
using namespace lux;

int main()
{
    kit::Agent gameState = kit::Agent();
    // initialize
    gameState.initialize();

    GameData *gameData = GameData::getInstance();
    set<string> unitIDs;
    while (true)
    {
        gameState.update();
        gameData->updateGameData(gameState);
        vector<string> actions = vector<string>();
    
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
            Unit unit = gameData->player.units[i];
            if(unitIDs.count(unit.id) == 0) // new unit 
            {
                unit.TransitionTo(new ClosestCityFindingState());
                unitIDs.insert(unit.id);
            }
            

            if(unit.canAct())
                actions.push_back(unit.act());
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
