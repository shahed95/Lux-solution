map<string, Unit> UnitExtraData::backup;

void UnitExtraData::backupUnitExtraData(Player &player)
{
    for (int i = 0; i < player.units.size(); i++)
    {
        Unit unit = player.units[i];
        backup[unit.id] = unit;
    }
}
void UnitExtraData::updateUnitExtraData(Player &player)
{
    GameData *g = GameData::getInstance();
    map<int, int> targeted;

    // 1. restore data
    for (int i = 0; i < g->player.units.size(); i++)
    {
        Unit &unit = g->player.units[i];
        if (backup.count(unit.id) == 0)
            continue;
        unit.TransitionTo(backup[unit.id].getState());
        unit.target = backup[unit.id].target;
        if (unit.getStateName() == "TargetResourceFindingState")
        {
            targeted[unit.target]++;
        }
        else if (unit.getStateName() == "ClosestResourceFindingState")
        {
            auto v = closestClusterOrder(unit.pos.x, unit.pos.y, 99999);
            if (v.size() != 0)
                targeted[v[0]]++;
        }
        unit.prepare_act();
    }

    // 2. initialize data
    for (int i = 0; i < g->player.units.size(); i++)
    {
        Unit &unit = g->player.units[i];
        if (backup.count(unit.id) != 0)
            continue;

        vector<int> order;
        int dayLeft = 30 - (g->gameState.turn % 40);
        int safeDist = (dayLeft / 2) - 6;

        if (g->gameState.turn < 50 && g->distfromOpponent[unit.pos.x][unit.pos.y] <= (safeDist))
        {
            unit.TransitionTo(new OppositionAttackState());
            continue;
        }

        order = closestClusterOrder(unit.pos.x, unit.pos.y, safeDist);

        if (i % 2 == 1 && g->player.cityTileCount > g->opponent.cityTileCount)
            order = largestClusterOrder(unit.pos.x, unit.pos.y, safeDist);

        for (auto best : order)
        {
            if (targeted[best] >= g->resourceClusters[best].cells.size())
                continue;
            if (rand() % 2 == 0)
                continue;
            unit.TransitionTo(new TargetResourceFindingState());
            unit.target = best;
            targeted[best]++;
            break;
        }
        if (unit.target == -1)
        {
            unit.TransitionTo(new ClosestResourceFindingState());
        }

        unit.prepare_act();
    }

    //pull from cluster
    if (g->gameState.turn != 25 && g->gameState.turn != 65)
        return;

    auto order = largestClusterOrder(0, 0, 999999);

    for (int i = 0; i < order.size(); i++)
    {
        auto c = order[i];
        if (targeted[c] != 0)
            continue;
        int distFromClosestUnit = 999999;
        int selected = -1;

        for (int j = 0; j < g->player.units.size(); j++)
        {
            auto &unit = g->player.units[j];

            if (unit.getSpaceUsed() < 50)
                continue;

            auto dist = g->distfromResourceCluster[c][unit.pos.x][unit.pos.y];

            if (dist < distFromClosestUnit)
            {
                distFromClosestUnit = dist;
                selected = j;
            }
        }
        if (selected != -1)
        {
            auto &unit = g->player.units[selected];
            unit.TransitionTo(new TargetResourceFindingState());
            unit.target = c;
            unit.prepare_act();
            targeted[c]++;
            break;
        }
    }
}

vector<int> UnitExtraData::closestClusterOrder(int x, int y, int lim)
{
    GameData *g = GameData::getInstance();
    vector<pair<int, int>> v;

    for (int i = 0; i < g->distfromResourceCluster.size(); i++)
    {
        auto cluster = g->resourceClusters[i];
        if (cluster.takable == 0)
            continue;
        auto distArray = g->distfromResourceCluster[i];
        if (distArray[x][y] > lim)
            continue;
        v.push_back({distArray[x][y], i});
    }

    sort(v.begin(), v.end());
    vector<int> ret;
    for (auto u : v)
        ret.push_back(u.second);
    return ret;
}

vector<int> UnitExtraData::largestClusterOrder(int x, int y, int lim)
{
    GameData *g = GameData::getInstance();
    vector<pair<int, int>> v;

    for (int i = 0; i < g->resourceClusters.size(); i++)
    {
        auto cluster = g->resourceClusters[i];
        if (cluster.takable == 0)
            continue;
        auto distArray = g->distfromResourceCluster[i];
        if (distArray[x][y] > lim)
            continue;
        v.push_back({cluster.cells.size() * -1, i});
    }

    sort(v.begin(), v.end());
    vector<int> ret;
    for (auto u : v)
        ret.push_back(u.second);
    return ret;
}
