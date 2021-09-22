#ifndef CLOSESTCITYFINDINGSTATE_H
#define CLOSESTCITYFINDINGSTATE_H
#pragma once

class ClosestCityFindingState  : public UnitState
{
	private:

	public:
    string act() override {
        const DIRECTIONS D[] = {CENTER, SOUTH, NORTH, WEST, EAST};
        return this->unit->move(D[rand()%5]);
    }
    void prepare_act() override {
        
    }

};
#endif