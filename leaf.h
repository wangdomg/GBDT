#include <map>
#include <vector>
using namespace std;


class LeafNode {
public:
    LeafNode(vector<int>& idvec)
    {
        idset = idvec;
    }

    /****属性****/
    // 叶子节点包含的样本的id集合
    vector<int> idset;
    // 叶子节点的预测值
    double predict_value;
    // 训练样本的类别数
    int K;  // 一共有多少个类别

    /****方法****/
    // 得到叶子节点包含的样本的id集合
    vector<int> get_idset();
    // 得到叶子节点的预测值
    double get_predict_value();
    //　更新叶子节点的预测值
    void update_predict_value(map<int, double> targets, int K);
};
