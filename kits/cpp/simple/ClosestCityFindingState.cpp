

string ClosestCityFindingState::act()
{
    auto g = GameData::getInstance();
    auto u = this->unit;
    return u->move(GameAlgo::moveDirection(u,g->distfromCities,g->simpleMap));
}

void ClosestCityFindingState::prepareAct()
{
    // probable cases: in city, no city ... 
    auto g = GameData::getInstance();
    auto u = this->unit;

    if(GameAlgo::getCell(u->pos.x, u->pos.y, g->simpleMap) == 'y') 
    {
        this->unit->TransitionTo(new ClosestResourceFindingState());
    }
}