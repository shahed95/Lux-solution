#include "SimplifiedGame.hpp"

SimplifiedGame* SimplifiedGame::simplifiedGame = 0;

SimplifiedGame::SimplifiedGame()
{

}

void SimplifiedGame::updateGame(GameMap &gameMap, Player &player, Player &opponent)
{
    sMap = createEmptyMap(gameMap.height, gameMap.width);
    addResourceInMap(gameMap);
    addCitiesInMap(player, false);
    addCitiesInMap(opponent, true);
    sMapHistory.push_back(sMap);
}

SimplifiedGame *SimplifiedGame::getInstance()
{
    if(simplifiedGame == nullptr)
    {
        simplifiedGame = new SimplifiedGame();
    }
    return simplifiedGame;
}

vector<vector<char>> SimplifiedGame::createEmptyMap(int height, int width)
{
    vector<vector<char>> ret;
    for (int i = 0; i < height; i++)
    {
        vector<char> temp(width, '.');
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
                sMap[x][y] = cell->getResourceType();
        }
    }
}

void SimplifiedGame::addCitiesInMap(Player &player, bool isOpponent)
{
    for (auto city : player.cities)
    {
        for (auto citytiles : city.second.citytiles)
        {
            sMap[citytiles.pos.x][citytiles.pos.y] = isOpponent ? 'z' : 'y';
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

char SimplifiedGame::getCell(int x, int y)
{
    if (isInside(x, y))
        return sMap[x][y];
    else
        return '*';
}

vector<vector<int>> SimplifiedGame::bfsOnMap(vector<pair<int, int>> startingPos, vector<pair<int, int>> unreachablePos)
{
    int dx[] = {0, 0, -1, 1};
    int dy[] = {1, -1, 0, 0};

    vector<vector<int>> dist;

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


vector<vector<int>> SimplifiedGame::dijkstraOnMap(vector<pair<int, int>> startingPos, vector<pair<int, int>> unreachablePos)
{
    int dx[] = {0, 0, -1, 1};
    int dy[] = {1, -1, 0, 0};

    vector<vector<int>> dist;
    vector<vector<bool>> vis;

    for (int i = 0; i < sMap.size(); i++)
    {
        vector<int> temp(sMap[0].size(), unvisited);
        vector<bool> tempbool(sMap[0].size(), false);
        
        dist.push_back(temp);
        vis.push_back(tempbool);
    }

    for (int i = 0; i < unreachablePos.size(); i++)
    {
        dist[unreachablePos[i].first][unreachablePos[i].second] = unreachable;
    }

    multiset<pair<int,pair<int, int>>> q;

    for (int i = 0; i < startingPos.size(); i++)
    {
        q.insert(make_pair(0,startingPos[i]));
        dist[startingPos[i].first][startingPos[i].second] = 0;
    }

    while (!q.empty())
    {
        auto curr = *q.begin();
        q.erase(q.begin());
        int currX = curr.second.first;
        int currY = curr.second.second;

        if(vis[currX][currY] == true) continue;

        vis[currX][currY] = true;

        for (int i = 0; i < 4; i++)
        {
            int x = currX + dx[i];
            int y = currY + dy[i];

            if (!isInside(x, y))
                continue;

            int cost = (getCell(x,y) == 'y') ? 1 : 2;

            if (dist[x][y] != unreachable && (dist[currX][currY] + cost) < dist[x][y])
            {
                dist[x][y] = dist[currX][currY] + cost;
                q.insert(make_pair(dist[x][y],make_pair(x, y)));
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
            if (types.find(sMap[i][j]) != std::string::npos)
            {
                v.push_back({i, j});
            }
        }
    }

    return v;
}

int SimplifiedGame::countNearbyCell(int x, int y, int withinDistance, string types) // Todo : optimize
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