#include <iostream>
#include <vector>
#include <stack>
#include <sstream>
#include <string>
using namespace std;

//采用标志位的方法
vector<char> delete1(vector<char> str) {
	vector<char> result = str;
	//flag标志位用于判断是否可以删除元素
	bool flag = true;
	while (flag) {
		flag = false;

		//每次都从数组的最左边开始查找
		int left = 2;
		while (left < result.size()) {
			//找到第一组连续相同的三个元素，将其删除
			//这里要注意break，每次循环都只删除第一组
			if (result[left] == result[left - 1] && result[left] == result[left - 2]) {
				result.erase(result.begin() + left - 2, result.begin() + left + 1);
				flag = true;
				break;
			}
			left++;
		}
	}
	return result;
}

//栈模拟
stack<char> delete2(vector<char>& str) {
	stack<char> st;

	int index = 0;
	while (index < str.size()) {
		st.push(str[index]);

		//如果栈中的元素数量>=3，找到栈顶的三个元素，判断这三个元素是否相等
		//如果相等，就把这三个元素弹出
		//如果不相等，就继续往栈顶压入一个元素，然后继续判断。
		if (st.size() >= 3) {
			int top = st.top();
			st.pop();
			int mid = st.top();
			st.pop();
			int bottom = st.top();

			if (top == mid && top == bottom) {
				st.pop();
			}
			else {
				st.push(mid);
				st.push(top);
			}
		}
		index++;
	}

	//栈模拟完了之后还需要反转
	stack<char> result;
	while (!st.empty()) {
		result.push(st.top());
		st.pop();
	}

	return result;
}

int main() {
	//输入卡片数量
	int n;
	cin >> n;
	//忽略换行
	cin.ignore();
	//输入卡片
	vector<char> str(n);
	string line;
	getline(cin, line);
	istringstream iss(line);
	for (int i = 0; i < n; i++) {
		iss >> str[i];
	}

	vector<char> result2 = delete1(str);
	for (int i = 0; i < result2.size(); i++) {
		cout << result2[i] << " ";
	}
	cout << endl;

	stack<char> result3 = delete2(str);
	while (!result3.empty()) {
		cout << result3.top() << " ";
		result3.pop();
	}

	return 0;
}