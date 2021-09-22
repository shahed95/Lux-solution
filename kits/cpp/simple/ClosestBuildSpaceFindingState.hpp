#ifndef CLOSESTBUILDSPACEFINDINGSTATE_H
#define CLOSESTBUILDSPACEFINDINGSTATE_H
#pragma once

class ClosestBuildSpaceFindingState : public UnitState
{
private:
public:
    string act() override;
    void prepare_act() override;
};
#endif