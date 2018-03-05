#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <set>
#include <vector>

using namespace std;

class Data
{
public:
	Data(string filename)
	{
		ifstream fin(filename);  
		string s;  
		int line = 0;
		while(getline(fin,s))
		{    
			if(line == 0)
			{
				splitString(s, field_names, ",");
				for(int i=0; i<field_names.size(); i++)
				{
					distinct_valueset[field_names[i]] = new set<int>;
				}
			}
			else
			{
				instances.push_back(construct_instance(s));
			}
			line++;
		}
	}

    /****属性****/
    // 第一行的所有字段，包括属性名和label名
	vector<string> field_names;
	// 各个字段下的所有取值集合
	map<string, set<int>*> distinct_valueset;
    // 所有样本
	vector<map<string,int>*> instances;



    /****方法****/
    // 样本数
	int size();
    // 样本的id集合
	vector<int> get_instances_idset();
	// 根据样本的id得到样本的各个属性值
	map<string,int> get_instance(int id);  // 根据样本的id得到样本的各个属性的值，返回的是一个map
    //　类别数
	int get_label_size();
	//　类别的取值集合
	set<int> get_label_valueset();
    //　属性名
	vector<string> get_attributes();
	//　各个属性下的所有取值
	set<int> get_distinct_valueset(string name);
    //  将文件的一行按照分隔符分割
	void splitString(const string&, vector<string>&, const string&);
    // 根据文件的一行构造一个样本实例
	map<string,int>* construct_instance(string s);

};
