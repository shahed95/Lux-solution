#ifndef UNITEXTRADATA_H
#define UNITEXTRADATA_H
#pragma once

class UnitExtraData
{
private:
    map<string, Unit> backup;

public:
    void backupUnitExtraData(Player &player);
    void restoreUnitExtraData(Player &player);
    void addNewUnitsExtraData(Player &player);
};
#endif