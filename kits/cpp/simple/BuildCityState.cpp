string BuildCityState::act()
{
    return this->unit->buildCity();
}

void BuildCityState::prepareAct()
{
    if(this->unit->getCargoSpaceLeft()!=0) // build done
    {
        this->unit->TransitionTo(new ClosestResourceFindingState());
    }
}