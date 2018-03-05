#include <iostream>
#include <time.h>
#include <vector>
#include "gbdt.h"

using namespace std;

int main(int argc, char *argv[])
{
    // 时钟，测试运行时间
    clock_t cstart, cend;
    cstart = clock();

    // 命令行传进来的参数
    int max_iter = atoi(argv[1]);
    double learn_rate = atof(argv[2]);
    double max_depth = atof(argv[3]);
    char* trainfile = argv[4];
    char* testfile = argv[5];

    // 构造数据集
	Data train_set(trainfile);
	Data test_set(testfile);

    // 训练
    GBDT gbdt = GBDT(max_iter, learn_rate, max_depth);
	gbdt.train(&train_set, train_set.get_instances_idset());


    /****下面进行测试****/
	int num_test = test_set.size();
	int right = 0;
	int label_size = train_set.get_label_size();

    // confusion matrix
	vector< vector<int> > matrix(label_size, vector<int>(label_size));
	for(int i=0; i<label_size; i++)
	for(int j=0; j<label_size; j++)
	    matrix[i][j] = 0;

	for (int i = 0; i<test_set.size(); i++)
	{
		auto instance = test_set.get_instance(i);
		int plabel = gbdt.predict(instance)-1;
		int rlabel = instance["label"]-1;
		matrix[plabel][rlabel] += 1;
		if (rlabel == plabel)
			right++;
	}

	cend = clock();

	cout<<"准确率: "<<(double)(right)/num_test<<endl;
	cout<<"用时: "<<(double)(cend-cstart)/CLOCKS_PER_SEC<<" s"<<endl;

	cout << "confusion matrix 如下: "<<endl;
	for(int i=0; i<label_size; i++)
	{
	    for(int j=0; j<label_size; j++)
	        cout<<matrix[i][j]<<"    ";
	    cout<<endl;
	}
}
