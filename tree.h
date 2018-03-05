#include "data.h"
#include "leaf.h"

using namespace std;

class LeafNode;

class Tree {
public:
    Tree()
    {
        split_feature = "";
        left_tree = NULL;
        right_tree = NULL;
        condition_value = 0.0;
        leaf_node = NULL;
    }

    /****属性****/
    // 分裂所用的属性名
    string split_feature;
    //　分裂属性的值
    double condition_value;
    // 左右子树
    Tree* left_tree;
    Tree* right_tree;
    // 叶子节点
    LeafNode* leaf_node;

    /****方法****/
    // 遍历树，预测样本的值
    double get_predict_value(map<string, int> instance);
    // 类的静态方法，构建一棵树
    static Tree* construct_decision_tree(Data* dataset, std::vector<int> &remainedSet, vector<string> remainedAttr, map<int, double>targets, int depth,
                              vector<LeafNode*> &leaf_nodes, int max_depth);
};
