void UnitExtraData::backupUnitExtraData(Player &player)
{
    for (int i = 0; i < player.units.size(); i++)
    {
        Unit unit = player.units[i];
        backup[unit.id] = unit;
    }
}
void UnitExtraData::restoreUnitExtraData(Player &player)
{
    for (int i = 0; i < player.units.size(); i++)
    {
        Unit &unit = player.units[i];
        if(backup.count(unit.id))
            unit.TransitionTo(backup[unit.id].getState());
    }
}

void UnitExtraData::addNewUnitsExtraData(Player &player)
{
    for (int i = 0; i < player.units.size(); i++)
    {
        Unit &unit = player.units[i];
        if(backup.count(unit.id) == 0)
            unit.TransitionTo(new ClosestResourceFindingState());
    }
}