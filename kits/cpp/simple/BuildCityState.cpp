string BuildCityState::act()
{
    auto g = GameData::getInstance();
    auto u = this->unit;
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