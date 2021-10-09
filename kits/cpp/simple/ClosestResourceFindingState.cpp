
string ClosestResourceFindingState::act()
{
    auto g = GameData::getInstance();
    auto u = this->unit;
    auto dir = GameAlgo::moveDirection(u, g->distfromResource, g->distfromResource, g->simpleMap);
    auto newPos = GameAlgo::getPosition(u->pos.x, u->pos.y, dir);
    int dayleft = 30 - (g->gameState.turn % 40);

    if (dayleft <= 2 && g->simpleMap[newPos.first][newPos.second] != 'y' && g->simpleMap[u->pos.x][u->pos.y] == 'y')
    {
        return u->move(DIRECTIONS::CENTER);
    }

    if (g->simpleMap[newPos.first][newPos.second] != 'y')
        g->simpleMap[newPos.first][newPos.second] = 'b';
    return u->move(dir);
}

void ClosestResourceFindingState::prepareAct()
{
    auto g = GameData::getInstance();
    auto u = this->unit;

    int dayleft = 30 - (g->gameState.turn % 40);

    if (dayleft <= 2 && g->distfromCities[u->pos.x][u->pos.y] <= 2)
    {
        u->TransitionTo(new ClosestCityFindingState());
        return;
    }

    if (u->getCargoSpaceLeft() == 0) // success
    {
        if (GameAlgo::getCell(u->pos.x, u->pos.y, g->simpleMap) == '.')
        {
            u->TransitionTo(new BuildCityState());
        }
        else
        {
            u->TransitionTo(new ClosestBuildSpaceFindingState());
        }
    }
}

int ClosestResourceFindingState::getPriority()
{
    auto g = GameData::getInstance();
    auto u = this->unit;
    return 0;
}