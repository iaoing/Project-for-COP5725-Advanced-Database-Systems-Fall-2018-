#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <algorithm>
#include "mb_tree.h"



using std::vector;
using std::max;
using std::min;
using std::string;
using std::queue;
using std::set;
using std::string;
// using namespace std;



////////////////////////////////
// class mbt_node;
////////////////////////////////
////////////////////////////////
// public functions;
////////////////////////////////
// mbt_node::mbt_node(mbt_node* other){
// 	char *t1, *t2;
// 	if(strlen(other->_tag) > 0){
// 		_tag = (char*)malloc(strlen(other->_tag)  * sizeof(char) + 2);
// 		sprintf(_tag, "%s%c", other->_tag, '\0');
// 	}else{
// 		_tag = NULL;
// 	}
// 	if(strlen(other->_text) > 0){
// 		_text = (char*)malloc(strlen(other->_text) * sizeof(char) + 2);
// 		sprintf(_text, "%s%c", other->_text, '\0');
// 	}else{
// 		_text = NULL;
// 	}
// 	_tag = t1;
// 	_text = t2;
// 	_id = other->_id;
// 	_parent = NULL;
// 	_children = new vector<mbt_node*>;
// 	// _level = other->_level;
// }

void mbt_node::add_child(mbt_node *node){
	node->_level = _level + 1;
	node->_parent = this;
	if(_children == NULL){
		LOG_X;
		exit(0);
	}
	// LOG("389219`283. %d %d \n", _children->size(), _children->capacity())
	// if(_children->capacity() < _children->size() + 5){
		// LOG("reserve the capacity. %d %d \n", _children->size(), _children->capacity())
	// 	_children->reserve(_children->capacity() + 32);
	// }
	// _children->resize(_children->size() + 1);
	// LOG("6uyty46532. %d %d \n", _children->size(), _children->capacity())
	// (*_children)[_children->size() - 1] = node;
	_children->push_back(node);
	// std::sort(_children->begin(), _children->end(), compare_func);
}

mbt_node* mbt_node::find_node(int t_id){
	// LOG("Line: %d. [%d] find [%d].\n", __LINE__, _id, t_id);
	if(t_id < _id){
		// LOG("Line: %d. Return this [%d].\n", __LINE__, _id);
		return NULL;
	}
	if(t_id == _id){
		// LOG("Line: %d. Return this [%d].\n", __LINE__, _id);
		return this;
	}
	int i = 0;
	mbt_node *pt = NULL;
	if(_children->size() == 0){
		return NULL;
	}
	std::sort(_children->begin(), _children->end(), compare_func12);

	if((*_children)[_children->size() - 1]->_id < t_id)
		return (*_children)[_children->size() - 1]->find_node(t_id);

	if(_children->size() > 0){
		pt = (*_children)[0];
	}
	for(i = 0; i < _children->size(); ++i){
		if((*_children)[i]->_id > t_id){
			return pt->find_node(t_id);
		}else if((*_children)[i]->_id < t_id){
			pt = (*_children)[i];
			continue;
		}else if((*_children)[i]->_id == t_id){
			// LOG("Line: %d. Return this _children[%d] [%d].\n", __LINE__, i, _id);
			return (*_children)[i];
		}
	}
	if(pt != NULL){
		return pt->find_node(t_id);
	}else
		return NULL;
}

void mbt_node::find_key_nodes(vector<mbt_node*> *ret_vc, const char *key){
	if(_tag && strcmp(_tag, key) == 0){
		ret_vc->push_back(this);
	}

	if(_children == NULL || _children->size() <= 0){
		return;
	}



	int i = 0;
	for(i = 0; i < _children->size(); ++i){

		// if(_tag != NULL){
		// 	LOG("id: [%d], tag: [%s], [%d] of [%d] children\n", _id, _tag, i, _children->size());
		// }else{			
		// 	LOG("id: [%d], tag: [NULL], [%d] of [%d] children\n", _id, i, _children->size());
		// }

		if((*_children)[i]->_tag != NULL && strcmp((*_children)[i]->_tag , key) == 0){
			ret_vc->push_back((*_children)[i]);
		}
		(*_children)[i]->find_key_nodes(ret_vc, key);
	}

	// int i = 0;
	// for(i = 0; i < _children->size(); ++i){
	// 	(*_children)[i]->find_key_nodes(ret_vc, key);
	// }
	return;
}

