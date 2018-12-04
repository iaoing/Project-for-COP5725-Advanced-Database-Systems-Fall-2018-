#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "mb_tree.h"

using std::vector;

void test_remove_node(mb_tree *t){
	mbt_node *add = new mbt_node(99);
	mbt_node *rem = t->find_node(18);
	mbt_node *par = rem->_parent;
	t->remove_node(rem);
	t->add_node(add, par->_id);
	t->add_node(rem, add->_id);
	t->visit();
}

void test_is_joint(mb_tree *t){
	int ret;
	ret = t->is_joint(t->find_node(17), t->find_node(2));
	printf("%d\n", ret);
	ret = t->is_joint(t->find_node(17), t->find_node(25));
	printf("%d\n", ret);
	ret = t->is_joint(t->find_node(12), t->find_node(13));
	printf("%d\n", ret);
	ret = t->is_joint(t->find_node(1), t->find_node(31));
	printf("%d\n", ret);
}

void test_get_all_nodes(mb_tree *t){
	vector<mbt_node*> *ret_vc = t->get_all_nodes();
	for(int i = 0; i < ret_vc->size(); ++i){
		printf("%d  ", (*ret_vc)[i]->_id);
	}
	printf("\n%d\n", ret_vc->size());
}

void test_get_rang(mb_tree *t){
	int a, b;
	t->get_range(18, &a, &b);
	// if(ret.size() == 2){
		printf("[%d], [%d]\n", a, b);
	// }else{
		// printf("NULL %d\n", ret.size());
	// }
}

void test_get_distance(mb_tree *t){
	int dis;
	dis = t->get_distance(t->find_node(8), t->find_node(11));
	printf("%d\n", dis);
}

void test_get_lca(mb_tree *t){
	mbt_node *n;
	n = t->get_lca(t->find_node(30), t->find_node(30));
	if(n == NULL){
		printf("NULL\n");
	}else{
		n->show(1);
	}
}

void test_get_parent(mb_tree *t){
	mbt_node *node;
	node = t->get_parent(23);
	if(node != NULL){
		node->show(1);
	}else{
		printf("NULL\n");
	}
	node = t->get_parent(14);
	if(node != NULL){
		node->show(1);
	}else{
		printf("NULL\n");
	}
	node = t->get_parent(65);
	if(node != NULL){
		node->show(1);
	}else{
		printf("NULL\n");
	}
	node = t->get_parent(0);
	if(node != NULL){
		node->show(1);
	}else{
		printf("NULL\n");
	}
}

void test_find_key_nodes(mb_tree *t){
	vector<mbt_node*> *ret_vc = t->find_key_nodes("t");
	LOG("%d\n", ret_vc->size());
	for(int i = 0; i < ret_vc->size(); ++i){
		(*ret_vc)[i]->show(1);
	}
}

void test_find_node(mb_tree *t){
	mbt_node *node;
 	node = t->find_node(23);
 	if(node != NULL)
	 	node->show(1);
 	node = t->find_node(1);
 	if(node != NULL)
	 	node->show(1);
 	node = t->find_node(31);
 	if(node != NULL)
	 	node->show(1);
 	node = t->find_node(32);
 	if(node != NULL)
	 	node->show(1);
 	node = t->find_node(0);
 	if(node != NULL)
	 	node->show(1);
}

void test_find_nn_key_node(mb_tree *t){
	mbt_node *node = t->find_nn_key_node(t->find_node(12), "t");
	if(node == NULL)
		printf("NULL\n");
	else
		node->show(1);
	node = t->find_nn_key_node(t->find_node(8), "t");
	if(node == NULL)
		printf("NULL\n");
	else
		node->show(1);
	node = t->find_nn_key_node(t->find_node(24), "t");
	if(node == NULL)
		printf("NULL\n");
	else
		node->show(1);
	node = t->find_nn_key_node(t->find_node(0), "t");
	if(node == NULL)
		printf("NULL\n");
	else
		node->show(1);
}

mb_tree *test_tree_gen(){
	mb_tree *t = new mb_tree();
	t->add_root(new mbt_node( 1));

	t->add_node(new mbt_node( 2, "t"), 1);
	t->add_node(new mbt_node( 3), 2);
	t->add_node(new mbt_node( 4), 3);
	t->add_node(new mbt_node( 5, "t"), 4);
	t->add_node(new mbt_node( 6), 4);
	t->add_node(new mbt_node( 7), 3);
	t->add_node(new mbt_node( 8), 7);
	t->add_node(new mbt_node( 9, "t"), 7);
	t->add_node(new mbt_node(10), 2);
	t->add_node(new mbt_node(11), 10);
	t->add_node(new mbt_node(12), 11);
	t->add_node(new mbt_node(13), 11);
	t->add_node(new mbt_node(14), 10);
	t->add_node(new mbt_node(15), 14);
	t->add_node(new mbt_node(16), 14);

	t->add_node(new mbt_node(17), 1);
	t->add_node(new mbt_node(18), 17);
	t->add_node(new mbt_node(19), 18);
	t->add_node(new mbt_node(20), 19);
	t->add_node(new mbt_node(21), 19);
	t->add_node(new mbt_node(22), 18);
	t->add_node(new mbt_node(23, "t"), 22);
	t->add_node(new mbt_node(24), 22);
	t->add_node(new mbt_node(25), 17);
	t->add_node(new mbt_node(26), 25);
	t->add_node(new mbt_node(27), 26);
	t->add_node(new mbt_node(28), 26);
	t->add_node(new mbt_node(29), 25);
	t->add_node(new mbt_node(30), 29);
	t->add_node(new mbt_node(31), 29);

	return t;
}

int main(int argc, char const *argv[])
{
	// add root;
	// char tag[] = "1";
	// mbt_node *node;
	// node = new mbt_node(1, 0, tag);
	// mb_tree *t = new mb_tree();
	// t->add_root(node);

	// // add nodes;
	// node = new mbt_node(2, 1, "2");
	// t->add_node(node, 1);

	// t->visit(2);

	mb_tree *t = test_tree_gen();
	t->visit(2);

	// test_find_node(t);
	// test_find_key_nodes(t);
	// test_get_parent(t);
	// test_find_nn_key_node(t);
	// test_get_lca(t);
	// test_get_distance(t);
	test_get_rang(t);
	// test_get_all_nodes(t);
	// test_is_joint(t);
	// test_remove_node(t);



	return 0;
}