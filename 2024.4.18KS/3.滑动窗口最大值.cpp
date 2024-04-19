#include <iostream>
#include <vector>
#include <deque>
using namespace std;

vector<int> maxSlidingWindow1(vector<int>& nums, int k) {
	vector<int> result;
	deque<int> window; // 双端队列，用于存储索引

	for (int i = 0; i < nums.size(); ++i) {
		// 删除队列中已经不在窗口中的元素
		// 一般来说，队尾 - 对头 = k
		// 但是我们要插入的索引i，肯定是要比对尾的值要大
		// 所以 i - 对头 > k，就插入
		// 至于为什么还有一个等于，是因为还有删除的过程
		// 这就导致对头的值可能会比实际的要大
		// 也就是说 队尾 - 对头 >= k

		//为什么window.front() <= i - k
		// i表示当前窗口的右边界
		// k表示窗口内元素的个数
		// 由于边界是从0开始的
		// 右边界 - 左边界 >= k 就需要删除窗口元素
		// 左边界 <= 右边界 - k
		while (!window.empty() && window.front() <= i - k) {
			window.pop_front();
		}

		// 删除队列中比当前元素小的元素 保证对头元素是最大的
		while (!window.empty() && nums[window.back()] <= nums[i]) {
			window.pop_back();
		}

		window.push_back(i);

		// 当窗口完全覆盖时，记录窗口中的最大值
		if (i >= k - 1) {
			result.push_back(nums[window.front()]);
		}
	}

	return result;
}

class MyQueue {
public:
	deque<int> dq;

	// 每次弹出的时候，比较当前要弹出的数值是否等于队列出口元素的数值，如果相等则弹出。
	// 同时pop之前判断队列当前是否为空
	void pop(int value) {
		if (!dq.empty() && value == dq.front()) {
			dq.pop_front();
		}
	}

	//向dq中添加元素
	//在push元素的时候要判断value和队尾元素的关系，如果队尾元素<value，就弹出队尾元素，直至队尾元素>value或dq为空
	//添加元素的关键在于要保证对头的元素是最大的
	void push(int value) {
		while (!dq.empty() && value > dq.back()) {
			dq.pop_back();
		}
		dq.push_back(value);
	}

	//获取对头元素
	int front() {
		return dq.front();
	}
};

vector<int> maxSlidingWindow1(vector<int>& nums, int k) {
	vector<int> result;
	MyQueue que;
	//先将前k个元素添加至que中
	for (int i = 0; i < k; i++) {
		que.push(nums[i]);
	}
	//此时que的对头元素就是最大的
	result.push_back(que.front());

	for (int i = k; i < nums.size(); i++) {
		que.pop(nums[i - k]);//滑动窗口移除最前面元素
		que.push(nums[i]);//向窗口中添加一个元素
		result.push_back(que.front());//获取窗口中的最大值
	}

	return result;
}

int main() {
	int n, k;
	cin >> n >> k;

	vector<int> nums(n);
	for (int i = 0; i < n; ++i) {
		cin >> nums[i];
	}

	vector<int> result = maxSlidingWindow1(nums, k);
	for (int i = 0; i < result.size(); ++i) {
		cout << result[i] << " ";
	}
	cout << endl;

	return 0;
}