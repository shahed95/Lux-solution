string BuildCityState::act()
{
    auto g = GameData::getInstance();
    auto u = this->unit;
    g->simpleMap[u->pos.x][u->pos.y] = 'b';
    if (u->canBuild(g->gameMap))
        return u->buildCity();
    return u->move(DIRECTIONS::CENTER);
}

void BuildCityState::prepareAct()
{
    auto g = GameData::getInstance();
    auto u = this->unit;
    if (u->getCargoSpaceLeft() != 0)
        u->TransitionTo(new ClosestResourceFindingState());
}

int BuildCityState::getPriority() 
{
    return -1000000;
}