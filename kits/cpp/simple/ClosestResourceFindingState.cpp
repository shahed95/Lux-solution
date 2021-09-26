
string ClosestResourceFindingState::act()
{
    auto g = GameData::getInstance();
    auto u = this->unit;
    auto dir = GameAlgo::moveDirection(u,g->distfromResource,g->simpleMap);
    auto newPos = GameAlgo::getPosition(u->pos.x,u->pos.y,dir);
    g->simpleMap[newPos.first][newPos.second] = 'b';
    return u->move(dir);
}

void ClosestResourceFindingState::prepareAct()
{
    auto g = GameData::getInstance();
    auto u = this->unit;

    if (u->getCargoSpaceLeft() == 0) // success
    {
        if (rand() % 2 == 0 && g->player.units.size() > 4)
        {
            u->TransitionTo(new ClosestCityFindingState());
        }
        else if (GameAlgo::getCell(u->pos.x, u->pos.y, g->simpleMap) == '.')
        {
            u->TransitionTo(new BuildCityState());
        }
        else
        {
            u->TransitionTo(new ClosestBuildSpaceFindingState());
        }
    }
}