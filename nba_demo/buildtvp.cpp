#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <vector>
#include <algorithm>
#include "buildtvp.h"

using std::string;
using std::vector;

bool compare_func(mbt_node *n1, mbt_node *n2){
	return (n1->_id < n2->_id);
}

bool compare_func_tvp_node(tvp_node n1, tvp_node n2){
	return (n1.a < n2.a);
}

mbt_node *copy_constructor(mbt_node *target){	
	mbt_node *ret = new mbt_node();
	ret->_id = target->_id;
	if(target->_tag != NULL){
		ret->_tag = (char*)malloc(strlen(target->_tag) + 2);
		sprintf(ret->_tag, "%s%c", target->_tag, '\0');
	}
	if(target->_text != NULL){
		ret->_text = (char*)malloc(strlen(target->_text) + 2);
		sprintf(ret->_text, "%s%c", target->_text, '\0');
	}
	return ret;
}

bool is_joint(vector<int> r1, vector<int> r2){
	if(r1.size() <= 0 || r2.size() <= 0){
		LOG_X;
		exit(0);
	}

	// LOG("check is_joint %d %d --> %d %d\n", r1[0], r1[1], r2[0], r2[1]);

	if(std::max(r1[0], r2[0]) > std::min(r1[1], r2[1])){
		return 0;
	}
	return 1;
}

void exchage_num(int *a, int *b){
	int tmp;
	if(*a > *b){
		tmp = *a;
		*a = *b;
		*b = tmp;
	}
}

///////////////////////////////
// class CT;
///////////////////////////////
void CT::build_ct(mb_tree *T, vector<mbt_node*>* keynodes, const char *key){
	raw_tree = T;
	// get ct nodes;
	std::sort(keynodes->begin(), keynodes->end(), compare_func);
	if(keynodes->size() < 2){
		// LOG("keynodes->size() --> (%d)\n", keynodes->size());
		return;
	}

	// for(int i = 1; i < keynodes->size(); ++i){
	// 	if((*keynodes)[i]->_tag != NULL){
	// 		LOG("%d \"%s\"\n", (*keynodes)[i]->_id, (*keynodes)[i]->_tag);
	// 	}else{			
	// 		LOG("%d \"NULL\"\n", (*keynodes)[i]->_id);
	// 	}
	// }
	// LOG("keynodes->size() --> [%d]\n", keynodes->size());

	vector<mbt_node*> ct_nodes, vc1;
	vector<int> rg;
	vector< vector<int> > vc2;
	int flag = 0;
	for(int i = 0; i < keynodes->size(); ++i){
		ct_nodes.push_back((*keynodes)[i]);
	}
	for(int i = 1; i < keynodes->size(); ++i){
		flag = 0;
		mbt_node *tmp = T->get_lca( (*keynodes)[i-1], (*keynodes)[i]);

		// LOG("LCA: [%d] [%d] --> [%d]\n", (*keynodes)[i-1]->_id, (*keynodes)[i]->_id, tmp->_id);

		if(tmp->_tag != NULL && strcmp(tmp->_tag, key) == 0){
			break;
		}
		for(int j = 0; j < ct_nodes.size(); ++j){
			if(tmp->_id == (ct_nodes)[j]->_id){
				flag = 1;
				break;
			}
		}
		if(flag == 0){
			ct_nodes.push_back(tmp);
		}
	}

	// for(vector<mbt_node*>::iterator tit = keynodes->begin(); tit != keynodes->end(); ++tit){
	// 	flag = 0;
	// 	mbt_node *tmp = T->get_lca( *(tit-1), *tit);
	// 	if(tmp->_tag != NULL && strcmp(tmp->_tag, key) == 0){
	// 		break;
	// 	}
	// 	for(vector<mbt_node*>::iterator ttit = keynodes->begin(); ttit != keynodes->end(); ++ttit){
	// 		if(tmp->_id == (*ttit)->_id){
	// 			flag = 1;
	// 			break;
	// 		}
	// 	}
	// 	if(flag == 0){
	// 		ct_nodes.push_back(tmp);
	// 	}
	// }

	std::sort(ct_nodes.begin(), ct_nodes.end(), compare_func);

	// for(int i = 1; i < ct_nodes.size(); ++i){
	// 	if((ct_nodes)[i]->_tag != NULL){
	// 		LOG("%d \"%s\"\n", (ct_nodes)[i]->_id, (ct_nodes)[i]->_tag);
	// 	}else{			
	// 		LOG("%d \"NULL\"\n", (ct_nodes)[i]->_id);
	// 	}
	// }
	// LOG("ct_nodes.size() --> [%d]\n", ct_nodes.size());

	// delete dup node;
	for(vector<mbt_node*>::iterator vit = ct_nodes.begin(); vit != ct_nodes.end(); ++vit){
		if(vit == ct_nodes.begin()){
			continue;
		}
		if((*vit)->_id == (*(vit-1))->_id){
			ct_nodes.erase(vit);
			--vit;
		}
	}
	std::sort(ct_nodes.begin(), ct_nodes.end(), compare_func);
	// LOG("ct_nodes.size() --> [%d]\n", ct_nodes.size());

	// build ct;
	flag = 0;
	for(int i = 0; i < ct_nodes.size(); ++i){
		int a, b;
		T->get_range(ct_nodes[i], &a, &b);
		// rg = T->get_range(ct_nodes[i]);
		vector<int>().swap(rg);
		rg.push_back(a);
		rg.push_back(b);

		// LOG("[%d] range --> [%d] [%d]\n", ct_nodes[i]->_id, a, b);

		for(int j = vc1.size() - 1; j >= 0; --j){
			if(vc1.size() == 0){
				LOG_X;
				break;
			}
			if(is_joint(vc2[j], rg)){
				if(ctree->size() == 0){
					// LOG("add_root [%d]\n", vc1[j]->_id);
					ctree->add_root(copy_constructor(vc1[j]));
				}

				// LOG("add_node [%d] [%d]\n", ct_nodes[i]->_id, vc1[j]->_id);

				ctree->add_node(copy_constructor(ct_nodes[i]), vc1[j]->_id);
				
				// ++flag;
				// ctree->visit();
				// if(flag > 20){
				// 	exit(0);
				// }

				break;
			}
		}
		vc1.push_back(ct_nodes[i]);
		vc2.push_back(rg);
	}
}

