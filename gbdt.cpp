#include "gbdt.h"
#include <cmath>

using namespace std;

// 训练
void GBDT::train(Data* dataset, vector<int> dataid)  // train_data是训练数据的id集合
{

    vector< vector<double> > f(dataset->size(), vector<double>(dataset->get_label_size()));  // f值，用来计算样本属于每一个类别的概率
    initialize(f, dataset);

    for (int iter = 1; iter <= this->max_iter; iter++)
    {
        cout<<"第"<<iter<<"轮迭代"<<endl;

        vector< vector<double> > residual = compute_residual(dataset, f);

        // 对每一个类别建一棵树
        for(int k=0; k<residual[0].size(); k++)
        {
            vector<LeafNode*> leaf_pnodes;

            map<int, double> targets;
            for(int n=0; n<residual.size(); n++)
                targets[n] = residual[n][k];

            vector<string> attr = dataset->get_attributes();
            Tree* tree = Tree::construct_decision_tree(dataset, dataid, attr, targets, 0, leaf_pnodes, this->max_depth);

            vector<LeafNode> leaf_nodes;
            for (LeafNode* p : leaf_pnodes) { leaf_nodes.push_back(*p); }

            this->trees[iter][k] = tree;
            update_f_value(f, k, tree, leaf_nodes, dataset, this->learn_rate);
        }
    }
}

// 预测
int GBDT::predict(map<string, int>& instance)
{
    map<int, double> f_value;
    int K = this->trees[1].size();
    for(int k=0; k<K; k++)
    {
        f_value[k] = 0.0;
    }
    for(int i=1; i<=this->max_iter; i++)
    {
        for(int k=0; k<K; k++)
        {
            Tree* tree = this->trees[i][k];
            f_value[k] += this->learn_rate * tree->get_predict_value(instance);
        }
    }

    map<int, double> exp_value;
    double total = 0.0;
    for(int k=0; k<K; k++)
    {
        exp_value[k] = exp(f_value[k]);
        total += exp_value[k];
    }

    int label = 0;
    double base = 0.0;
    for(int k=0; k<K; k++)
    {
        if(exp_value[k]/total > base)
        {
            label = k;
            base = exp_value[k]/total;
        }
    }

    return label+1;
}


// 多分类，计算每一个样本的残差，这里的k表示第k类
vector< vector<double> > GBDT::compute_residual(Data* dataset, vector< vector<double> > f)
{
    int N = dataset->size();  // 样本数目
    int K = dataset->get_label_size();  // 类别数目

    // 构造样本的真实概率矩阵
    vector< vector<double> > y(N, vector<double>(K));
    for(auto id : dataset->get_instances_idset())
    {
        for(int i=0; i<K; i++)
        {
            if(i==dataset->get_instance(id)["label"]-1)
                y[id][i] = 1.0;
            else
                y[id][i] = 0.0;
        }
    }
    
    // 计算样本的概率矩阵
    vector< vector<double> > p(N, vector<double>(K));
    for(auto id : dataset->get_instances_idset())
    {
        double total = 0.0;
        for(int i=0; i<K; i++)
            total += exp(f[id][i]);
        for(int i=0; i<K; i++)
            p[id][i] = exp(f[id][i]) / total;
    }

    // 计算样本的残差矩阵
    vector< vector<double> > residual(N, vector<double>(K));
    for(auto id : dataset->get_instances_idset())
    {
        for(int i=0; i<K; i++)
            residual[id][i] = y[id][i] - p[id][i];
    }

    return residual;
}


// 根据样本所在的叶子节点的预测值更新每一个样本的f值
void GBDT::update_f_value(vector< vector<double> >& f, int k,  Tree* tree, vector<LeafNode> leaf_nodes, Data* dataset, double learn_rate)
{
    vector<int> data_idset = dataset->get_instances_idset();
    for(auto node : leaf_nodes)
    {
        for(auto id : node.get_idset())
        {
            f[id][k] += learn_rate * node.get_predict_value();
        }
    }
    
}

// 对每一个样本初始化它的f值
void GBDT::initialize(vector< vector<double> >& f, Data* dataset) {
    vector<int> ids = dataset->get_instances_idset();
    for (auto id : ids) 
    {
        int K = dataset->get_label_size();
        for(int k=0; k<K; k++)
            f[id][k] = 0.0;
    }
}


