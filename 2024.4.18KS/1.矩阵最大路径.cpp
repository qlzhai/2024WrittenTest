#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
using namespace std;
/*
	本题与leetcode上第64题相似，不同之处是64题是求最小路径和，本题是求最大路径和
	leetcode64和本次都是采用动态规划求解的经典题目
*/
int minPathSum(vector<vector<int>>& grid) {
	int m = grid.size();
	int n = grid[0].size();

	// 创建一个与原矩阵相同大小的dp矩阵
	// dp[i][j]表示从grid[0][0]到grid[i][j]的最短路径和
	vector<vector<int>> dp(m, vector<int>(n, 0));

	//初始化dp
	dp[0][0] = grid[0][0];
	//初始化dp的第一列和第一行
	//由于只能往右或者往下，所以dp第一列或第一行就是上一位的累加
	//dp[i][0]只能由dp[i - 1][0]向下移动到达
	for (int i = 1; i < m; i++) {
		dp[i][0] = dp[i - 1][0] + grid[i][0];
	}
	//dp[0][j]只能由dp[0][j - 1]向右移动到达
	for (int j = 1; j < n; j++) {
		dp[0][j] = dp[0][j - 1] + grid[0][j];
	}

	for (int i = 1; i < m; i++) {
		for (int j = 1; j < n; j++) {
			//dp[i][j] = 从[i,j]的左边向右移动到达[i,j]的距离 和 从[i,j]的上边向下移动到达[i,j]的距离 的最小值
			dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
		}
	}

	return dp[m - 1][n - 1];
}

int maxPathSum(vector<vector<int>>& grid) {
	int m = grid.size();
	int n = grid[0].size();

	// 创建一个与原矩阵相同大小的dp矩阵
	vector<vector<int>> dp(m, vector<int>(n, 0));

	// 初始化第一行和第一列
	dp[0][0] = grid[0][0];
	for (int i = 1; i < m; ++i) {
		dp[i][0] = dp[i - 1][0] + grid[i][0];
	}
	for (int j = 1; j < n; ++j) {
		dp[0][j] = dp[0][j - 1] + grid[0][j];
	}

	// 动态规划计算每个位置的最大路径和
	for (int i = 1; i < m; ++i) {
		for (int j = 1; j < n; ++j) {
			//dp[i][j] = 从[i,j]的左边向右移动到达[i,j]的距离 和 从[i,j]的上边向下移动到达[i,j]的距离 的最大值
			dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
		}
	}

	// 返回右下角位置的最大路径和
	return dp[m - 1][n - 1];
}

int main() {
	string line;
	getline(cin, line);

	//解析line
	//列数是line的最后一位
	string sn = "";
	sn += line[line.size() - 1];
	int N = stoi(sn);
	//行数是line的倒数第三位（倒数第二位是 ','）
	string sm = "";
	sm += line[line.size() - 1 - 1 - 1];
	int M = stoi(sm);
	//解析出图的关联矩阵形式
	vector<vector<int>> grid(M, vector<int>(N));
	string str = "";
	//由于line的line[0]='[',所以下标i从1开始
	//line的最后一位是列数 倒数第二位是',' 倒数第三位是行号，倒数第四为是']'，所以下标的最大值是line.size()-5 
	for (int i = 1; i < line.size() - 1 - 1 - 1 - 1; i++) {
		str += line[i];
	}
	//最终获得的str就是关联矩阵的行表示，将其转换成矩阵的形式
	stringstream ss(str);
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			string s;
			getline(ss, s, ',');
			grid[i][j] = stoi(s);
		}
	}

	int maxSum = maxPathSum(grid);
	cout << maxSum << endl;

	return 0;
}