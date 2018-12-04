#ifndef __MB_TREE_H__
#define __MB_TREE_H__ 

#include <string.h>
#include <stdio.h>
#include <vector>
#include "debug_log.h"

using std::vector;

class mbt_node;
class mb_tree;


class mbt_node{
public:
	mbt_node() : _id(-1), _level(-1), _tag(NULL), _text(NULL), _parent(NULL), _children(new vector<mbt_node*>) {
		_children->reserve(32);
	}

	mbt_node(int i) : _id(i), _level(-1), _tag(NULL), _text(NULL), _parent(NULL), _children(new vector<mbt_node*>) { }

	mbt_node(int i, char *t) : _id(i), _level(-1), _text(NULL), _parent(NULL), _children(new vector<mbt_node*>) {
		_tag  = (char*)malloc(strlen(t) * sizeof(char) + 2);
		sprintf(_tag,  "%s%c", t, '\0');
	}

	mbt_node(int i, char *t, char *x) : _id(i), _level(-1), _tag(t), _text(x), _parent(NULL), _children(new vector<mbt_node*>) { }

	mbt_node(int i, char *t, char *x, vector<mbt_node*> *vc) : _id(i), _level(-1), _tag(t), _text(x), _parent(NULL), _children(vc) { }
	
	mbt_node(int i, const char *t) : _id(i), _level(-1) {
		_tag = (char*)malloc(strlen(t) * sizeof(char) + 2);
		sprintf(_tag, "%s%c", t, '\0');
		_text = NULL;
		_parent = NULL;
		_children = new vector<mbt_node*>;
	}

	mbt_node(int i, const char *t, const char *x) : _id(i), _level(-1) {
		_tag  = (char*)malloc(strlen(t) * sizeof(char) + 2);
		_text = (char*)malloc(strlen(x) * sizeof(char) + 2);
		sprintf(_tag,  "%s%c", t, '\0');
		sprintf(_text, "%s%c", x, '\0');
		_parent = NULL;
		_children = new vector<mbt_node*>;
	}
	
	// mbt_node(mbt_node* other);
	
	~mbt_node(){
		// if(_tag)  free(_tag);
		// if(_text) free(_text);
		// if(_children != NULL){
		// 	for(int i = _children->size(); i >= 0; ++i){
		// 		delete (*_children)[i];
		// 	}
		// 	delete _children;
		// }
	}

public:
	void add_child(mbt_node *node);
	mbt_node* find_node(int t_id);
	void get_range(int *a, int *b);
	mbt_node* get_parent();
	vector<mbt_node*>* get_children();
	void set_level(int l);
	void update_info();
	int is_leaf();

public:
	void find_key_nodes(vector<mbt_node*> *ret_vc, const char *key);
	int key_fit(const char *key);
	mbt_node* find_nn_node(const char *key);
	void get_all_nodes(vector<mbt_node*>* ret_vc);

public:
	char *_tag;
	char *_text;
	int _id;
	int _level;
	mbt_node *_parent;
	vector<mbt_node*> *_children;

// for debug;
public:
	// level 0: print id only;
	// level 1: print id and tag;
	// level 2: print id and tag and text;
	void visit(int level, const char *prefix);
	void show(int level);
};


class mb_tree
{
public:
	mb_tree():_root(NULL), _nb_nodes(0), _depth(0){
		_keywords = new vector<char*>();
	};

	mb_tree(mbt_node *t):_root(t), _nb_nodes(1), _depth(0){
		_keywords = new vector<char*>();
	};
	
	~mb_tree(){
		// if(_root) delete _root;
	}

public:
	int size();
	void add_root(mbt_node *t);
	void add_node(mbt_node *node, int parent_id);
	void add_node_key(mbt_node *node);
	void clear_keywords();
	// void add_node(mbt_node *node, mbt_node *nodex);
	vector<mbt_node*> *get_all_nodes();
	mbt_node* find_node(int id);
	vector<mbt_node*> *find_key_nodes(const char *key);
	void find_key_nodes(vector<mbt_node*> *ret_vc, const char *key);
	mbt_node* find_nn_key_node(mbt_node *node, const char *key);
	mbt_node* find_nn_key_node(int id, const char *key);
	int is_joint(mbt_node *n1, mbt_node *n2);
	mbt_node* get_parent(mbt_node *n);
	mbt_node* get_parent(int id);
	mbt_node* get_lca(mbt_node *n1, mbt_node *n2);
	int get_distance(mbt_node *n1, mbt_node *n2);
	int get_distance(int id1, int id2);
	void get_range(mbt_node *n, int *a, int *b);
	void get_range(int id, int *a, int *b);
	void remove_node(mbt_node *n);
	void update_info(int id);
	vector<int> *get_children_id(int id);

// private:
	// int compare_nodes_id(mbt_node *n1, mbt_node *n2);
	// int compare_nodes_level(mbt_node *n1, mbt_node *n2);
	// int compare_nodes_tag(mbt_node *n1, mbt_node *n2);
	// int compare_nodes_text(mbt_node *n1, mbt_node *n2);

	bool not_in_keywords(const char *x);

public:
	mbt_node *_root;
	int _nb_nodes;	// number of nodes;
	int _depth;		// _depth of this tree;
	vector<char*> *_keywords;

// for debug;
public:
	void visit(int level);
	void visit();
	void show_keys();
};


bool compare_func12(mbt_node *n1, mbt_node *n2);


#endif