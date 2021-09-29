#ifndef CLUSTER_H
#define CLUSTER_H
	
class Cluster  
{
	private:
	public:
        int islocked;
        vector<pair<int,int>> cells;
        pair<int,int> startpos;
        char type;
		Cluster(vector<vector<char>> simpleMap, char _type, pair<int,int> sourcePos);
        void updateCluster(vector<vector<char>> &simpleMap);
};
#endif