mbt_node* CT::get_nn_node(mb_tree *T, vector<mbt_node*>* keynodes, mbt_node* n, const char *key){
	// 
	return ctree->find_nn_key_node(n, key);

	// init;
	vector<mbt_node*> candi, *children;
	mbt_node* parent = n->_parent;
	children = n->_children;
	if(parent != NULL && parent->_tag != NULL && (strcmp(parent->_tag, key) == 0)){
		candi.push_back(parent);
	}
	for(int i = 0; i < children->size(); ++i){
		if((*children)[i]->_tag != NULL && strcmp((*children)[i]->_tag, key) == 0)
			candi.push_back((*children)[i]);
	}

	// quick check;
	if(candi.size() == 0){
		// LOG_X;
		return NULL;
	}
	if(candi.size() == 1){
		return candi[0]; 
	}

	// compute the distance for each pair of [n, candi[i]];
	int min_pos = 0, min_dist = raw_tree->get_distance(n->_id, candi[0]->_id);
	for(int i = 1; i < candi.size(); ++i){
		int dist = raw_tree->get_distance(n->_id, candi[i]->_id);
		if(dist < min_dist){
			min_pos = i;
			min_dist = dist;
		}
	}
	return candi[min_pos];
}

///////////////////////////////
// class ECT;
///////////////////////////////
void ECT::build_ect(mb_tree *T, CT *ct, vector<mbt_node*>* keynodes, const char *key){
	ectree = ct->ctree;
	raw_tree = T;
	// init;
	vector<mbt_node*> *all_ct_nodes = ct->ctree->get_all_nodes();
	std::sort(all_ct_nodes->begin(), all_ct_nodes->end(), compare_func);
	
	// edges records the edges for ct in the order of id;
	// nn_edges records the nearest node for each edges in edges;
	mbt_node  *tmp1, *tmp2;
	vector<mbt_node*> *children, edges, nn_edges;
	for(int i = 0; i < all_ct_nodes->size(); ++i){
		children = (*all_ct_nodes)[i]->_children;
		std::sort(children->begin(), children->end(), compare_func);
		for(int j = 0; j < children->size(); ++j){
			if((*all_ct_nodes)[i]->_tag != NULL && strcmp((*all_ct_nodes)[i]->_tag, key) == 0){
				tmp1 = (*all_ct_nodes)[i];
			}else{
				tmp1 = ct->get_nn_node(T, keynodes, (*all_ct_nodes)[i], key);
			}
			if((*children)[j]->_tag != NULL && strcmp((*children)[j]->_tag, key) == 0){
				tmp2 = (*children)[j];
			}else{
				tmp2 = ct->get_nn_node(T, keynodes, (*children)[j], key);
			}
			if(tmp1 != tmp2){
				edges.push_back((*all_ct_nodes)[i]);
				edges.push_back((*children)[j]);
				nn_edges.push_back(tmp1);
				nn_edges.push_back(tmp2);

				// LOG("[%d] [%d] [%d] [%d] \n", (*all_ct_nodes)[i]->_id, (*children)[j]->_id, tmp1->_id, tmp2->_id);
			}
		}
	}
	// LOG("12345676654321\n");

	// get the extended node for each edges;
	int hops;
	mbt_node *nn_change;
	for(int i = 0; i < edges.size()/2; ++i){
		hops = (T->get_distance(edges[i*2]->_id,     nn_edges[i*2]->_id)         + 
		        T->get_distance(edges[i*2 + 1]->_id, nn_edges[i*2 + 1]->_id)     + 
		        T->get_distance(edges[i*2]->_id,     edges[i*2 + 1]->_id) - 1) / 2 - 
		       T->get_distance(edges[i*2 + 1]->_id, nn_edges[i*2 + 1]->_id);
		nn_change = edges[i*2 + 1];
		while(hops > 0){
			if(nn_change == NULL){
				LOG_X;
				exit(0);
			}
			// nn_change = nn_change->_parent;
			nn_change = T->get_parent(nn_change->_id);
			hops -= 1;
		}
		mbt_node *add_node = copy_constructor(nn_change);
		mbt_node *rem_node = edges[i*2 + 1];
		mbt_node *par_node = edges[i*2];

		// check;
		if(add_node->_id == rem_node->_id || add_node->_id == par_node->_id || rem_node->_id == par_node->_id){
			continue;
		}

		// printf("8549321 before update! [%d] [%d] [%d]\n", par_node->_id, add_node->_id, rem_node->_id);
		// par_node->visit(1, "");

		ct->ctree->remove_node(rem_node);
		ct->ctree->add_node(add_node, par_node->_id);
		ct->ctree->add_node(rem_node, add_node->_id);
		std::sort(add_node->_children->begin(), add_node->_children->end(), compare_func);
		std::sort(rem_node->_children->begin(), rem_node->_children->end(), compare_func);
		std::sort(par_node->_children->begin(), par_node->_children->end(), compare_func);
		// update the info inlcude: sort the _children, update the level;
		ct->ctree->update_info(par_node->_id);

		// printf("68745389281243 after update! [%d] [%d] [%d]\n", par_node->_id, add_node->_id, rem_node->_id);
		// par_node->visit(1, "");

	}
}

