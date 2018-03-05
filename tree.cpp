#include "tree.h"
#include <iostream>

using namespace std;

double Tree::get_predict_value(map<string, int> instance) {
        
    if(this->leaf_node != NULL) {
        return this->leaf_node->get_predict_value();
    }
    if (split_feature == "") {
        throw "the tree is null";
    }
    if(instance[split_feature] == condition_value) {
        return this->left_tree->get_predict_value(instance);
    }
    return this->right_tree->get_predict_value(instance);
}

double MSE(vector<double>& values) {
    if(values.size() < 2)
        return 0.0;
    double sum = 0.0;
    for (int i = 0; i < values.size(); i++) {
        sum += values[i];
    }
    double mean = sum / float(values.size());
    double error = 0.0;
    for (int i = 0; i < values.size(); i++) {
        error += (mean - values[i]) * (mean - values[i]);
    }
    return error/(double)(values.size());
}

Tree* Tree::construct_decision_tree(Data* dataset, vector<int> &remainedSet, vector<string> remainedAttr, map<int, double>targets, int depth,
                              vector<LeafNode*> &leaf_nodes, int max_depth) {
    if (depth < max_depth) 
    {
        vector<string> attributes = remainedAttr;
        double mse = -100.0;
        string selectedAttribute = "";
        int conditionValue = 0;
        vector<int> selectedLeftIdSet;
        vector<int> selectedRightIdSet;
        for (auto attribute : attributes) {
            set<int> attrValues = dataset->get_distinct_valueset(attribute);
            for (auto attrValue : attrValues) {
                vector<int> leftIdSet;
                vector<int> rightIdSet;
                for (auto Id : remainedSet) {
                    map<string, int> instance = dataset->get_instance(Id);
                    int value = instance[attribute];
                    if (value == attrValue) {
                        leftIdSet.push_back(Id);
                    } else {
                        rightIdSet.push_back(Id);
                    }
                }
                vector<double> leftTargets;
                vector<double> rightTargets;
                for (auto id : leftIdSet) {
                    leftTargets.push_back(targets[id]);
                }
                for (auto id : rightIdSet) {
                    rightTargets.push_back(targets[id]);
                }
                double sum_mse = ((double)(leftTargets.size())/(leftTargets.size()+rightTargets.size()))*MSE(leftTargets)+((double)(rightTargets.size())/(leftTargets.size()+rightTargets.size()))*MSE(rightTargets);
                if (mse < 0 || sum_mse < mse) {
                    selectedAttribute = attribute;
                    conditionValue = attrValue;
                    mse = sum_mse;
                    selectedLeftIdSet = leftIdSet;
                    selectedRightIdSet = rightIdSet;
                }
            }
        }
        if (selectedAttribute == ""|| mse < 0) {
            throw "cannot determine the split attribute.";
        }


        // 判断左右两棵子树是否为空，只要有一棵树为空，就不再分类了
        if(selectedLeftIdSet.size()>0 && selectedRightIdSet.size()>0)
        {
            Tree *tree = new Tree();
            tree->split_feature = selectedAttribute;
            tree->condition_value = conditionValue;
            // 去掉已经用过的分裂属性
            for(vector<string>::iterator it=remainedAttr.begin(); it!=remainedAttr.end(); it++)
            {
                if(*it==selectedAttribute)
                {
                    remainedAttr.erase(it);
                    break;
                }

            }
            tree->left_tree = construct_decision_tree(dataset, selectedLeftIdSet, remainedAttr, targets, depth+1, leaf_nodes, max_depth);
            tree->right_tree = construct_decision_tree(dataset, selectedRightIdSet, remainedAttr, targets, depth+1, leaf_nodes, max_depth);
            return tree;
        }
        else if(selectedLeftIdSet.size()>0)
        {
            LeafNode *node = new LeafNode(selectedLeftIdSet);
            node->update_predict_value(targets, dataset->get_label_size());
            leaf_nodes.push_back(node);  // 这里记录叶子节点是为了之后方便根据新建的树更新每一个样本的f值
            Tree *tree = new Tree();
            tree->leaf_node = node;
            return tree;
        }
        else
        {
            LeafNode *node = new LeafNode(selectedRightIdSet);
            node->update_predict_value(targets, dataset->get_label_size());
            leaf_nodes.push_back(node);  // 这里记录叶子节点是为了之后方便根据新建的树更新每一个样本的f值
            Tree *tree = new Tree();
            tree->leaf_node = node;
            return tree;
        }

    } 
    else 
    {
        LeafNode *node = new LeafNode(remainedSet);
        node->update_predict_value(targets, dataset->get_label_size());
        leaf_nodes.push_back(node);  // 这里记录叶子节点是为了之后方便根据新建的树更新每一个样本的f值
        Tree *tree = new Tree();
        tree->leaf_node = node;
        return tree;
    }
}