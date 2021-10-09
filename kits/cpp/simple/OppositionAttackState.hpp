#ifndef OPPOSITIONATTACKSTATE_H
#define OPPOSITIONATTACKSTATE_H
#pragma once    

class OppositionAttackState : public UnitState
{
private:
public:
    string act() override;
    void prepareAct() override;
    string stateName() override
    {
        return "OppositionAttackState";
    }
    int getPriority() override;
};
#endif