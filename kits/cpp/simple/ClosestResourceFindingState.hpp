#ifndef CLOSESTRESOURCE_H
#define CLOSESTRESOURCE_H
#pragma once

class ClosestResourceFindingState : public UnitState {
private:
public:
    string act() override;
    void prepare_act() override;
};

#endif
