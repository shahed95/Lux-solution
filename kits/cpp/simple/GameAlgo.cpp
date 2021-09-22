#include "GameAlgo.hpp"

vector<vector<char>> GameAlgo::createSimpleMap(GameMap &gameMap, Player &player, Player &opponent)
{
    vector<vector<char>> simpleMap;
    for (int i = 0; i < gameMap.height; i++)
    {
        vector<char> temp(gameMap.width, '.');
        simpleMap.push_back(temp);
    }

    for (int y = 0; y < gameMap.height; y++)
    {
        for (int x = 0; x < gameMap.width; x++)
        {
            Cell *cell = gameMap.getCell(x, y);
            if (cell->hasResource())
                simpleMap[x][y] = cell->getResourceType();
        }
    }

    for (auto city : player.cities)
    {
        for (auto citytiles : city.second.citytiles)
        {
            simpleMap[citytiles.pos.x][citytiles.pos.y] = 'y';
        }
    }

    for (auto city : opponent.cities)
    {
        for (auto citytiles : city.second.citytiles)
        {
            simpleMap[citytiles.pos.x][citytiles.pos.y] = 'z';
        }
    }

    return simpleMap;
}

char GameAlgo::getCell(int x, int y, vector<vector<char>> &simpleMap)
{
    if (x < 0 || x >= simpleMap.size() || y < 0 || y >= simpleMap[0].size())
        return outside;
    return simpleMap[x][y];
}

vector<vector<int>> GameAlgo::bfsOnMap(vector<pair<int, int>> startingPos, vector<pair<int, int>> unreachablePos, vector<vector<char>> &simpleMap)
{
    int dx[] = {0, 0, -1, 1};
    int dy[] = {1, -1, 0, 0};

    vector<vector<int>> dist;

    for (int i = 0; i < simpleMap.size(); i++)
    {
        vector<int> temp(simpleMap[0].size(), unvisited);
        dist.push_back(temp);
    }

    for (int i = 0; i < unreachablePos.size(); i++)
    {
        dist[unreachablePos[i].first][unreachablePos[i].second] = unreachable;
    }

    queue<pair<int, int>> q;

    for (int i = 0; i < startingPos.size(); i++)
    {
        q.push(startingPos[i]);
        dist[startingPos[i].first][startingPos[i].second] = 0;
    }

    while (!q.empty())
    {
        auto curr = q.front();
        q.pop();
        for (int i = 0; i < 4; i++)
        {
            int x = curr.first + dx[i];
            int y = curr.second + dy[i];

            if (getCell(x, y, simpleMap) == outside)
                continue;

            if (dist[x][y] == unvisited)
            {
                dist[x][y] = dist[curr.first][curr.second] + 1;
                q.push({x, y});
            }
        }
    }
    return dist;
}

vector<vector<int>> GameAlgo::dijkstraOnMap(vector<pair<int, int>> startingPos, vector<pair<int, int>> unreachablePos, vector<vector<char>> &simpleMap)
{
    int dx[] = {0, 0, -1, 1};
    int dy[] = {1, -1, 0, 0};

    vector<vector<int>> dist;
    vector<vector<bool>> vis;

    for (int i = 0; i < simpleMap.size(); i++)
    {
        vector<int> temp(simpleMap[0].size(), unvisited);
        vector<bool> tempbool(simpleMap[0].size(), false);

        dist.push_back(temp);
        vis.push_back(tempbool);
    }

    for (int i = 0; i < unreachablePos.size(); i++)
    {
        dist[unreachablePos[i].first][unreachablePos[i].second] = unreachable;
    }

    multiset<pair<int, pair<int, int>>> q;

    for (int i = 0; i < startingPos.size(); i++)
    {
        q.insert(make_pair(0, startingPos[i]));
        dist[startingPos[i].first][startingPos[i].second] = 0;
    }

    while (!q.empty())
    {
        auto curr = *q.begin();
        q.erase(q.begin());
        int currX = curr.second.first;
        int currY = curr.second.second;

        if (vis[currX][currY] == true)
            continue;

        vis[currX][currY] = true;

        for (int i = 0; i < 4; i++)
        {
            int x = currX + dx[i];
            int y = currY + dy[i];

            if (getCell(x, y, simpleMap) == outside)
                continue;

            int cost = (getCell(x, y, simpleMap) == 'y') ? 1 : 2;

            if (dist[x][y] != unreachable && (dist[currX][currY] + cost) < dist[x][y])
            {
                dist[x][y] = dist[currX][currY] + cost;
                q.insert(make_pair(dist[x][y], make_pair(x, y)));
            }
        }
    }
    return dist;
}

vector<pair<int, int>> GameAlgo::getAllposition(string types, vector<vector<char>> &simpleMap)
{
    vector<pair<int, int>> v;

    for (int i = 0; i < simpleMap.size(); i++)
    {
        for (int j = 0; j < simpleMap[i].size(); j++)
        {
            if (types.find(simpleMap[i][j]) != std::string::npos)
            {
                v.push_back({i, j});
            }
        }
    }

    return v;
}

DIRECTIONS GameAlgo::moveDirection(Unit unit, vector<vector<int>> &distArray, vector<vector<char>> &simpleMap)
{
    int DX[] = {0, 0, 0, -1, 1};
    int DY[] = {0, 1, -1, 0, 0};
    DIRECTIONS D[] = {CENTER, SOUTH, NORTH, WEST, EAST};
    
    int closestDist = unvisited;
    DIRECTIONS closestDirection = CENTER;
    int closestX = -1, closestY = -1;

    for (int i = 0; i < 5; i++)
    {
        int goX = unit.pos.x + DX[i];
        int goY = unit.pos.y + DY[i];

        char goCellvalue = getCell(goX,goY,simpleMap);

        if (goCellvalue !='z' && goCellvalue !='b' && goCellvalue !='*') 
        {
            if (distArray[goX][goY] < closestDist)
            {
                closestDist = distArray[goX][goY];
                closestDirection = D[i];
            }
        }
    }

    return closestDirection;
}

pair<int,int> GameAlgo::getPosition(int x, int y, DIRECTIONS d)
{
    int DX[] = {0, 0, 0, -1, 1};
    int DY[] = {0, 1, -1, 0, 0};
    DIRECTIONS D[] = {CENTER, SOUTH, NORTH, WEST, EAST};

    for(int i=0; i<5; i++)
    {
        if(D[i] == d)
        {
            return {x+DX[i],y+DY[i]};
        }
    }
}

/*
int GameAlgo::countNearbyCell(int x, int y, int withinDistance, string types)
{
    int ret = 0;
    for (int d = 0; d <= withinDistance; d++)
    {
        for (int dx = 0; dx <= d; dx++)
        {
            int dy = d - dx;
            if (types.find(getCell(x + dx, y + dy)) != std::string::npos)
                ret++;
            if (dy != 0 && types.find(getCell(x + dx, y - dy)) != std::string::npos)
                ret++;
            if (dx != 0 && types.find(getCell(x - dx, y + dy)) != std::string::npos)
                ret++;
            if (dx != 0 && dy != 0 && types.find(getCell(x - dx, y - dy)) != std::string::npos)
                ret++;
        }
    }
    return ret;
}
*/
