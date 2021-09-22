#ifndef BUILDCITYSTATE_H
#define BUILDCITYSTATE_H
#pragma once

class BuildCityState : public UnitState
{
private:
public:
    string act() override;
    void prepare_act() override;
};
#endif