#ifndef INSIDECITYSTATE_H
#define INSIDECITYSTATE_H

	
class InsideCityState : public UnitState
{
	private:

	public:
string act() override;
    void prepareAct() override;
    string stateName() override
    {
        return "ClosestResourceFindingState";
    }

};
#endif