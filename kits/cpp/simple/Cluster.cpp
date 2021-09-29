
Cluster::Cluster(vector<vector<char>> simpleMap, char _type, pair<int, int> sourcePos)
{
    int dx[] = {0, 0, -1, 1, 1, 1, -1, -1};
    int dy[] = {1, -1, 0, 0, 1, -1, 1, -1};
    startpos = sourcePos;
    this->type = _type;
    this->islocked = 0;

    queue<pair<int, int>> q;
    q.push(sourcePos);
    simpleMap[sourcePos.first][sourcePos.second] = '$';

    while (!q.empty())
    {
        auto curr = q.front();
        q.pop();
        cells.push_back(curr);

        for (int i = 0; i < 8; i++)
        {
            int x = curr.first + dx[i];
            int y = curr.second + dy[i];
            if (x < 0 || x >= simpleMap.size() || y < 0 || y >= simpleMap[0].size())
                continue;
            if (simpleMap[x][y] == type)
            {
                q.push({x, y});
                simpleMap[x][y] = '$';
            }
        }
    }
}

void Cluster::updateCluster(vector<vector<char>> &simpleMap)
{
    for (int i = 0; i < cells.size(); i++)
    {
        if (simpleMap[cells[i].first][cells[i].second] == '.')
        {
            swap(cells[i], cells[cells.size() - 1]);
            cells.pop_back();
        }
    }
}