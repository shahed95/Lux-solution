#include "ClusterMaker.h"

void ClusterMaker::make(vector<vector<char>> _grid, string types)
{
    this->grid = _grid;

    if (grid.size() == 0)
        return;

    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            temp.clear();
            dfs(i, j, types);
            for (auto u : temp)
                belongs.push_back(clusters.size());
            if (temp.size() != 0)
                clusters.push_back(temp);
        }
    }
    random_shuffle(belongs.begin(),belongs.end());
}

void ClusterMaker::dfs(int posX, int posY, string &types)
{
    if (posX < 0 || posX >= grid.size() || posY < 0 || posY >= grid[0].size())
        return;
    if (types.find(grid[posX][posY]) == string::npos)
        return;

    grid[posX][posY] = '$';
    temp.push_back({posX, posY});

    dfs(posX + 1, posY, types);
    dfs(posX - 1, posY, types);
    dfs(posX, posY + 1, types);
    dfs(posX, posY - 1, types);
}

int ClusterMaker::giveaCluster(int i)
{
    return belongs[i%belongs.size()];
}