mbt_node* mbt_node::find_nn_node(const char *key){
	if(_tag && strcmp(_tag, key) == 0){
		return this;
	}
	int i = 0;
	mbt_node *n;
	queue<mbt_node*> lpq;
	set<mbt_node*> lpset;
	lpq.push(this);
	while(!lpq.empty()){
		n = lpq.front();
		lpset.insert(n);
		lpq.pop();
		if(n->_tag && strcmp(n->_tag, key) == 0){
			return n;
		}
		// if(lpq.empty()){
			if(n->_parent && (lpset.find(n->_parent) == lpset.end())){
				lpq.push(n->_parent);
			}
			for(i = 0; i < n->_children->size(); ++i){
				if(lpset.find((*(n->_children))[i]) == lpset.end()){
					lpq.push((*(n->_children))[i]);
				}
			}
		// }
	}
	return NULL;
}

void mbt_node::get_all_nodes(vector<mbt_node*>* ret_vc){
	ret_vc->push_back(this);
	if(_children){
		for(int i = 0; i < _children->size(); ++i){
			((*_children)[i])->get_all_nodes(ret_vc);
		}
	}
}

/////////////////////////////////////////
// 
/////////////////////////////////////////
void mbt_node::get_range(int *a, int *b){
	mbt_node* last = this;
	while(last->_children->size() > 0 ){
		last = last->_children->back();
	}
	// vector<int> *rst = new vector<int>{this->_id, last->_id};
	// ret_vc[0] = _id;
	// ret_vc[1] = last->_id;
	*a = _id;
	*b = last->_id;
}


mbt_node* mbt_node::get_parent(){
	return this->_parent;
}

vector<mbt_node*>* mbt_node::get_children(){
	return this->_children;
}

void mbt_node::set_level(int l){
	_level = l;
}

void mbt_node::update_info(){
	vector<mbt_node*>::iterator it;
	std::sort(_children->begin(), _children->end(), compare_func12);
	for(it = _children->begin(); it != _children->end(); ++it){
		(*it)->_level = _level + 1;
		(*it)->update_info();
	}
	// for(int i = 0; i < _children->size(); ++i){
	// 	(*_children)[i]->_level = _level + 1;
	// 	(*_children)[i]->update_info();
	// }
}

int mbt_node::is_leaf(){
	if(_children->size() == 0){
		return 1;
	}
	return 0;
}

int mbt_node::key_fit(const char *key){
	if(strcmp(_tag, key) == 0){
		return 1;
	}
	return 0;
}


//////////////////////////////////////
// for debug;
//////////////////////////////////////
void mbt_node::visit(int level, const char *prefix){
	if(level >= 0){
		printf("%s%d", prefix, this->_id);
	}
	if(level >= 1){
		if(this->_tag){
			printf(" \"%s\"", _tag);
		}else{
			printf(" \"\"");
		}
	}
	if(level >= 2){
		if(this->_text){
			printf(" \"%s\"", _text);
		}else{
			printf(" \"\"");
		}
	}
	printf("\n");

	char *ch_prefix = (char*)malloc(1024 * sizeof(char));
	sprintf(ch_prefix, "%s|   ", prefix);
	if(_children == NULL){
		// LOG("id: [%d]\n", _id);
		exit(0);
	}
	for(int i = 0; i < _children->size(); ++i){
		(*_children)[i]->visit(level, ch_prefix);
	}
	free(ch_prefix);
}

void mbt_node::show(int level){
	if(level >= 0){
		printf("[%d]", this->_id);
	}
	if(level >= 1){
		if(this->_tag){
			printf(" \"%s\"", _tag);
		}else{
			printf(" \"\"");
		}
	}
	if(level >= 2){
		if(this->_text){
			printf(" \"%s\"", _text);
		}else{
			printf(" \"\"");
		}
	}
	printf("\n");
}



////////////////////////////////
// class mb_tree;
////////////////////////////////
////////////////////////////////
// public functions;
////////////////////////////////
int mb_tree::size(){
	return _nb_nodes;
}

void mb_tree::add_root(mbt_node *t){
	if(_root != NULL){
		LOG_X;
		exit(0);
	}
	_root = t;
	_root->set_level(0);
	_nb_nodes++;

	// add keyword to _keywords;
	if(t->_tag != NULL){
		_keywords->push_back(t->_tag);
	}
}

