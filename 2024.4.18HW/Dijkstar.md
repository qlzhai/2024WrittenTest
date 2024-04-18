# Dijkstar

leetcode题目对应：743[网络延迟时间](https://leetcode.cn/problems/network-delay-time/description/)、1334[阈值距离内邻居最少的城市 ](https://leetcode.cn/problems/find-the-city-with-the-smallest-number-of-neighbors-at-a-threshold-distance/description/)、1368[使网格图至少有一条有效路径的最小代价 ](https://leetcode.cn/problems/minimum-cost-to-make-at-least-one-valid-path-in-a-grid/description/)

## 图的输入方式为邻接矩阵

### 算法流程

`vector<int> visited(edges.size(), 0)`;用来揭露已经访问过的节点

`vector<int> cost(edges.size(), INT_MAX)`;cost[i]表示节点i距离节点0的最短距离

1.利用邻接矩阵初始化cost

2.初始化`cost[0]=0`，0节点距离0节点的最短距离为0

3.从cost中寻找离0节点距离最短的节点index

4.从index节点出发，找到与index节点相邻的节点

5.判断矩阵更新cost

6.返回步骤3

### C++代码

以下代码为饿了么某次秋招第一题[题目连接](https://www.nowcoder.com/exam/test/79503908/detail?pid=55238116&examPageSource=Company&testCallback=https%3A%2F%2Fwww.nowcoder.com%2Fexam%2Fcompany%3FcurrentTab%3Drecommand%26jobId%3D100%26selectStatus%3D0%26tagIds%3D899&testclass=软件开发)

```c++
#include <iostream>
#include <vector>
#include <limits.h> //INT_MAX头文件
using namespace std;

void dijkstarMatrix(vector<vector<int>>& edges, vector<int>& cost) {
	vector<int> visited(edges.size(), 0);//用来记录已经访问过的节点

	cost[0] = 0;//起点到其本身的最短距离是0
	visited[0] = 1;//初始化已经遍历过0节点
	//初始化cost
	for (int i = 0; i < edges.size(); i++) {
		if (edges[0][i] != 0) cost[i] = edges[0][i];
	}

	int index = 0;//用来记录到0节点的最短距离的节点
	int minDistance = 0;
	for (int i = 0; i < edges.size(); i++) {
		//找到离0节点的最短距离的节点
		minDistance = INT_MAX;
		for (int j = 1; j < edges.size(); j++) {
			if (visited[j] == 0 && minDistance > cost[j]) {
				minDistance = cost[j];
				index = j;
			}
		}

		visited[index] = 1;

		//更新cost
		for (int j = 1; j < edges.size(); j++) {
			if (edges[index][j] < INT_MAX) {
				if (cost[j] > cost[index] + edges[index][j]) {
					cost[j] = cost[index] + edges[index][j];
				}
			}
		}
	}
}

int main() {
	int n, m, q;//节点数 道路数 送外卖次数
	cin >> n >> m >> q;

	vector<vector<int>> nums(n, vector<int>(n, INT_MAX));
	int u, v, w;
	for (int i = 0; i < m; i++) {
		cin >> u >> v >> w;
		nums[u - 1][v - 1] = nums[v - 1][u - 1] = w;
	}

	vector<int> terminal(q, 0);
	for (int i = 0; i < q; i++) {
		cin >> terminal[i];
	}
	vector<int> cost(nums.size(), INT_MAX);
	dijkstarMatrix(nums, cost);

	int result = 0;
	for (int i = 0; i < q; i++) {
		//cout << cost[terminal[i] - 1] << endl;
		result += cost[terminal[i] - 1] * 2;
	}
	cout << result << endl;
	system("pause");
	return 0;
}
```

## 图的输入方式为邻接表

在某些情况下，邻接矩阵的规模会很大，导致内存浪费等。

### 邻接表

首先定义一个结构体**Edge**如下，**Edge**中有两个变量，分别为**to**和**weight**。其中**to**表示与当前节点相连的节点，**weight**表示这条边的权重。

```c++
struct Edge {
	int to;
	int weight;
	Edge(int t, int w) : to(t), weight(w) {}
};
```

### 采用优先队列`priority_queue`优化

- `priority_queue`[C++ 优先队列 ](https://blog.csdn.net/weixin_57761086/article/details/126802156)优先级队列，也是一种容器适配器。内部采用了**堆**这样的数据结构，保证了第一个元素总是整个优先队列中**最大的**（或**最小的**元素）。
- `priority_queue`默认使用vector作为底层容器，在vector上使用堆算法将vector构造成堆的结构
- 默认情况下`priority_queue`是大堆

优先队列其实就是堆，堆中元素都是有固定大小关系的

### 大堆less

每个节点的值都不大于它的双亲节点，堆顶元素是最大的

### 小堆greater

每个节点的值都不小于它的双亲节点，堆顶元素是最小的

### C++代码

以下代码为饿了么某次秋招第一题[题目连接](https://www.nowcoder.com/exam/test/79503908/detail?pid=55238116&examPageSource=Company&testCallback=https%3A%2F%2Fwww.nowcoder.com%2Fexam%2Fcompany%3FcurrentTab%3Drecommand%26jobId%3D100%26selectStatus%3D0%26tagIds%3D899&testclass=软件开发)

```c++
#include <iostream>
#include <vector>
#include <queue>
#include <limits.h>
using namespace std;

struct Edge {
	int to;
	int weight;
	Edge(int t, int w) : to(t), weight(w) {}
};

void dijkstra(vector<vector<Edge>>& graph, vector<int>& cost, int start) {
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;//采用优先队列，pair<当前节点距离0节点的最短距离, 当前节点>
	pq.push({ 0, start });
	cost[start] = 0;

	while (!pq.empty()) {
		int u = pq.top().second;//当前离起点距离最短的节点
		int currentCost = pq.top().first;//当前离起始节点最短的节点的距离
		pq.pop();

		if (currentCost > cost[u]) continue;

		for (const Edge& e : graph[u]) {
			int v = e.to;
			int weight = e.weight;
			if (cost[v] > cost[u] + weight) {
				cost[v] = cost[u] + weight;
				pq.push({ cost[v], v });
			}
		}
	}
}

int main() {
	int n, m, q;
	cin >> n >> m >> q;

	vector<vector<Edge>> graph(n);
	int u, v, w;
	for (int i = 0; i < m; i++) {
		cin >> u >> v >> w;
		graph[u - 1].emplace_back(v - 1, w);
		graph[v - 1].emplace_back(u - 1, w);
	}

	vector<int> terminal(q);
	for (int i = 0; i < q; i++) {
		cin >> terminal[i];
		terminal[i]--; // 将节点编号转换为0-based索引
	}

	vector<int> cost(n, INT_MAX);
	dijkstra(graph, cost, 0);

	long long result = 0;
	for (int i = 0; i < q; i++) {
		result += cost[terminal[i]] * 2; // 计算送外卖的总路程
	}
	cout << result << endl;

	return 0;
}
```

## leetcode题目

### 743[网络延迟时间](https://leetcode.cn/problems/network-delay-time/description/)

```c++
#include <iostream>
#include <vector>
#include <queue>
#include <limits.h>

using namespace std;

class Solution {
public:
	vector<int> dijkstar(vector<vector<int>>& graph, int n, int k) {
		vector<bool> visited(n, false);//用来记录节点是否被访问过
		vector<int> cost(n, INT_MAX); //记录当前节点距离节点k的最短距离

		//初始化
		for (int i = 0; i < n; i++) {
			if (graph[k][i] >= 0) {
				cost[i] = graph[k][i];
			}
		}

		cost[k] = 0;
		visited[k] = true;

		//用来记录当前循环中到k的最短距离的节点及其距离
		int index = 0;
		int minDistance = 0;

		for (int i = 0; i < n; i++) {
			minDistance = INT_MAX;
			for (int j = 0; j < n; j++) {
				if (j == k) continue;
				if (visited[j] == false && minDistance > cost[j]) {
					index = j;
					minDistance = cost[j];
				}
			}

			visited[index] = true;

			//更新cost
			for (int j = 0; j < n; j++) {
				if (j == k) continue;
				if (graph[index][j] < INT_MAX) {
					if (cost[j] > cost[index] + graph[index][j]) {
						cost[j] = cost[index] + graph[index][j];
					}
				}
			}
		}
		return cost;
	}

	int networkDelayTime(vector<vector<int>>& times, int n, int k) {
		vector<vector<int>> graph(n, vector<int>(n, INT_MAX));
		for (int i = 0; i < times.size(); i++) {
			graph[times[i][0] - 1][times[i][1] - 1] = times[i][2];
		}

		vector<int> cost = dijkstar(graph, n, k - 1);

		int maxDistance = 0;
		for (int i = 0; i < n; i++) {
			cout << cost[i] << endl;
			if (cost[i] > maxDistance) {
				maxDistance = cost[i];
			}
		}
		if (maxDistance == INT_MAX) return -1;
		return maxDistance;
	}
};

```

### 1334[阈值距离内邻居最少的城市 ](https://leetcode.cn/problems/find-the-city-with-the-smallest-number-of-neighbors-at-a-threshold-distance/description/)

```c++
#include <iostream>
#include <vector>

using namespace std;

struct Edge {
	int to;
	int weight;
	Edge(int v, int w) :to(v), weight(w) {};
};

int dijkstar(int n, vector<vector<int>>& edges, int distanceThreshold, int start) {
	int result = 0;
	vector<bool> visited(n, false);
	vector<int> cost(n, INT_MAX);

	//初始化cost
	for (int i = 0; i < n; i++) {
		cost[i] = edges[start][i];
	}

	//初始化完成之后，还要判断start节点是否可以到达其他节点
	int isYes = false;
	for (int i = 0; i < n; i++) {
		if (cost[i] != INT_MAX) {
			isYes = true;
		}
	}
	if (isYes == false) return 0;

	cost[start] = 0;
	visited[start] = true;

	int index = 0;
	int minDistance = 0;
	for (int i = 0; i < n; i++) {
		//找到距离start节点最近的节点
		minDistance = INT_MAX;
		for (int j = 0; j < n; j++) {
			if (j == start) continue;
			if (visited[j] == false && cost[j] < minDistance) {
				index = j;
				minDistance = cost[j];
			}
		}
		visited[index] = true;

		//更新cost
		for (int j = 0; j < n; j++) {
			if (j == start) continue;
			if (edges[index][j] < INT_MAX) {
				if (cost[j] > cost[index] + edges[index][j]) {
					cost[j] = cost[index] + edges[index][j];
				}
			}
		}
	}
	for (int i = 0; i < cost.size(); i++) {
		if (cost[i] <= distanceThreshold && cost[i] != 0) result++;
	}
	return result;
}

int findTheCity(int n, vector<vector<int>>& edges, int distanceThreshold) {
	//因为边为邻接表的形式，将其转换为邻接矩阵的形式
	vector<vector<int>> edgeMatrix(n, vector<int>(n, INT_MAX));
	for (int i = 0; i < edges.size(); i++) {
		edgeMatrix[edges[i][0]][edges[i][1]] = edges[i][2];
		edgeMatrix[edges[i][1]][edges[i][0]] = edges[i][2];
	}

	/*for (int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++){
			cout <<edgeMatrix[i][j] << " ";
		}
		cout <<endl;
	}*/

	int minResult = INT_MAX;
	int index = 0;
	for (int i = 0; i < n; i++) {
		int result = dijkstar(n, edgeMatrix, distanceThreshold, i);

		cout << result << endl;
		if (minResult >= result) {
			//cout << minResult << endl;
			minResult = result;
			index = i;
		}
	}
	return index;
}
```

### 1368[使网格图至少有一条有效路径的最小代价 ](https://leetcode.cn/problems/minimum-cost-to-make-at-least-one-valid-path-in-a-grid/description/)

