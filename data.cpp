#include "data.h"

using namespace std;

// 分割字符串成string
void Data::splitString(const string& s, vector<string>& v, const string& c)
{
  string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  while(string::npos != pos2)
  {
    v.push_back(s.substr(pos1, pos2-pos1));
 
    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if(pos1 != s.length())
    v.push_back(s.substr(pos1));
}

// 分割字符串成int
void splitStringToF(const string& s, vector<int>& v, const string& c)
{
  string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  while(string::npos != pos2)
  {
    v.push_back(atoi(s.substr(pos1, pos2-pos1).c_str()));
 
    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if(pos1 != s.length())
    v.push_back(atoi(s.substr(pos1).c_str()));
}


map<string,int>* Data::construct_instance(string s)
{
	map<string,int> *instance = new map<string,int>;

	vector<int> vals;
	splitStringToF(s, vals, ",");
	for(int i=0; i<vals.size(); i++)
	{
		string field_name = field_names[i];
		(*instance)[field_name] = vals[i];
		distinct_valueset[field_name]->insert(vals[i]);
	}
	return instance;
}


vector<int> Data::get_instances_idset()
{
	vector<int> idset;
	for(int i=0; i<instances.size(); i++)
		idset.push_back(i);

	return idset;
}

int Data::get_label_size()
{
	return distinct_valueset["label"]->size();
}

set<int> Data::get_label_valueset()
{
	return *(distinct_valueset["label"]);
}

int Data::size()
{
	return instances.size();
}

map<string,int> Data::get_instance(int id)
{
	return *(instances[id]);
}

vector<string> Data::get_attributes()
{
	vector<string> attribute_names;

	for(auto x:field_names)
		if(x!="label")
			attribute_names.push_back(x);

	return attribute_names;
}

set<int> Data::get_distinct_valueset(string name)
{
	return *(distinct_valueset[name]);
}

