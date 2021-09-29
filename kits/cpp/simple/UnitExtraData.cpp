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
    map<int, int> mp;

    // 1. restore data
    for (int i = 0; i < player.units.size(); i++)
    {
        Unit &unit = player.units[i];
        if (backup.count(unit.id) == 0)
            continue;

        unit.TransitionTo(backup[unit.id].getState());
        unit.target = backup[unit.id].target;
        mp[unit.target]++;
    }

    // 2. initialize data
    for (int i = 0; i < player.units.size(); i++)
    {
        Unit &unit = player.units[i];
        if (backup.count(unit.id) != 0)
            continue;

        unit.TransitionTo(new TargetResourceFindingState());

        GameData *g = GameData::getInstance();
        unit.target = -1;
        vector<pair<int, int>> options;
        for (int j = 0; j < g->resourceClusters.size(); j++)
        {
            if (g->resourceClusters[j].cells.size() == 0 || g->resourceClusters[j].islocked == 1)
                continue;

            if (unit.target == -1)
                unit.target = j;
            int dayleft = 30 - (g->gameState.turn % 40);
            if ((g->distfromResourceCluster[j][unit.pos.x][unit.pos.y] * 2 + 7) >= dayleft)
                continue;
            options.push_back({g->distfromResourceCluster[j].size(), j});
        }
        sort(options.begin(), options.end());
        if (options.size() != 0 && i % 2 == 1)
        {
            unit.target = options.back().second;
            mp[unit.target]++;
        }
        else
            unit.TransitionTo(new ClosestResourceFindingState());
    }
}