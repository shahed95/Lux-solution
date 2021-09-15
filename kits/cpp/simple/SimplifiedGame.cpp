#include "SimplifiedGame.h"

SimplifiedGame::SimplifiedGame()
{
}
SimplifiedGame::SimplifiedGame(GameMap &gameMap, Player &player, Player &opponent)
{
    createSimpleMap(gameMap);
    addResourceInMap(gameMap);
    addCitiesInMap(player, false);
    addCitiesInMap(opponent, true);
}

void SimplifiedGame::createSimpleMap(GameMap &gameMap)
{
    for (int i = 0; i < gameMap.height; i++)
    {
        vector<pair<char, int>> temp(gameMap.width, {'.', 0});
        sMap.push_back(temp);
    }
}

vector<vector<char>> SimplifiedGame::createEmptyMap(int height, int width)
{
    vector<vector<char>> ret;

    for (int i = 0; i < height; i++)
    {
        vector<char> temp(width, {'.'});
        ret.push_back(temp);
    }
    return ret;
}

void SimplifiedGame::addResourceInMap(GameMap &gameMap)
{
    for (int y = 0; y < gameMap.height; y++)
    {
        for (int x = 0; x < gameMap.width; x++)
        {
            Cell *cell = gameMap.getCell(x, y);
            if (cell->hasResource())
                sMap[x][y] = {cell->getResourceType(), cell->resource.amount}; // 'w', 'c', 'u'
        }
    }
}

void SimplifiedGame::addCitiesInMap(Player &player, bool isOpponent)
{
    int id = 0;
    for (auto city : player.cities)
    {
        id++;
        for (auto citytiles : city.second.citytiles)
        {
            sMap[citytiles.pos.x][citytiles.pos.y] = {isOpponent ? 'z' : 'y', id};
        }
    }
}

bool SimplifiedGame::isInside(int x, int y)
{
    if (sMap.size() == 0)
        return false;
    if (x < 0 || x >= sMap.size() || y < 0 || y >= sMap[0].size())
        return false;
    return true;
}

char SimplifiedGame::getCellType(int x, int y)
{
    if (isInside(x, y))
        return sMap[x][y].first;
    else
        return '*';
}

vector<vector<int>> SimplifiedGame::bfsOnMap(vector<pair<int, int>> startingPos, vector<pair<int, int>> unreachablePos)
{
    int dx[] = {0, 0, -1, 1};
    int dy[] = {1, -1, 0, 0};

    vector<vector<int>> dist;

    int unvisited = 999999;
    int unreachable = 999998;

    for (int i = 0; i < sMap.size(); i++)
    {
        vector<int> temp(sMap[0].size(), unvisited);
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

            if (!isInside(x, y))
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

vector<pair<int, int>> SimplifiedGame::getAllposition(string types)
{
    vector<pair<int, int>> v;

    for (int i = 0; i < sMap.size(); i++)
    {
        for (int j = 0; j < sMap[i].size(); j++)
        {
            if (types.find(sMap[i][j].first) != std::string::npos)
            {
                v.push_back({i, j});
            }
        }
    }

    return v;
}

int SimplifiedGame::closeCityCount(int x, int y, int withinDistance)
{
    if (!isInside(x, y))
        return 0;

    int ret = 0;
    for (int d = 0; d <= withinDistance; d++)
    {
        for (int dx = 0; dx <= d; dx++)
        {
            int dy = d - dx;
            if (getCellType(x + dx, y + dy) == 'y')
                ret++;
            if (dy != 0 && getCellType(x + dx, y - dy) == 'y')
                ret++;
            if (dx != 0 && getCellType(x - dx, y + dy) == 'y')
                ret++;
            if (dx != 0 && dy != 0 && getCellType(x - dx, y - dy) == 'y')
                ret++;
        }
    }
    return ret;
}

vector<pair<int, int>> SimplifiedGame::getAllResourcePosition(int researchPoint)
{
    string resourceString = "w";
    if (researchPoint >= 50)
        resourceString += "c";
    if (researchPoint >= 200)
        resourceString += "u";

    vector<pair<int, int>> v;

    for (int i = 0; i < sMap.size(); i++)
    {
        for (int j = 0; j < sMap[i].size(); j++)
        {
            if (resourceString.find(sMap[i][j].first) != std::string::npos && closeCityCount(i,j,1)==0)
            {
                v.push_back({i, j});
            }
        }
    }
    return v;
}