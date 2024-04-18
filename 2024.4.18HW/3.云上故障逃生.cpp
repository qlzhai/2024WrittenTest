#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <sstream>
#include <map>

using namespace std;

/*
第1行n表示云上业务节点数， 2<=n<=10000，节点编号从 0 开始，依次递增；
第2到1+n行表示业务节点间的网络时延矩阵表 delayMatrix，delayMatrix[i][j] 表示节点 i 到节点 j 的通信时延;
1）如果节点 i 和节点 j 之间没有直接相连的边，则 delayMatrix[i][j] 为 -1，第i个节点和它自己也没有边，所以delayMatrix[i][i]=-1
2）节点间有边时延范围为 1<=�����������[�][�]<=10001<=delayMatrix[i][j]<=1000，矩阵元素间使用空格分割
另，输入保证 delayMatrix[i][j] == delayMatrix[j][i]
第2+n行表示各业务节点的剩余容量表 remainingCapacity，其中 remainingCapacity[i] 表示节点 i 的剩余业务容量，业务量的范围1<=�����������������[�]<=1001<=remainingCapacity[i]<=100，数组元素间使用空格分割；
第3+n行表示故障业务节点编号 faultyNode，表示发生故障的节点，取值范围为 0<=����������<=�−10<=faultyNode<=n−1 ；
第4+n行表示受损业务节点需要迁移的业务量, 受损业务量的范围 (0−1000](0−1000] 。
*/

vector<int> dijkstar(vector<vector<int>> matrix, int start) {
	//cost[i]表示节点i到start的最短距离
	vector<int> cost(matrix.size(), INT_MAX);
	//初始化cost
	for (int i = 0; i < matrix.size(); i++) {
		if (matrix[start][i] == -1) continue;
		cost[i] = matrix[start][i];
	}
	cost[start] = INT_MAX;

	vector<bool> visited(matrix.size(), false);
	visited[start] = true;

	int index = 0;//用来记录每次循环到start节点距离最短的节点编号
	int mindistance = 0;
	for (int i = 0; i < matrix.size(); i++) {
		mindistance = INT_MAX;

		//找到巨距离start节点最近的节点
		for (int j = 0; j < matrix.size(); j++) {
			if (j == start) continue;
			if (cost[j] < mindistance && visited[j] == false) {
				index = j;
				mindistance = cost[j];
			}
		}

		visited[index] = true;

		//更新cost
		for (int j = 0; j < matrix.size(); j++) {
			if (matrix[index][j] == -1) continue;
			cost[j] = min(cost[index] + matrix[index][j], cost[j]);
		}
	}

	return cost;
}

int main() {
	int n;
	cin >> n;
	cin.ignore();//接收换行

	//输入邻接矩阵
	vector<vector<int>> matrix(n, vector<int>(n, -1));
	for (int i = 0; i < n; i++) {
		string line;
		getline(cin, line);
		istringstream iss(line);
		for (int j = 0; j < n; j++) {
			iss >> matrix[i][j];
		}
	}

	//输入剩余业务容量表
	vector<int> remainCapacityTable(n, 0);
	string line;
	getline(cin, line);
	istringstream iss(line);
	for (int i = 0; i < n; i++) {
		iss >> remainCapacityTable[i];
	}

	//输入故障节点
	int faultyNode;
	cin >> faultyNode;

	cin.ignore();
	//需要迁移的业务量
	int moveCapacity;
	cin >> moveCapacity;

	//采用dijkstar算法求各个节点到故障节点的最短距离
	vector<int> cost = dijkstar(matrix, faultyNode);

	//采用map集合映射<节点i到故障点的最短距离，节点i>
	//map集合可以将距离排序
	map<int, int> costMap;
	for (int i = 0; i < cost.size(); i++) {
		if (i == faultyNode) continue;
		costMap[cost[i]] = i;
	}

	int sum = 0;
	vector<int> result;
	for (auto& m : costMap) {
		sum += remainCapacityTable[m.second];
		result.push_back(m.second);
		if (sum >= moveCapacity) {
			break;
		}
	}

	for (int i = 0; i < result.size(); i++) {
		cout << result[i] << " ";
	}

	return 0;
}