mbt_node *ECT::get_nn_node(vector<mbt_node*>* keynodes, int id, const char *key){
	mbt_node *node = ectree->find_node(id);

	if(node == NULL){
		LOG("[%d]\n", id);
		exit(0);
	}

	// if(node->_tag != NULL){
	// 	LOG("[%d] --> [%d] [%s]\n", id, node->_id, node->_tag);
	// }else{
	// 	LOG("[%d] --> [%d] \n", id, node->_id);
	// }

	return get_nn_node(keynodes, node, key);
}

mbt_node *ECT::get_nn_node(vector<mbt_node*>* keynodes, mbt_node* n, const char *key){
	// if(n->_tag != NULL){
	// 	LOG("[%d] [%s]\n", n->_id, n->_tag);
	// }else{
	// 	LOG("[%d] \n", n->_id);
	// }

	if(n->_tag != NULL && strcmp(n->_tag, key) == 0){
		return n;
	}
	mbt_node *parent = n->_parent;
	if(parent != NULL && parent->_tag != NULL && strcmp(parent->_tag, key) == 0){
		return parent;
	}
	vector<mbt_node*> *children = n->_children;
	std::sort(children->begin(), children->end(), compare_func);

	if(children->size() != 0 && (*children)[0]->_tag != NULL && strcmp((*children)[0]->_tag, key) == 0){
		return (*children)[0];
	}

	return ectree->find_nn_key_node(n, key);

	// if(children->size() == 0 || ((*children)[0]->_tag != NULL && strcmp((*children)[0]->_tag, key)) != 0){
	// 	LOG_X;
	// 	return NULL;
	// }
}