void mb_tree::add_node(mbt_node *node, int parent_id){
	mbt_node *node_x = NULL;
	node_x = find_node(parent_id);
	if(node_x == NULL){
		LOG("node_x == NULL!");
		exit(0);
	}

	// if(parent_id == _root->_id){
	// 	node->_level = 1;
	// 	node->_parent = _root;
	// 	_root->_children->push_back(node);
	// }else{
		node_x->add_child(node);
	// }
	_nb_nodes++;

	// add keyword to _keywords;
	if(node->_tag != NULL && not_in_keywords(node->_tag)){
		_keywords->push_back(node->_tag);
	}
}

void mb_tree::add_node_key(mbt_node *node){
	_nb_nodes++;

	// add keyword to _keywords;
	// if(node->_tag != NULL && not_in_keywords(node->_tag)){
		_keywords->push_back(node->_tag);
	// }
}

void mb_tree::clear_keywords(){
	for(vector<char*>::iterator vit = _keywords->begin(); vit != _keywords->end(); ++vit){
		if(vit == _keywords->begin()){
			continue;
		}
		if((*vit) == (*(vit-1)) || strcmp(*vit, *(vit-1)) == 0){
			_keywords->erase(vit);
			--vit;
		}
	}
}

bool mb_tree::not_in_keywords(const char *x){
	if(x == NULL){
		LOG_X;
		return 1;
	}
	for(int i = 0; i < _keywords->size(); ++i){
		if(strcmp(x, (*_keywords)[i]) == 0){
			return 0;
		}
	}
	return 1;
}

// void mb_tree::add_node(mbt_node *node, mbt_node *parent){
// 	if(node == NULL || parent == NULL){
// 		LOG_X;
// 	}
// 	parent->add_child(node);
// 	_nb_nodes++;
// }

vector<mbt_node*> *mb_tree::get_all_nodes(){
	vector<mbt_node*> *ret_vc = new vector<mbt_node*>;
	if(_root == NULL){
		return ret_vc;
	}
	_root->get_all_nodes(ret_vc);
	return ret_vc;
}

mbt_node* mb_tree::find_node(int id){
	if(id < _root->_id)
		return NULL;
	return _root->find_node(id);
}

vector<mbt_node*> *mb_tree::find_key_nodes(const char *key){
	vector<mbt_node*> *ret_vc = new vector<mbt_node*>;

	if(errno == ENOMEM){
		LOG("errno == ENOMEM\n");
	}

	if(_root == NULL){
		return ret_vc;
	}
	_root->find_key_nodes(ret_vc, key);
	return ret_vc;
}

void mb_tree::find_key_nodes(vector<mbt_node*> *ret_vc, const char *key){
	if(_root == NULL){
		return;
	}

	_root->find_key_nodes(ret_vc, key);

	for(std::vector<mbt_node*>::iterator kit = ret_vc->begin(); kit != ret_vc->end(); ++kit){
		if(kit == ret_vc->begin()){
			continue;
		}
		if((*kit)->_id == (*(kit-1))->_id){
			ret_vc->erase(kit);
			--kit;
		}
	}

}

mbt_node* mb_tree::find_nn_key_node(mbt_node *node, const char *key){
	if(node == NULL)
		return NULL;
	return node->find_nn_node(key);
}

mbt_node* mb_tree::find_nn_key_node(int id, const char *key){
	mbt_node *n;
	n = this->find_node(id);
	if(n == NULL)
		return NULL;
	return find_nn_key_node(n, key);
}


////////////////////////////////
// private functions;
////////////////////////////////
// int mb_tree::compare_nodes_id(mbt_node *n1, mbt_node *n2){
// 	if(n1->_id > n2->_id){
// 		return 1;
// 	}else if(n1->_id == n2->_id){
// 		return 0;
// 	}else{
// 		return -1;
// 	}
// }

// int mb_tree::compare_nodes_level(mbt_node *n1, mbt_node *n2){
// 	if(n1->_level > n2->_level){
// 		return 1;
// 	}else if(n1->_level == n2->_level){
// 		return 0;
// 	}else{
// 		return -1;
// 	}
// }

// int mb_tree::compare_nodes_tag(mbt_node *n1, mbt_node *n2){
// 	if(string(n1->_tag) > string(n2->_tag)){
// 		return 1;
// 	}else if(string(n1->_tag) == string(n2->_tag)){
// 		return 0;
// 	}else{
// 		return -1;
// 	}
// }

// int mb_tree::compare_nodes_text(mbt_node *n1, mbt_node *n2){
// 	if(string(n1->_text) > string(n2->_text)){
// 		return 1;
// 	}else if(string(n1->_text) == string(n2->_text)){
// 		return 0;
// 	}else{
// 		return -1;
// 	}
// }

