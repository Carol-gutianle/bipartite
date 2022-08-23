#pragma once
#pragma warning(disable:6385)
#include"global.h"
#include"Vertex.h"
#include"Edge.h"
class Graph
{
private:
	int* val;
	int* col_index;
	int* col_help;
	int* row_index;
	int numVertex;
	int nU;
	int nV;
	int numEdge;
	Edge* weight_arr;
public:
	/*默认行是U集合*/
	Graph(int nu, int nv, int m) {
		nU = nu;
		nV = nv;
		numVertex = nu  + nv;
		numEdge = m;
		val = new int[numEdge];
		col_index = new int[numEdge];
		col_help = new int[numEdge];
		row_index = new int[nu+1];
		weight_arr = new Edge [numEdge];
		/*初始化数组*/
		fill(val, val + numEdge, 0);
		fill(col_index, col_index + numEdge, 0);
		fill(col_help, col_help + numEdge, 0);
		fill(row_index, row_index + nU + 1, 0);
		for (int i = 0; i < numEdge; i++) {
			weight_arr[i].setWeight(0.0);
			weight_arr[i].setFrom(-1);
			weight_arr[i].setTo(-1);
		}
	}
	/*创建图*/
	/*
	* nu nv m
	* 3 3 6
	* u v w
	* 0 0 1
	* 0 1 1
	* 1 0 1
	* 1 2 1
	* 2 1 1
	* 2 2 1
	*/
	void createGraph() {
		/*读测试文件*/
		string filename = "./thwikiquote.txt";
		ifstream ifs(filename);
		if (ifs.is_open()) {
			int u, v, w;
			int i = 0;
			while (!ifs.eof()) {
				ifs >> u >> v >> w;
				row_index[u+1] ++;
				col_index[i] = v;
				col_help[i] = u;
				val[i] = w;
				i++;
			}
			/*关闭文件*/
			ifs.close();
			/*调整row_index*/
			for (int j = 1; j < nU + 1; j++) {
				row_index[j] += row_index[j - 1];
			}
		}
		else {
			cout << "Cannot open test file!" << endl;
		}
	}
	/*打印*/
	void printCSR() {
		printf("U节点的个数为：%d\n", nU);
		printf("V节点的个数为: %d\n", nV);
		printf("Val矩阵为: \n");
		for (int i = 0; i < numEdge; i++) {
			printf("%d ", val[i]);
		}
		printf("\n");
		printf("col_index矩阵为：\n");
		for (int i = 0; i < numEdge; i++) {
			printf("%d ", col_index[i]);
		}
		printf("\n");
		printf("col_help矩阵为：\n");
		for (int i = 0; i < numEdge; i++) {
			printf("%d ", col_help[i]);
		}
		printf("\n");
		printf("row_index矩阵为：\n");
		for (int i = 0; i < nU + 1; i++) {
			printf("%d ", row_index[i]);
		}
		printf("\n");
	}
	/*广度优先搜索*/
	void bfs(int s) {
		queue<int>q;
		vector<Edge>edge;
		stack<Vertex>stk;
		Vertex* v = new Vertex[numVertex];
		/*设置索引*/ 
		for (int i = 0; i < nU; i++) {
			v[i].setIdx(i);
			v[i].setU();
		}
		for (int i = 0; i < nV; i++) {
			v[i + nU].setIdx(i);
		}
		/*打印v数组*/
		if (!DEBUG) {
			printf("V数组为: \n");
			for (int i = 0; i < numVertex; i++) {
				printf("idx:%d \n", v[i].getIdx());
			}
		}
		q.push(s);
		v[s].setAssigned();
		/*默认从U侧开始*/
		while (!q.empty()) {
			Vertex t = v[q.front()];
			stk.push(t);
			int currIdx = t.getIdx();
			/*打印信息*/
			if (!DEBUG) {
				printf("id:%d ", t.getIdx());
			}
			q.pop();
			/*放入邻居节点*/
			/*如果是U侧节点*/
			bool hasNeighbor = false;
			if (t.getIsU()) {
				/*获取邻居节点*/
				int offset = row_index[currIdx];
				int nNeighbor = row_index[currIdx + 1] - row_index[currIdx];
				for (int i = 0; i < nNeighbor; i++) {
					if (!v[nU + col_index[offset + i]].getAssigned()) {
						v[nU + col_index[offset + i]].setAssigned();
						v[nU + col_index[offset + i]].setDistance(t.getDistance()+1);
						v[nU + col_index[offset + i]].setWeight(t.getWeight());
						q.push(nU + col_index[offset + i]);
						/*打印信息*/
						if (DEBUG) {
							printf("\n");
							printf("U侧的%d新赋值V侧的%d  邻居的d为%d w为%d", currIdx, v[nU + col_index[offset + i]].getIdx(), v[nU + col_index[offset + i]].getDistance(), v[nU + col_index[offset + i]].getWeight());
						}
						Edge edge_new(currIdx, nU + col_index[offset + i], 1);
						edge.push_back(edge_new);
						hasNeighbor = true;
					}
					else {
						/*判断邻居节点是否是下一节点*/
						if (v[nU + col_index[offset + i]].getDistance()  == t.getDistance() + 1) {
							v[nU + col_index[offset + i]].setWeight(v[nU + col_index[offset + i]].getWeight() + 1);
							/*打印信息*/
							if (DEBUG) {
								printf("\n");
								printf("U侧的%d再赋值V侧的%d  邻居的d为%d w为%d", currIdx, v[nU + col_index[offset + i]].getIdx(), v[nU + col_index[offset + i]].getDistance(), v[nU + col_index[offset + i]].getWeight());
							}
							Edge edge_new(currIdx, nU + col_index[offset + i], 1);
							edge.push_back(edge_new);
							hasNeighbor = true;
						}
					}
				}
			}
			/*如果是V侧节点*/
			else {
				/*遍历col_index*/
				for (int i = 0; i < numVertex; i++) {
					if (col_index[i] == currIdx) {
						if (!v[col_help[i]].getAssigned()) {
							v[col_help[i]].setAssigned();
							v[col_help[i]].setDistance(t.getDistance() + 1);
							v[col_help[i]].setWeight(t.getWeight());
							q.push(col_help[i]);
							if (DEBUG) {
								printf("\n");
								printf("V侧%d新赋值%d 邻居的d为%d 节点的d为%d", col_index[i], col_help[i], v[col_help[i]].getDistance(), v[col_index[i]].getDistance());
							}
							Edge edge_new(currIdx + nU, col_help[i], 1);
							edge.push_back(edge_new);
							hasNeighbor = true;
						}
						else {
							if (v[col_help[i]].getDistance() == t.getDistance() + 1) {
								v[col_help[i]].setWeight(v[col_help[i]].getWeight() + 1);
								if (DEBUG) {
									printf("\n");
									printf("V侧%d再赋值%d 邻居的d为%d 节点的d为%d", col_index[i], col_help[i], v[col_help[i]].getDistance(), v[col_index[i]].getDistance());
								}
								Edge edge_new(currIdx + nU, col_help[i], 1);
								edge.push_back(edge_new);
								hasNeighbor = true;
							}
						}
					}
				}
			}
			if (!hasNeighbor) {
				if (t.getIsU()) {
					v[t.getIdx()].setLeaf();
				}
				else {
					v[t.getIdx() + nU].setLeaf();
				}
			}
		}
		/*遍历边数组*/
		int lenOfEdge = edge.size();
		if (DEBUG) {
			printf("\n");
		}
		for (int i = lenOfEdge-1; i >= 0; i--) {
			Vertex from = v[edge[i].getFrom()];
			Vertex to = v[edge[i].getTo()];
			double edgeRatio = double(from.getWeight()) / double(to.getWeight());
			edge[i].setWeight(edgeRatio);
			/*判断To节点是否为叶子节点*/
			if (!to.getIsLeaf()) {
				/*找出所有的子节点*/
				if (from.getIsU()) {
					double base = 1.0;
					for (int j = 0; j < lenOfEdge; j++) {
						if (edge[j].getFrom() == to.getIdx() + nU) {
							base += edge[j].getWeight();
						}
					}
					edge[i].setWeight(base* edge[i].getWeight());
				}
				else {
					double base = 1.0;
					for (int j = 0; j < lenOfEdge; j++) {
						if (edge[j].getFrom() == to.getIdx() ) {
							base += edge[j].getWeight();
						}
					}
					edge[i].setWeight(base * edge[i].getWeight());
				}
			}
			if (DEBUG) {
				printf("from:%d to:%d weight:%f", edge[i].getFrom(), edge[i].getTo(), edge[i].getWeight());
				printf("\n");
			}
			/*存入映射表*/
			/*如果是U侧节点*/
			if (edge[i].getFrom() < nU) {
				int offset = row_index[edge[i].getFrom()];
				weight_arr[edge[i].getTo() - nU + offset].setWeight(weight_arr[edge[i].getTo() - nU + offset].getWeight() + edge[i].getWeight());
				weight_arr[edge[i].getTo() - nU + offset].setFrom(edge[i].getFrom());
				weight_arr[edge[i].getTo() - nU + offset].setTo(edge[i].getTo() - nU);
			}
			else {
				int offset = row_index[edge[i].getTo()];
				weight_arr[edge[i].getFrom() - nU + offset].setWeight(weight_arr[edge[i].getFrom() - nU + offset].getWeight() + edge[i].getWeight());
				weight_arr[edge[i].getFrom() - nU + offset].setFrom(edge[i].getTo());
				weight_arr[edge[i].getFrom() - nU + offset].setTo(edge[i].getFrom() - nU);
			}
		}
	}
	/*随机游走的bfs*/
	void rdwalk_bfs(int s) {
		queue<int>q;
		vector<Edge>edge;
		stack<Vertex>stk;
		Vertex* v = new Vertex[numVertex];
		/*设置索引*/
		for (int i = 0; i < nU; i++) {
			v[i].setIdx(i);
			v[i].setU();
		}
		for (int i = 0; i < nV; i++) {
			v[i + nU].setIdx(i);
		}
		/*打印v数组*/
		if (!DEBUG) {
			printf("V数组为: \n");
			for (int i = 0; i < numVertex; i++) {
				printf("idx:%d \n", v[i].getIdx());
			}
		}
		q.push(s);
		v[s].setAssigned();
		/*默认从U侧开始*/
		while (!q.empty()) {
			Vertex t = v[q.front()];
			stk.push(t);
			int currIdx = t.getIdx();
			/*打印信息*/
			if (!DEBUG) {
				printf("id:%d ", t.getIdx());
			}
			q.pop();
			/*放入邻居节点*/
			/*如果是U侧节点*/
			bool hasNeighbor = false;
			if (t.getIsU()) {
				/*获取邻居节点*/
				int offset = row_index[currIdx];
				int nNeighbor = row_index[currIdx + 1] - row_index[currIdx];
				/*随机选择子节点*/
				/*生成随机数种子*/
				srand((unsigned)time(NULL));
				int rdchld = (rand() % (nNeighbor));
				for (int i = 0; i < nNeighbor; i++) {
					/*生成选择概率*/
					float r = static_cast<float>(rand()) / static_cast<float> (RAND_MAX);
					if (r < 0.5) {
						continue;
					}
					if (!v[nU + col_index[offset + i]].getAssigned()) {
						v[nU + col_index[offset + i]].setAssigned();
						v[nU + col_index[offset + i]].setDistance(t.getDistance() + 1);
						v[nU + col_index[offset + i]].setWeight(t.getWeight());
						q.push(nU + col_index[offset + i]);
						/*打印信息*/
						if (DEBUG) {
							printf("\n");
							printf("U侧的%d新赋值V侧的%d  邻居的d为%d w为%d", currIdx, v[nU + col_index[offset + i]].getIdx(), v[nU + col_index[offset + i]].getDistance(), v[nU + col_index[offset + i]].getWeight());
						}
						Edge edge_new(currIdx, nU + col_index[offset + i], 1);
						edge.push_back(edge_new);
						hasNeighbor = true;
					}
					else {
						/*判断邻居节点是否是下一节点*/
						if (v[nU + col_index[offset + i]].getDistance() == t.getDistance() + 1) {
							v[nU + col_index[offset + i]].setWeight(v[nU + col_index[offset + i]].getWeight() + 1);
							/*打印信息*/
							if (DEBUG) {
								printf("\n");
								printf("U侧的%d再赋值V侧的%d  邻居的d为%d w为%d", currIdx, v[nU + col_index[offset + i]].getIdx(), v[nU + col_index[offset + i]].getDistance(), v[nU + col_index[offset + i]].getWeight());
							}
							Edge edge_new(currIdx, nU + col_index[offset + i], 1);
							edge.push_back(edge_new);
							hasNeighbor = true;
						}
					}
				}
			}
			/*如果是V侧节点*/
			else {
				/*遍历col_index*/
				for (int i = 0; i < numVertex; i++) {
					if (col_index[i] == currIdx) {
						/*生成选择概率*/
						float r = static_cast<float>(rand()) / static_cast<float> (RAND_MAX);
						if (r < 0.5) {
							continue;
						}
						if (!v[col_help[i]].getAssigned()) {
							v[col_help[i]].setAssigned();
							v[col_help[i]].setDistance(t.getDistance() + 1);
							v[col_help[i]].setWeight(t.getWeight());
							q.push(col_help[i]);
							if (DEBUG) {
								printf("\n");
								printf("V侧%d新赋值%d 邻居的d为%d 节点的d为%d", col_index[i], col_help[i], v[col_help[i]].getDistance(), v[col_index[i]].getDistance());
							}
							Edge edge_new(currIdx + nU, col_help[i], 1);
							edge.push_back(edge_new);
							hasNeighbor = true;
						}
						else {
							if (v[col_help[i]].getDistance() == t.getDistance() + 1) {
								v[col_help[i]].setWeight(v[col_help[i]].getWeight() + 1);
								if (DEBUG) {
									printf("\n");
									printf("V侧%d再赋值%d 邻居的d为%d 节点的d为%d", col_index[i], col_help[i], v[col_help[i]].getDistance(), v[col_index[i]].getDistance());
								}
								Edge edge_new(currIdx + nU, col_help[i], 1);
								edge.push_back(edge_new);
								hasNeighbor = true;
							}
						}
					}
				}
			}
			if (!hasNeighbor) {
				if (t.getIsU()) {
					v[t.getIdx()].setLeaf();
				}
				else {
					v[t.getIdx() + nU].setLeaf();
				}
			}
		}
		/*遍历边数组*/
		int lenOfEdge = edge.size();
		if (DEBUG) {
			printf("\n");
		}
		for (int i = lenOfEdge - 1; i >= 0; i--) {
			Vertex from = v[edge[i].getFrom()];
			Vertex to = v[edge[i].getTo()];
			double edgeRatio = double(from.getWeight()) / double(to.getWeight());
			edge[i].setWeight(edgeRatio);
			/*判断To节点是否为叶子节点*/
			if (!to.getIsLeaf()) {
				/*找出所有的子节点*/
				if (from.getIsU()) {
					double base = 1.0;
					for (int j = 0; j < lenOfEdge; j++) {
						if (edge[j].getFrom() == to.getIdx() + nU) {
							base += edge[j].getWeight();
						}
					}
					edge[i].setWeight(base * edge[i].getWeight());
				}
				else {
					double base = 1.0;
					for (int j = 0; j < lenOfEdge; j++) {
						if (edge[j].getFrom() == to.getIdx()) {
							base += edge[j].getWeight();
						}
					}
					edge[i].setWeight(base * edge[i].getWeight());
				}
			}
			if (DEBUG) {
				printf("from:%d to:%d weight:%f", edge[i].getFrom(), edge[i].getTo(), edge[i].getWeight());
				printf("\n");
			}
			/*存入映射表*/
			/*如果是U侧节点*/
			if (edge[i].getFrom() < nU) {
				int offset = row_index[edge[i].getFrom()];
				weight_arr[edge[i].getTo() - nU + offset].setWeight(weight_arr[edge[i].getTo() - nU + offset].getWeight() + edge[i].getWeight());
				weight_arr[edge[i].getTo() - nU + offset].setFrom(edge[i].getFrom());
				weight_arr[edge[i].getTo() - nU + offset].setTo(edge[i].getTo() - nU);
			}
			else {
				int offset = row_index[edge[i].getTo()];
				weight_arr[edge[i].getFrom() - nU + offset].setWeight(weight_arr[edge[i].getFrom() - nU + offset].getWeight() + edge[i].getWeight());
				weight_arr[edge[i].getFrom() - nU + offset].setFrom(edge[i].getTo());
				weight_arr[edge[i].getFrom() - nU + offset].setTo(edge[i].getFrom() - nU);
			}
		}

	}
	/*排序函数*/
	static bool cmp(Edge& a, Edge& b) {
		return a.getWeight() >= b.getWeight();
	}
	/*edge-betweenenss计算程序*/
	void edgeBetweenness_calc() {
		for (int i = 0; i < nU; i++) {
			bfs(i);
		}
		/*排序函数*/
		sort(weight_arr, weight_arr + numEdge, cmp);
		/*打印结果*/
		if (DEBUG) {
			for (int j = 0; j < TOP; j++) {
				printf("起点为%d,终点为%d,权重为%f \n", weight_arr[j].getFrom(), weight_arr[j].getTo(), weight_arr[j].getWeight());
			}
		}
	}
};