vector<mbt_node*> *ECT::get_children(int id){
	mbt_node *node = ectree->find_node(id);
	if(node == NULL){
		LOG_X;
		exit(0);
	}
	return node->_children;
}

vector<int> *ECT::get_children_id(int id){
	mbt_node *node = ectree->find_node(id);
	if(node == NULL){
		LOG_X;
		exit(0);
	}
	vector<int> *ret = new vector<int>;
	for(int i = 0; i < node->_children->size(); ++i){
		ret->push_back((*(node->_children))[i]->_id);
	}
	return ret;
}


/////////////////////////////////////////////
// class TVP;
/////////////////////////////////////////////
void TVP::build_tvp(mb_tree *T, ECT *ect, vector<mbt_node*>* keynodes, const char *key){
	// quick check;
	if(keynodes->size() == 0){
		return;
	}
	if(keynodes->size() == 1){
		return;
	}

	// init;
	raw_tree = T;
	vector<int> s;
	tvp_index = new vector<tvp_node>();
	copy_tree(ect->ectree);
	vector<mbt_node*> *all_ect_nodes = ect->ectree->get_all_nodes();
	vector<int> all_ect_nodes_id;
	for(int i = 0; i < all_ect_nodes->size(); ++i){
		all_ect_nodes_id.push_back((*all_ect_nodes)[i]->_id);
	}
	std::sort(all_ect_nodes_id.begin(), all_ect_nodes_id.end());
	// vector<int> s;
	vector<int>::iterator s_it;
	mbt_node *nn_node = NULL;
	vector<mbt_node*> *children = NULL;
	vector<int> *children_id = NULL;
	int pos = 0, a, b, c, d, flag;

	// LOG("584392143");
	// LOG("all_ect_nodes_id.size() --> [%d]\n", all_ect_nodes_id.size());

	for(int i = 0; i < all_ect_nodes_id.size(); ++i){

		// LOG("[%d] of [%d], id: [%d]\n",i, all_ect_nodes_id.size(), (all_ect_nodes_id)[i]);

		nn_node = NULL;
		nn_node = ect->get_nn_node(keynodes, (all_ect_nodes_id)[i], key);
		if(nn_node == NULL){
			LOG_X;
			exit(0);
		}

		// LOG("[%d] get_nn_node(%d, %s) --> [%d] [%s] \n", i, (all_ect_nodes_id)[i], key, nn_node->_id, nn_node->_tag);

		// vector<int> rg = T->get_range((*all_ect_nodes)[i]->_id);
		a = 0; b = 0;
		T->get_range((all_ect_nodes_id)[i], &a, &b);
		exchage_num(&a, &b);
		// if(rg.size() != 2){
		// 	LOG_X;
		// 	exit(0);
		// }
		s.push_back(a);
		s.push_back(b);
		s.push_back(nn_node->_id);

		children_id = adv_tree->get_children_id((all_ect_nodes_id)[i]);
		for(int j = 0; j < children_id->size(); ++j){
			c = 0; d = 0;
			T->get_range((*children_id)[j], &c, &d);
			exchage_num(&c, &d);
			// get the cover range in s;
			for(s_it = s.begin(); s_it != s.end(); ++s_it){
				pos = s_it - s.begin();
				if(pos % 3 == 0){
					if(s[pos] <= c && s[pos+1] >= d){
						break;
					}
				}
			}
			if(s_it == s.end()){
				continue;
			}
			// erase the old range and push the new one/two;
			if(c - s[pos] > 0){
				s.push_back(s[pos]);
				s.push_back(c - 1);
				s.push_back(nn_node->_id);
			}
			if(s[pos+1] - d > 0){
				s.push_back(d + 1);
				s.push_back(s[pos+1]);
				s.push_back(nn_node->_id);
			}
			s_it = s.begin();
			for(int k = 0; k < pos; ++k){
				++s_it;
			}
			s.erase(s_it, s_it + 3);
			// s.erase(s_it + 1);
			// s.erase(s_it);
		}
	}

	// LOG("54938276271843\n");
	// LOG("s.size() --> [%d]\n", s.size());

	// merge all the range in s if they are continuely;
	// for(s_it = s.begin(); s_it != s.end();){
	// 	// if(flag > 0){
	// 	// 	s.erase(s_it);
	// 	// 	s_it--;
	// 	// 	continue;
	// 	// }
	// 	pos = s_it - s.begin();
	// 	if(pos > 2 && pos % 3 == 0){
	// 		if(s[pos+2] == s[pos-1] && s[pos-2] + 1 == s[pos]){
	// 			s[pos-2] = s[pos+1];
	// 			s_it = s.erase(s_it, s_it + 3);
	// 			continue;
	// 		}
	// 	}
	// 	++s_it;
	// }

	vector<tvp_node> *tmp_tvp = new vector<tvp_node>();
	for(int i = 0; i < s.size(); ++i){
		if(i%3==0){
			tvp_node tn;
			tn.a = s[i];
			tn.b = s[i+1];
			tn.n = s[i+2];
			tmp_tvp->push_back(tn);
		}
	}

	// LOG("128495487391");
	// LOG("tmp_tvp->size() --> [%d]\n", tmp_tvp->size());

	std::sort(tmp_tvp->begin(), tmp_tvp->end(), compare_func_tvp_node);
	// merge all range;
	if(tmp_tvp->size() > 1){
		tvp_index->push_back((*tmp_tvp)[0]);
		int index_last = tvp_index->size() - 1;
		for(int i = 1; i < tmp_tvp->size(); ++i){
			if((*tvp_index)[index_last].n == (*tmp_tvp)[i].n && (*tvp_index)[index_last].b + 1 == (*tmp_tvp)[i].a){
				(*tvp_index)[index_last].b = (*tmp_tvp)[i].b;
			}else{
				tvp_index->push_back((*tmp_tvp)[i]);
			}
			index_last = tvp_index->size() - 1;
		}
	}

	// LOG("59438u1");

}