int mb_tree::is_joint(mbt_node *n1, mbt_node *n2){
	if(n1 == NULL || n2 == NULL){
		LOG_X;
		return -1;
	}
	// vector<int> r1;
	// vector<int> r2;
	int a, b, c, d;
	n1->get_range(&a, &b);
	n2->get_range(&c, &d);
	// if(r1.size() <= 0 || r2.size() <= 0){
	// 	return -1;
	// }
	// if(max(r1[0], r2[0]) > min(r1[1], r2[1])){
	if(max(a, c) > min(b, d)){
		return 0;
	}
	return 1;
}

mbt_node* mb_tree::get_parent(mbt_node *n){
	return n->get_parent();
}

mbt_node* mb_tree::get_parent(int id){
	mbt_node *n;
	n = this->find_node(id);
	if(n == NULL)
		return NULL;
	return n->get_parent();
}

mbt_node* mb_tree::get_lca(mbt_node *n1, mbt_node *n2){
	if(n1 == NULL || n2 == NULL){
		return NULL;
	}
	// if one of them is _root; 
	if(this->_root == n1 || this->_root == n2){
		return this->_root;
	}

	// judge which one is lower; 
	mbt_node *hig_l, *low_l;
	if(n1->_level < n2->_level){
		hig_l = n1;
		low_l = n2;
	}else{
		hig_l = n2;
		low_l = n1;
	}

	// make them at the same level;
	while(low_l->_level != hig_l->_level){
		low_l = low_l->_parent;
	}
	assert(low_l->_level == hig_l->_level);

	// search up concurrently;
	while(low_l != hig_l){
		low_l = low_l->_parent;
		hig_l = hig_l->_parent;
	}

	// got the lca when stopped;
	return hig_l;
}

int mb_tree::get_distance(mbt_node *n1, mbt_node *n2){
	if(n1 == NULL || n2 == NULL){
		return -1;
	}
	mbt_node *lca = get_lca(n1, n2);
	if(lca == NULL){
		return -1;
	}
	return abs(lca->_level - n1->_level) + abs(lca->_level - n2->_level);
}

int mb_tree::get_distance(int id1, int id2){
	return get_distance(find_node(id1), find_node(id2));
}


void mb_tree::get_range(mbt_node *n, int *a, int *b){
	if(n == NULL){
		LOG_X;
		exit(0);
	}
	n->get_range(a, b);
}

void mb_tree::get_range(int id, int *a, int *b){
	mbt_node *n = find_node(id);
	if(n == NULL){
		LOG_X;
		exit(0);
	}
	get_range(n, a, b);
}

void mb_tree::remove_node(mbt_node *n){
	if(n == NULL){
		return;
	}
	if(n->_parent == NULL){
		LOG_X;
		exit(0);
	}
	vector<mbt_node*> *tmp = n->_parent->_children;
	vector<mbt_node*>::iterator it = tmp->begin();
	for(; it != tmp->end(); ++it){
		// int pos = it - tmp->begin();
		// if(n->_id == (*tmp)[pos]->_id){
		if(n->_id == (*it)->_id){
			tmp->erase(it);
			break;
		}
	}
}

void mb_tree::update_info(int id){
	mbt_node *node;
	node = find_node(id);
	if(node == NULL){
		LOG_X;
		return;
	}
	node->update_info();
}

vector<int> *mb_tree::get_children_id(int id){
	vector<int> *ret = new vector<int>();
	mbt_node *node = find_node(id);
	if(node == NULL){
		LOG_X;
		return ret;
	}
	
	for(int i = 0; i < node->_children->size(); ++i){
		ret->push_back((*(node->_children))[i]->_id);
	}
	return ret;
}



//////////////////////////////////////////////
// for debug;
//////////////////////////////////////////////
void mb_tree::visit(int level){
	printf("\n\n++++++++++++++++++++++++++++\n\n");
	if(_root == NULL){
		printf("_root is NULL!\n");
		LOG_X;
		exit(0);
	}
	_root->visit(level, "");
	printf("\n----------------------------\n\n");
}

void mb_tree::visit(){
	visit(1);
}

void mb_tree::show_keys(){
	for(int i = 0; i < _keywords->size(); ++i){
		if((*_keywords)[i] == NULL){
			printf("NULL\n");
			continue;
		}
		printf("%s\n", (*_keywords)[i]);
	}
}

////////////////////////////////////
// 
////////////////////////////////////
bool compare_func12(mbt_node *n1, mbt_node *n2){
	return (n1->_id < n2->_id);
}
