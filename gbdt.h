#include <iostream>
#include <map>
#include <vector>
#include "tree.h"

using namespace std;

class GBDT
{
public:
	GBDT(int max_iter, double learn_rate, int max_depth)
	{
		this->max_iter = max_iter;
		this->learn_rate = learn_rate;
		this->max_depth = max_depth;
	}

	/****属性****/
	// 迭代次数
	int max_iter;
	//　学习率
	float learn_rate;
	// 树的最大深度
	int max_depth;
	// 所有的树, trees[i][j]表示第i轮迭代时为第ｊ类构造的树
	map<int, map<int, Tree*> > trees;

    /****方法****/
    // 训练
	void train(Data* dataset, vector<int> dataid);
	// 预测
	int predict(map<string, int>& instance);
	// 计算残差矩阵
	vector< vector<double> > compute_residual(Data* dataset, vector< vector<double> > f);
    // 根据构建的树更新ｆ矩阵
    void update_f_value(vector< vector<double> >& f, int k, Tree* tree, vector<LeafNode> leaf_nodes, Data* dataset, double learn_rate);
    // 初始化ｆ矩阵
    void initialize(vector< vector<double> >& f, Data* dataset);
};
