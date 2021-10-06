string TargetResourceFindingState::act()
{
    auto g = GameData::getInstance();
    auto u = this->unit;
    auto dir = GameAlgo::moveDirection(u, g->distfromResourceCluster[u->target], g->distfromDotsNeg, g->simpleMap);
    auto newPos = GameAlgo::getPosition(u->pos.x, u->pos.y, dir);
    g->simpleMap[newPos.first][newPos.second] = 'b';
    return u->move(dir);
}

void TargetResourceFindingState::prepareAct()
{
    auto g = GameData::getInstance();
    auto u = this->unit;

    if (g->distfromResourceCluster[u->target][u->pos.x][u->pos.y] <=1 || g->resourceClusters[u->target].cells.size() == 0)
    {
        u->target = -1;
        u->TransitionTo(new ClosestResourceFindingState());
        u->prepare_act();
        return;
    }
}

int TargetResourceFindingState::getPriority() 
{
    auto g = GameData::getInstance();
    auto u = this->unit;
    return 0;
}