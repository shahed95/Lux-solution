

string ClosestBuildSpaceFindingState::act()
{
    auto g = GameData::getInstance();
    auto u = this->unit;
    auto dir = GameAlgo::moveDirection(u,g->distfromDots,g->simpleMap);
    auto newPos = GameAlgo::getPosition(u->pos.x,u->pos.y,dir);
    g->simpleMap[newPos.first][newPos.second] = 'b';
    return u->move(dir);
}

void ClosestBuildSpaceFindingState::prepareAct()
{
    auto g = GameData::getInstance();
    auto u = this->unit;
    if(u->getCargoSpaceLeft()!=0) // somehow lost resource
    {
        u->TransitionTo(new ClosestResourceFindingState());
    }
    else if(GameAlgo::getCell(u->pos.x, u->pos.y, g->simpleMap) == '.') // builable
    {
        this->unit->TransitionTo(new BuildCityState());
    }

}