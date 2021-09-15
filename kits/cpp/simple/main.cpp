#include "lux/kit.hpp"
#include "lux/define.cpp"
#include <string.h>
#include <vector>
#include <set>
#include <stdio.h>
#include "SimplifiedGame.h"
#include "SimplifiedGame.cpp"
using namespace std;
using namespace lux;

/*---------------------------------------------------
 Plan:
 1. Devide into smaller subproblems
 2. write different Strageties for each subproblem
 3. apply strategy based on the situation
 4. try to find the best combination of strategies
----------------------------------------------------*/

int main()
{
    kit::Agent gameState = kit::Agent();
    // initialize
    gameState.initialize();

    while (true)
    {

        /** Do not edit! **/
        // wait for updates
        gameState.update();

        vector<string> actions = vector<string>();

        /** AI Code Goes Below! **/

        Player &player = gameState.players[gameState.id];
        Player &opponent = gameState.players[(gameState.id + 1) % 2];
        GameMap &gameMap = gameState.map;

        SimplifiedGame sGame = SimplifiedGame(gameMap, player, opponent);

        //-------------------------------------------------------------------------------------------------------//
        // city Action AI
        //-------------------------------------------------------------------------------------------------------//

        int unitBuilable = player.cityTileCount - player.units.size();

        for (auto city : player.cities)
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

        // which direction each worker should move ?
        // if it's mining resources (not adjacent to city) do not move
        //

        // y = my city tiles, z = opponent city tiles
        auto distfromCities = sGame.bfsOnMap(sGame.getAllposition("y"), sGame.getAllposition("z"));
        auto distfromWoods = sGame.bfsOnMap(sGame.getAllposition("w"), sGame.getAllposition("zcu"));
        auto distfromDots = sGame.bfsOnMap(sGame.getAllposition("."), sGame.getAllposition("yzwcu"));

        auto dummyMap = sGame.createEmptyMap(gameMap.height, gameMap.width);

        for (int i = 0; i < player.units.size(); i++)
        {
            Unit unit = player.units[i];
            int ux = unit.pos.x;
            int uy = unit.pos.y;

            if (unit.isWorker() && unit.canAct())
            {
                auto &distarray = distfromCities;

                if (unit.canBuild(gameMap) && i % 2 == 0)
                {
                    actions.push_back(unit.buildCity());
                }
                else if (unit.getCargoSpaceLeft() > 0)
                {
                    distarray = distfromWoods;
                }
                else
                {
                    if(i % 2 == 0) distarray = distfromDots;
                }

                map<int, DIRECTIONS> mp;

                mp[distarray[ux][uy]] = CENTER;

                if (dummyMap[ux - 1][uy] != 'p')
                {
                    mp[distarray[ux - 1][uy]] = WEST;
                }
                if (dummyMap[ux + 1][uy] != 'p')
                {
                    mp[distarray[ux + 1][uy]] = EAST;
                }
                if (dummyMap[ux][uy + 1] != 'p')
                {
                    mp[distarray[ux][uy + 1]] = SOUTH;
                }
                if (dummyMap[ux][uy - 1] != 'p')
                {
                    mp[distarray[ux][uy - 1]] = NORTH;
                }

                DIRECTIONS bestDirection = mp.begin()->second;
                actions.push_back(unit.move(bestDirection));
                unit.pos.translate(bestDirection, 1);
                dummyMap[unit.pos.x][unit.pos.y] = 'p';
            }
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
