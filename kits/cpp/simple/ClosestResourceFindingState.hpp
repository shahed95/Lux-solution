#ifndef CLOSESTRESOURCE_H
#define CLOSESTRESOURCE_H
#pragma once

class ClosestResourceFindingState : public UnitState {
 public:
    ClosestResourceFindingState()
    {

    }
    string act() override {
        const DIRECTIONS D[] = {CENTER, SOUTH, NORTH, WEST, EAST};
        return this->unit->move(D[rand()%5]);
    }
    void prepare_act() override {

    }
};

#endif
