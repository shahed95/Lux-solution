#include "lux/kit.hpp"
#include "lux/define.cpp"
#include <string.h>
#include <vector>
#include <set>
#include <stdio.h>

using namespace std;
using namespace lux;
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

    vector<Cell *> resourceTiles = vector<Cell *>();
    vector<Cell *> freeTiles = vector<Cell *>();
    for (int y = 0; y < gameMap.height; y++)
    {
      for (int x = 0; x < gameMap.width; x++)
      {
        Cell *cell = gameMap.getCell(x, y);
        if (cell->hasResource())
        {
          if(cell->hasWood()) resourceTiles.push_back(cell);
        }
        else
        {
          freeTiles.push_back(cell);
        }
      }
    }

    // we iterate over all cities

    int need_unit = player.cityTileCount - player.units.size();
    set<pair<int,int> > citytilesPos;
    for (auto city : player.cities)
    {
      for(auto citytiles : city.second.citytiles)
      {
        citytilesPos.insert({citytiles.pos.x,citytiles.pos.y});
        if(citytiles.canAct() && need_unit > 0)
        {
          actions.push_back(citytiles.buildWorker());
          need_unit--;
        } 
        else if (citytiles.canAct())
        {
          actions.push_back(citytiles.research());
        }
      }
    }

    // we iterate over all our units and do something with them
    set<pair<int,int> > taken;

    for (int i = 0; i < player.units.size(); i++)
    {
      Unit unit = player.units[i];
      if (unit.isWorker() && unit.canAct())
      {
        if (unit.getCargoSpaceLeft() > 0)
        {
          // if the unit is a worker and we have space in cargo, lets find the nearest resource tile and try to mine it
          Cell *closestResourceTile;
          float closestDist = 9999999;
          for (auto it = resourceTiles.begin(); it != resourceTiles.end(); it++)
          {
            auto cell = *it;
            if (cell->resource.type == ResourceType::coal && !player.researchedCoal()) continue;
            if (cell->resource.type == ResourceType::uranium && !player.researchedUranium()) continue;
            float dist = cell->pos.distanceTo(unit.pos);
            

            
            if (dist < closestDist )
            {
              closestDist = dist;
              closestResourceTile = cell;
            }
          }


          if (closestResourceTile != nullptr)
          {
            auto dir = unit.pos.directionTo(closestResourceTile->pos);
            int posx = unit.pos.x;
            int posy = unit.pos.y;
            if(dir == NORTH)
            {
              posy--;
              if(taken.count({posx,posy}))
              {
                posy++;
                dir = EAST;
              }
            }
            if(dir == EAST)
            {
              posx++;
              if(taken.count({posx,posy}))
              {
                posx--;
                dir = SOUTH;
              }
            }
            if(dir == SOUTH)
            {
              posy++;
              if(taken.count({posx,posy}))
              {
                posy--;
                dir = WEST;
              }
            }
            if(dir == WEST)
            {
              posx--;
              if(taken.count({posx,posy}))
              {
                posx++;
                dir = CENTER;
              }
            }

            if(citytilesPos.count({posx,posy})==0) taken.insert({posx,posy});
           
            actions.push_back(unit.move(dir));
          }
        }
        else if (i % 2 == 0)
        {
          if(unit.canBuild(gameMap))
          {
            actions.push_back(unit.buildCity());
          }
          else // find a free space
          {
             Cell *closestfreeTile;
             float closestDist = 9999999;

             for (auto it = freeTiles.begin(); it != freeTiles.end(); it++)
             {
               auto cell = *it;
               if(citytilesPos.count({cell->pos.x,cell->pos.y})) continue;
               float dist = cell->pos.distanceTo(unit.pos);
               if (dist < closestDist)
               {
                 closestDist = dist;
                 closestfreeTile = cell;
               }
             }
             if (closestfreeTile != nullptr)
             {
               auto dir = unit.pos.directionTo(closestfreeTile->pos);
               int posx = unit.pos.x;
            int posy = unit.pos.y;
            if(dir == NORTH)
            {
              posy--;
              if(taken.count({posx,posy}))
              {
                posy++;
                dir = EAST;
              }
            }
            if(dir == EAST)
            {
              posx++;
              if(taken.count({posx,posy}))
              {
                posx--;
                dir = SOUTH;
              }
            }
            if(dir == SOUTH)
            {
              posy++;
              if(taken.count({posx,posy}))
              {
                posy--;
                dir = WEST;
              }
            }
            if(dir == WEST)
            {
              posx--;
              if(taken.count({posx,posy}))
              {
                posx++;
                dir = CENTER;
              }
            }

            if(citytilesPos.count({posx,posy})==0) taken.insert({posx,posy});
               actions.push_back(unit.move(dir));
             }
          }
        }
        else
        {
          // if unit is a worker and there is no cargo space left, and we have cities, lets return to them
          if (player.cities.size() > 0)
          {
            auto city_iter = player.cities.begin();
            auto &city = city_iter->second;

            float closestDist = 999999;
            CityTile *closestCityTile;
            for (auto &citytile : city.citytiles)
            {
              float dist = citytile.pos.distanceTo(unit.pos);
              if (dist < closestDist)
              {
                closestCityTile = &citytile;
                closestDist = dist;
              }
            }
            if (closestCityTile != nullptr)
            {
              auto dir = unit.pos.directionTo(closestCityTile->pos);
              int posx = unit.pos.x;
            int posy = unit.pos.y;
            if(dir == NORTH)
            {
              posy--;
              if(taken.count({posx,posy}))
              {
                posy++;
                dir = EAST;
              }
            }
            if(dir == EAST)
            {
              posx++;
              if(taken.count({posx,posy}))
              {
                posx--;
                dir = SOUTH;
              }
            }
            if(dir == SOUTH)
            {
              posy++;
              if(taken.count({posx,posy}))
              {
                posy--;
                dir = WEST;
              }
            }
            if(dir == WEST)
            {
              posx--;
              if(taken.count({posx,posy}))
              {
                posx++;
                dir = CENTER;
              }
            }

            if(citytilesPos.count({posx,posy})==0) taken.insert({posx,posy});
              actions.push_back(unit.move(dir));
            }
          }
        }
      }
    }

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
