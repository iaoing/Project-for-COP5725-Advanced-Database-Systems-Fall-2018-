#ifndef __BUILD_TVP_H__
#define __BUILD_TVP_H__ 

#include <stdio.h>
#include <string>
#include "mb_tree.h"

class TVP;
class ECT;
class CT;

typedef struct tvp_node
{	
	int a;
	int b;
	int n;
}tvp_node;

bool compare_func(mbt_node *n1, mbt_node *n2);
bool compare_func_tvp_node(tvp_node n1, tvp_node n2);
mbt_node *copy_constructor(mbt_node *target);
bool is_joint(vector<int> r1, vector<int> r2);
void exchage_num(int *a, int *b);




class TVP
{
public:
	TVP(){
		raw_tree = NULL;
	};
	~TVP();

	void build_tvp(mb_tree *T, ECT *ect, vector<mbt_node*>* keynodes, const char *key);
	void show();
	void copy_tree(mb_tree *ect);
	mbt_node *nn_search(int id);
	int nn_search_id(int id);
	void flush(int fd);
	std::string flush();

public:
	mb_tree *raw_tree;
	mb_tree *adv_tree;
	vector<tvp_node> *tvp_index;
};


class ECT
{
public:
	ECT(){
		ectree = NULL;
	};

	~ECT(){};

	void build_ect(mb_tree *T, CT *ct, vector<mbt_node*>* keynodes, const char *key);
	mbt_node *get_nn_node(vector<mbt_node*>* keynodes, mbt_node* n, const char *key);
	mbt_node *get_nn_node(vector<mbt_node*>* keynodes, int id, const char *key);
	vector<mbt_node*> *get_children(int id);
	vector<int> *get_children_id(int id);

public:
	mb_tree *ectree;
	mb_tree *raw_tree;

};


class CT
{
public:
	CT(){
		ctree = new mb_tree();
	};

	~CT(){
		// delete ctree;
	};

	void build_ct(mb_tree *T, vector<mbt_node*>* keynodes, const char *key);
	mbt_node* get_nn_node(mb_tree *T, vector<mbt_node*>* keynodes, mbt_node* n, const char *key);

public:
	mb_tree *ctree;
	mb_tree *raw_tree;
	
};


#endif