#ifndef BUILDCITYSTATE_H
#define BUILDCITYSTATE_H
#pragma once

class BuildCityState : public UnitState
{
private:
public:
    string act() override
    {
        return this->unit->buildCity();
    }
    void prepare_act() override
    {
        // if(this->unit->getCargoSpaceLeft() != 0)
        // {
        //     this->unit->TransitionTo(new ClosestResourceFindingState());
        //     return;
        // }
        
        // if(this->unit->canBuild(GameData::getInstance()->gameMap) == false)
        // {
        //     this->unit->TransitionTo(new ClosestBuildSpaceFindingState());
        // }

    }
};
#endif