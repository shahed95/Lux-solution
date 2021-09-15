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

const int DX[] = {0, 0, 0, -1, 1};
const int DY[] = {0, 1, -1, 0, 0};
const DIRECTIONS D[] = {CENTER, SOUTH, NORTH, WEST, EAST};

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
        auto distfromResource = sGame.bfsOnMap(sGame.getAllposition("w"), sGame.getAllposition("zcu"));
        auto distfromDots = sGame.bfsOnMap(sGame.getAllposition("."), sGame.getAllposition("yz"));

        if (player.researchPoints >= 50)
        {
            distfromResource = sGame.bfsOnMap(sGame.getAllposition("wc"), sGame.getAllposition("zu"));
        }
        if (player.researchPoints >= 200)
        {
            distfromResource = sGame.bfsOnMap(sGame.getAllposition("wcu"), sGame.getAllposition("z"));
        }

        set<pair<int, int>> taken;

        for (int i = 0; i < player.units.size(); i++)
        {
            Unit unit = player.units[i];

            if (!unit.canAct())
                continue;

            int closestDist = 999999;
            DIRECTIONS closestDirection;
            int closestX, closestY;

            auto &distArray = distfromResource;

            if (unit.getCargoSpaceLeft() == 0) // has enough
            {
                if (i % 2 == 0)
                {
                    if (unit.canBuild(gameMap))
                    {
                        actions.push_back(unit.buildCity());
                        continue;
                    }
                    else
                    {
                        distArray = distfromDots;
                    }
                }
                else
                {
                    distArray = distfromCities;
                }
            }
            else if ((gameState.turn % 40 + 5) >= 30 && distfromCities.size() != 0)
            {
                distArray = distfromCities;
            }

            for (int i = 0; i < 5; i++)
            {
                int goX = unit.pos.x + DX[i];
                int goY = unit.pos.y + DY[i];
                if (sGame.isInside(goX, goY) && taken.count({goX, goY}) == 0)
                {
                    if (distArray[goX][goY] < closestDist)
                    {
                        closestDist = distArray[goX][goY];
                        closestDirection = D[i];
                        closestX = goX;
                        closestY = goY;
                    }
                }
            }

            actions.push_back(unit.move(closestDirection));
            if (sGame.getCellType(closestX, closestY) != 'y')
            {
                taken.insert({closestX, closestY});
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
