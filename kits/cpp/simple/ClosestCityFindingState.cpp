

string ClosestCityFindingState::act()
{
    auto g = GameData::getInstance();
    auto u = this->unit;
    auto dir = GameAlgo::moveDirection(u, g->distfromCities, g->distfromCities, g->simpleMap);
    auto newPos = GameAlgo::getPosition(u->pos.x, u->pos.y, dir);
    g->simpleMap[newPos.first][newPos.second] = 'b';
    return u->move(dir);
}

void ClosestCityFindingState::prepareAct()
{
    auto g = GameData::getInstance();
    auto u = this->unit;
    if (g->player.cityTileCount == 0)
    {
        this->unit->TransitionTo(new ClosestResourceFindingState());
    }
    if (GameAlgo::getCell(u->pos.x, u->pos.y, g->simpleMap) == 'y')
    {
        this->unit->TransitionTo(new ClosestResourceFindingState());
    }
}

int ClosestCityFindingState::getPriority() 
{
    auto g = GameData::getInstance();
    auto u = this->unit;
    return 0;
}