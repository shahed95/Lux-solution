#ifndef CLUSTER_MAKER
#define CLUSTER_MAKER

#include "lux/kit.hpp"
#include "lux/define.cpp"
#include <string.h>
#include <vector>
#include <queue>
#include <set>
#include <stdio.h>
using namespace std;
using namespace lux;

class ClusterMaker
{
public:
    vector<int> belongs;
    vector<vector<char>> grid;
    vector<pair<int,int>> temp;
    vector<vector<pair<int, int>>> clusters;
    void make(vector<vector<char>> grid, string types);
    void dfs(int posX, int posY, string &types);
    int giveaCluster(int i);
};

#endif