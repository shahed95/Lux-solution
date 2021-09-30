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
            simpleMap[citytiles.pos.x][citytiles.pos.y] = 'y';
    }

    for (auto city : opponent.cities)
    {
        for (auto citytiles : city.second.citytiles)
            simpleMap[citytiles.pos.x][citytiles.pos.y] = 'z';
    }

    for (auto &unit : player.units)
    {
        if (!unit.canAct() && simpleMap[unit.pos.x][unit.pos.y] == '.')
            simpleMap[unit.pos.x][unit.pos.y] = 'b';
    }

    for (auto &unit : opponent.units)
    {
        if (!unit.canAct() && simpleMap[unit.pos.x][unit.pos.y] == '.')
            simpleMap[unit.pos.x][unit.pos.y] = 'b';
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

vector<vector<int>> GameAlgo::createDistanceArray(string sources, string blocks, vector<vector<char>> &simpleMap)
{
    return bfsOnMap(getAllposition(sources, simpleMap), getAllposition(blocks, simpleMap), simpleMap);
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

DIRECTIONS GameAlgo::moveDirection(Unit *unit, vector<vector<int>> &distArray, vector<vector<char>> &simpleMap)
{
    int DX[] = {0, 0, 0, -1, 1};
    int DY[] = {0, 1, -1, 0, 0};
    DIRECTIONS D[] = {CENTER, SOUTH, NORTH, WEST, EAST};

    int closestDist = unvisited + 10;
    DIRECTIONS closestDirection = CENTER;
    int closestX = 0, closestY = 0;

    vector<int> options;
    for (int i = 0; i < 5; i++)
        options.push_back(i);
    random_shuffle(options.begin(), options.end());
    auto g = GameData::getInstance();
    for (auto i : options)
    {
        int goX = unit->pos.x + DX[i];
        int goY = unit->pos.y + DY[i];

        char goCellvalue = getCell(goX, goY, simpleMap);

        if (goCellvalue != 'z' && goCellvalue != 'b' && goCellvalue != '*')
        {
            if (distArray[goX][goY] < closestDist)
            {
                closestDist = distArray[goX][goY];
                closestDirection = D[i];
                closestX = goX;
                closestY = goY;
            }
            else if (distArray[goX][goY] == closestDist && g->distfromResource[goX][goY] < g->distfromResource[closestX][closestY])
            {
                closestDist = distArray[goX][goY];
                closestDirection = D[i];
                closestX = goX;
                closestY = goY;
            }
        }
    }

    return closestDirection;
}

pair<int, int> GameAlgo::getPosition(int x, int y, DIRECTIONS d)
{
    int DX[] = {0, 0, 0, -1, 1};
    int DY[] = {0, 1, -1, 0, 0};
    DIRECTIONS D[] = {CENTER, SOUTH, NORTH, WEST, EAST};

    for (int i = 0; i < 5; i++)
    {
        if (D[i] == d)
        {
            return {x + DX[i], y + DY[i]};
        }
    }
}

vector<vector<int>> GameAlgo::initDistfromCities(vector<vector<char>> &simpleMap)
{
    return createDistanceArray("y", "zb", simpleMap);
}

vector<vector<int>> GameAlgo::initDistfromDots1(vector<vector<char>> &simpleMap)
{
    return createDistanceArray(".", "yzb", simpleMap);
}

vector<vector<int>> GameAlgo::initDistfromResource1(vector<vector<char>> &simpleMap, string withResource, Player &player, Player &opponent)
{
    return createDistanceArray(withResource, "zb", simpleMap);
}

vector<vector<int>> GameAlgo::initDistfromResource2(vector<vector<char>> &simpleMap, string withResource, Player &player, Player &opponent)
{
    auto tempMap = simpleMap;
    auto oppMap = tempMap;
    auto myMap = tempMap;

    for (auto u : opponent.units)
        oppMap[u.pos.x][u.pos.y] = 'o';

    for (auto u : player.units)
        myMap[u.pos.x][u.pos.y] = 'm';

    auto distFromOpponent = createDistanceArray("o", "yb", oppMap);
    auto distFromMyUnit = createDistanceArray("m", "zb", myMap);

    for (int i = 1; i < tempMap.size() - 1; i++)
    {
        for (int j = 1; j < tempMap.size() - 1; j++)
        {
            if (distFromOpponent[i][j] < 10 && player.cities.size() >= 2)
                continue;
            if (tempMap[i][j] != 'c' && tempMap[i][j] != 'u' && tempMap[i][j] != 'w')
                continue;

            int citycells = 0;
            map<char, int> cnt;
            pair<int, int> dir[] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};

            for (auto u : dir)
                cnt[tempMap[u.first + i][u.second + j]]++;

            if (cnt['y'] >= 2)
            {
                tempMap[i][j] = 'b';
                if (tempMap[i - 1][j] != 'y')
                    tempMap[i - 1][j] = 'b';
                if (tempMap[i + 1][j] != 'y')
                    tempMap[i + 1][j] = 'b';
                if (tempMap[i][j - 1] != 'y')
                    tempMap[i][j - 1] = 'b';
                if (tempMap[i][j + 1] != 'y')
                    tempMap[i][j + 1] = 'b';
            }
        }
    }

    return createDistanceArray(withResource, "zyb", tempMap);
}