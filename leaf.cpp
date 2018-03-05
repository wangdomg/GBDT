#include "leaf.h"
#include <cmath>

vector<int> LeafNode::get_idset() {
    return idset;
}

double LeafNode::get_predict_value() {
    return predict_value;
}

void LeafNode::update_predict_value(map<int, double> targets, int K) {
    double up = 0.0;
    for (int i = 0; i < idset.size(); i++) {
        up += targets[idset[i]];
    }
    double down = 0.0;
    for (int i = 0; i < idset.size(); i++) {
        down += abs(targets[idset[i]]) * (1 - abs(targets[idset[i]]));
    }
    predict_value = (up/down)*((double)(K-1)/K);
}
