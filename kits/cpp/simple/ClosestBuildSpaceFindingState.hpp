#ifndef CLOSESTBUILDSPACEFINDINGSTATE_H
#define CLOSESTBUILDSPACEFINDINGSTATE_H
#pragma once

class ClosestBuildSpaceFindingState : public UnitState
{
private:
public:
    string act() override;
    void prepareAct() override;
    string stateName() override
    {
        return "ClosestBuildSpaceFindingState";
    }
    int getPriority() override;
};
#endif