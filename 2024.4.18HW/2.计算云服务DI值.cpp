#include <iostream>
#include <map>
#include <unordered_set>
#include <vector>
#include <string>
#include <set>
#include <queue>
using namespace std;

/*
	第一行输入M和N(M≤100000，N≤1000)，使用空格分隔，M表示代表云服务阈值，N表示接下来有N行问题统计数据；
	接下来输入一个N∗4的矩阵表，行内使用空格分隔，第一列Ai为服务节点，第二列Bi为Ai的父节点，
	如果Ai为云服务则无父节点，此时Bi用∗号表示(Ai和Bi取值为字符串，1≤字符串长度≤5，均由小写英文字母或∗号组成)，
	第三列Ci为问题级别（Ci取值为{0,1}，0表示严重问题，1表示一般问题），第四列Di为该节点该级别的问题数量(Di≤1000)。
	说明：输入保证只出现树的关系，不会出现连通图的情况。

	本题的关键在于构建N叉树
	在输入矩阵表的时候，每输入一行数据的前两个数据就是当前节点及其父节点的名字
	需要判断当前节点及其父节点是否被创建过
	如果没有被创建，就先创建，然后将其加入已经创建的集合中
	如果已经被创建，就需要更新原来节点的数据
	在更新节点数据的时候，我们不仅要更新当前节点的数据，还需要更新在父节点中当前节点的数据

	由于本题可能不止一个根节点，还需要一个集合，用来存储根节点，方便最后遍历
	遍历方式采用前、中、后等方式等可以，由于是N叉树，可能没办法进行前、中、后遍历
	我采用的是层次遍历
*/
//定义一个N叉树的类
class Node {
public:
	string node_name;
	map<string, Node*> childern;
	int my_model_0;
	int my_model_1;

	Node() {}

	Node(string name) {
		this->node_name = name;
		my_model_0 = 0;
		my_model_1 = 0;
	}

	Node(string name, int model_0, int model_1) {
		this->node_name = name;
		my_model_0 = model_0;
		my_model_1 = model_1;
	}

	bool operator==(Node* node1) {
		if (this->node_name == node1->node_name) return true;
		return false;
	}

	bool operator>(Node* node1) {
		if (this->node_name > node1->node_name) return true;
		return false;
	}

	bool operator<(Node* node1) {
		if (this->node_name < node1->node_name) return true;
		return false;
	}
};

int main() {
	int M, N;
	cin >> M >> N;

	map<string, Node*> father;
	map<string, Node*> constructNode;
	for (int i = 0; i < N; i++) {
		string node_name, parent_name;
		int model, price;
		cin >> node_name >> parent_name >> model >> price;

		//判断名字为node_name的节点是否已经被构建
		if (constructNode.find(node_name) != constructNode.end()) {
			//cout << node_name << endl;
			//当前节点已经被构建过，直接修改该节点的数据
			if (model == 0) {
				constructNode[node_name]->my_model_0 = price;
			}
			else {
				constructNode[node_name]->my_model_1 = price;
			}
		}
		else {
			//当前节点没有被构建过
			//cout << node_name << endl;
			Node* node = new Node();
			node->node_name = node_name;
			if (model == 0) {
				node->my_model_0 = price;
			}
			else {
				node->my_model_1 = price;
			}

			//把当前节点插入到已经构建的节点map
			constructNode[node_name] = node;
		}

		//判断当前节点有无父节点
		if (parent_name == "*") {
			//改节点无父节点，是根节点
			father[node_name] = constructNode[node_name];
		}
		else {
			//当前节点有父节点
			//判断当前节点的父节点是否被构建过
			if (constructNode.find(parent_name) != constructNode.end()) {
				//当前节点的父节点已经被构建过
				constructNode[parent_name]->childern[node_name] = constructNode[node_name];
			}
			else {
				//当前节点的父节点没有被构建过
				Node* parentNode = new Node(parent_name);
				parentNode->childern[node_name] = constructNode[node_name];
				constructNode[parent_name] = parentNode;
			}
		}
	}

	int riskServerCount = 0;//记录风险服务器的个数
	for (auto& f : father) {
		Node* root = f.second;
		if (root == NULL) continue;
		queue<Node*> q;
		q.push(root);
		int model0_count = 0;//严重问题数
		int model1_count = 0;//一般问题数
		while (!q.empty()) {
			int size = q.size();
			for (int i = 0; i < size; i++) {
				Node* cur = q.front();
				q.pop();
				cout << cur->node_name << ": (" << cur->my_model_0 << ", " << cur->my_model_1 << ") ";
				model0_count += cur->my_model_0;
				model1_count += cur->my_model_1;
				if (cur->childern.size() > 0) {
					for (auto& c : cur->childern) {
						//cout << " hi " << " ";
						q.push(c.second);
					}
				}
			}
		}
		cout << model0_count << " " << model1_count << endl;
		if (M < (5 * model0_count + 2 * model1_count)) {
			riskServerCount++;
		}
		cout << endl;
	}
	cout << "riskServerCount: " << riskServerCount << endl;
	return 0;
}