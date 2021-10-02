#ifndef CLOSESTRESOURCE_H
#define CLOSESTRESOURCE_H
#pragma once

class ClosestResourceFindingState : public UnitState {
private:
public:
    string act() override;
    void prepareAct() override;
    string stateName() override
    {
        return "ClosestResourceFindingState";
    }
    int getPriority() override;
};

#endif
