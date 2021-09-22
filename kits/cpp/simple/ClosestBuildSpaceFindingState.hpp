#ifndef CLOSESTBUILDSPACEFINDINGSTATE_H
#define CLOSESTBUILDSPACEFINDINGSTATE_H
#pragma once

class ClosestBuildSpaceFindingState : public UnitState
{
private:
public:
    string act() override
    {
        const DIRECTIONS D[] = {CENTER, SOUTH, NORTH, WEST, EAST};
        return this->unit->move(D[rand() % 5]);
    }
    void prepare_act() override
    {
        if(this->unit->getCargoSpaceLeft() != 0)
        {
            this->unit->TransitionTo(new ClosestResourceFindingState());
        }
        GameData *d = GameData::getInstance();
        if(this->unit->canBuild(GameData::getInstance()->gameMap) == true)
        {
            this->unit->TransitionTo(new BuildCityState());
        }
    }
};
#endif