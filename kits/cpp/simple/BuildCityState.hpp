#ifndef BUILDCITYSTATE_H
#define BUILDCITYSTATE_H
#pragma once

class BuildCityState : public UnitState
{
private:
public:
    string act() override;
    void prepareAct() override;
    string stateName() override
    {
        return "BuildCityState";
    }
    int getPriority() override;
};
#endif