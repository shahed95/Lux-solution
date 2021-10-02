#ifndef TARGETRESOURCEFINDINGSTATE_H
#define TARGETRESOURCEFINDINGSTATE_H
#pragma once

class TargetResourceFindingState : public UnitState
{
private:
public:
    string act() override;
    void prepareAct() override;
    string stateName() override
    {
        return "TargetResourceFindingState";
    }
    int getPriority() override;
};
#endif