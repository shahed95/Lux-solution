#ifndef CLOSESTCITYFINDINGSTATE_H
#define CLOSESTCITYFINDINGSTATE_H
#pragma once

class ClosestCityFindingState  : public UnitState
{
private:
public:
    string act() override;
    void prepare_act() override;

};
#endif