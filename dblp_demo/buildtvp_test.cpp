#include <stdio.h>
#include <stdlib.h>
#include "buildtvp.h"

void test_buildtvp(mb_tree *t){
	// build ct first;
	CT *ct = new CT();
	vector<mbt_node*> *keynodes = t->find_key_nodes("t");
	// for(int i = 0; i < keynodes->size(); ++i){
	// 	printf("%d  ", (*keynodes)[i]->_id);
	// }
	ct->build_ct(t, keynodes, "t");

	// build ect;
	ECT *ect = new ECT();
	ect->build_ect(t, ct, keynodes, "t");

	// build tvp;
	TVP *tvp = new TVP();
	tvp->build_tvp(t, ect, keynodes, "t");
	tvp->show();
	// for(int i = 0; i < tvp->s.size(); ++i){
	// 	if(i%3==0){
	// 		printf("\n");
	// 	}
	// 	printf("%d  ", (tvp->s)[i]);
		
	// }
}

void test_buildect(mb_tree *t){
	// build ct first;
	CT *ct = new CT();
	vector<mbt_node*> *keynodes = t->find_key_nodes("t");
	for(int i = 0; i < keynodes->size(); ++i){
		printf("%d  ", (*keynodes)[i]->_id);
	}
	ct->build_ct(t, keynodes, "t");

	// build ect;
	ECT *ect = new ECT();
	ect->build_ect(t, ct, keynodes, "t");
	ect->ectree->visit();
}

void test_buildct(mb_tree *t){
	CT ct;

	vector<mbt_node*> *keynodes = t->find_key_nodes("t");
	for(int i = 0; i < keynodes->size(); ++i){
		printf("%d  ", (*keynodes)[i]->_id);
	}
	ct.build_ct(t, keynodes, "t");
	ct.ctree->visit();
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
	mb_tree *t = test_tree_gen();
	t->visit(2);

	// test_buildct(t);
	// test_buildect(t);
	test_buildtvp(t);



	return 0;
}