void TVP::copy_tree(mb_tree *t){
	adv_tree = new mb_tree();
	mbt_node *node;
	vector<mbt_node*> *all_nodes = t->get_all_nodes();

	for(int i = 0; i < all_nodes->size(); ++i){
		node = copy_constructor((*all_nodes)[i]);
		if(adv_tree->size() == 0){
			adv_tree->add_root(node);
		}else{
			adv_tree->add_node(node, (*all_nodes)[i]->_parent->_id);
		}
	}



}

// void TVP::show(){
// 	for(int i = 0; i < s.size(); ++i){
// 		if(i%3==0){
// 			printf("\n");
// 		}
// 		printf("%d  ", (s)[i]);
// 	}
// 	printf("\n");
// }

mbt_node *TVP::nn_search(int id){
	for(int i = 0; i < tvp_index->size(); ++i){
		if((*tvp_index)[i].a <= id and id <= (*tvp_index)[i].b){
			return raw_tree->find_node((*tvp_index)[i].n);
		}
	}
	return NULL;
}

int TVP::nn_search_id(int id){
	for(int i = 0; i < tvp_index->size(); ++i){
		if((*tvp_index)[i].a <= id and id <= (*tvp_index)[i].b){
			return ((*tvp_index)[i].n);
		}
	}
	return -1;
}


void TVP::show(){
	for(int i = 0; i < tvp_index->size(); ++i){
		printf("%d  %d  -->  %d\n", (*tvp_index)[i].a, (*tvp_index)[i].b, (*tvp_index)[i].n );
	}
	printf("\n");
}


void TVP::flush(int fd){
	char buf[16384];
	size_t len = 0;
	for(int i = 0; i < tvp_index->size(); ++i){
		len = strlen(buf);
		if(len > 16000){
			write(fd, buf, len);
			fsync(fd);
			memset(buf, '\0', 16384);
			len = 0;
		}
		sprintf(buf + len, "%d?%d?%d?", (*tvp_index)[i].a, (*tvp_index)[i].b, (*tvp_index)[i].n);
	}
	len = strlen(buf);
	sprintf(buf + len, "\n");
	len = strlen(buf);
	write(fd, buf, len);
	fsync(fd);
}

string TVP::flush(){
	string ret;
	for(int i = 0; i < tvp_index->size(); ++i){
		ret += std::to_string((*tvp_index)[i].a);
		ret += "?";
		ret += std::to_string((*tvp_index)[i].b);
		ret += "?";
		ret += std::to_string((*tvp_index)[i].n);
		ret += "?";
	}
	return ret;
}


