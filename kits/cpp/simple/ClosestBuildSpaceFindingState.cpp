

string ClosestBuildSpaceFindingState::act()
{
    auto g = GameData::getInstance();
    auto u = this->unit;
    return u->move(GameAlgo::moveDirection(u,g->distfromDots,g->simpleMap));
}

void ClosestBuildSpaceFindingState::prepareAct()
{
    auto g = GameData::getInstance();
    auto u = this->unit;
    if(u->getCargoSpaceLeft()!=0) // somehow lost resource
    {
        u->TransitionTo(new ClosestResourceFindingState());
    }

    if(GameAlgo::getCell(u->pos.x, u->pos.y, g->simpleMap) == '.') // builable
    {
        this->unit->TransitionTo(new BuildCityState());
    }

}