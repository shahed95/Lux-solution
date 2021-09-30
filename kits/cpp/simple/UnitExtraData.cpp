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
    }

    // 2. initialize data
    for (int i = 0; i < g->player.units.size(); i++)
    {
        Unit &unit = g->player.units[i];
        if (backup.count(unit.id) != 0)
            continue;

        vector<int> order;
        int dayLeft = 30 - (g->gameState.turn % 40);

        order = closestClusterOrder(unit.pos.x, unit.pos.y, (dayLeft / 2) - 6);
        
        if (i % 2 == 1 && g->player.cityTileCount > g->opponent.cityTileCount)
            order = largestClusterOrder(unit.pos.x, unit.pos.y, (dayLeft / 2) - 6);

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
    }
}

vector<int> UnitExtraData::closestClusterOrder(int x, int y, int lim)
{
    GameData *g = GameData::getInstance();
    vector<pair<int, int>> v;

    for (int i = 0; i < g->distfromResourceCluster.size(); i++)
    {
        auto cluster = g->resourceClusters[i];
        if(cluster.islocked==1) continue;
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
        if(cluster.islocked==1) continue;
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
