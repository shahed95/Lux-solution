#ifndef UNITEXTRADATA_H
#define UNITEXTRADATA_H
#pragma once

class UnitExtraData
{
private:
    

public:
    static map<string, Unit> backup;
    static void backupUnitExtraData(Player &player);
    static void updateUnitExtraData(Player &player);
};
#endif