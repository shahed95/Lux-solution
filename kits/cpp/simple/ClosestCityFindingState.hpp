#ifndef CLOSESTCITYFINDINGSTATE_H
#define CLOSESTCITYFINDINGSTATE_H
#pragma once

class ClosestCityFindingState  : public UnitState
{
private:
public:
    string act() override;
    void prepareAct() override;
    string stateName() override
    {
        return "ClosestCityFindingState";
    }
    int getPriority() override;
};
#endif