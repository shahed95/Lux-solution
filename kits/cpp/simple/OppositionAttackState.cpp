string OppositionAttackState::act()
{
    auto g = GameData::getInstance();
    auto u = this->unit;
    auto dir = GameAlgo::moveDirection(u, g->distfromOpponent, g->distfromOpponent, g->simpleMap);
    auto newPos = GameAlgo::getPosition(u->pos.x, u->pos.y, dir);
    g->simpleMap[newPos.first][newPos.second] = 'b';
    return u->move(dir);
}

void OppositionAttackState::prepareAct()
{
    auto g = GameData::getInstance();
    auto u = this->unit;

    if (g->distfromOpponent[u->pos.x][u->pos.y] <=2)
    {
        u->TransitionTo(new ClosestResourceFindingState());
        u->prepare_act();
        return;
    }
}

int OppositionAttackState::getPriority() 
{
    auto g = GameData::getInstance();
    auto u = this->unit;
    return 0;
}