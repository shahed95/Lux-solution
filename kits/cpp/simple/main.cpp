#include "lux/kit.hpp"
#include "lux/define.cpp"
#include <string.h>
#include <vector>
#include <set>
#include <stdio.h>
#include "SimplifiedGame.cpp"
#include "ClusterMaker.cpp"
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

string unitAction(Unit unit, vector<vector<int>> &distArray, vector<vector<char>> &visit, SimplifiedGame *sGame)
{
    int closestDist = sGame->unvisited;
    DIRECTIONS closestDirection;
    int closestX = -1, closestY = -1;
    for (int i = 0; i < 5; i++)
    {
        int goX = unit.pos.x + DX[i];
        int goY = unit.pos.y + DY[i];
        if (sGame->isInside(goX, goY) && visit[goX][goY] == '.')
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
    if (closestX == -1)
    {
        return unit.move(D[rand() % 5]);
    }
    if (sGame->getCell(closestX, closestY) != 'y')
        visit[closestX][closestY] = 'x';

    return unit.move(closestDirection);
}

int main()
{
    kit::Agent gameState = kit::Agent();
    // initialize
    gameState.initialize();
    SimplifiedGame *sGame = SimplifiedGame::getInstance();
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

        sGame->updateGame(gameMap, player, opponent);

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
        // y = my city tiles, z = opponent city tiles

        auto distfromCities = sGame->bfsOnMap(sGame->getAllposition("y"), sGame->getAllposition("z"));
        auto distfromDots = sGame->bfsOnMap(sGame->getAllposition("."), sGame->getAllposition("yz"));

        // make many dist resource
        string needResource = (player.researchPoints >= 200) ? "ucw" : ((player.researchPoints >= 50) ? "cw" : "w");
        ClusterMaker clusterMaker;
        clusterMaker.make(sGame->sMap, needResource);

        vector<vector<int>> distfromResource[clusterMaker.clusters.size()];

        for (int i = 0; i < clusterMaker.clusters.size(); i++)
        {
            distfromResource[i] = sGame->bfsOnMap(clusterMaker.clusters[i], sGame->getAllposition("z"));
        }

        map<int, int> taken;

        vector<vector<char>> visit = sGame->createEmptyMap(gameMap.height, gameMap.width);


        for (int i = 0; i < player.units.size(); i++)
        {
            Unit unit = player.units[i];
            int dayLeft = 40 - (gameState.turn % 40);

            if (!unit.canAct())
                continue;

            auto &distArray = distfromResource[0];
            int flag = 0;

            if (unit.getCargoSpaceLeft() == 0)
            {
                if (i % 2 == 0 || gameState.turn <= 20 || player.cityTileCount <= opponent.cityTileCount)
                {
                    if (unit.canBuild(gameMap))
                    {
                        actions.push_back(unit.buildCity());
                        continue;
                    }
                    else
                        distArray = distfromDots;
                }
                else
                    distArray = distfromCities;
            }
            else if (dayLeft < 0)
            {
                distArray = distfromCities;
            }
            else
            {
                for (int j = 0; j < clusterMaker.clusters.size(); j++)
                {
                    if (distfromResource[j][unit.pos.x][unit.pos.y] <= distArray[unit.pos.x][unit.pos.y] && taken[j] <= clusterMaker.clusters[j].size())
                    {
                        distArray = distfromResource[j];
                        taken[j]++;
                    }
                }
            }

            actions.push_back(unitAction(unit, distArray, visit, sGame));
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
