#include <string>
#include <iostream>
#include <boost/timer.hpp>
#include "core.h"
#include "debug_log.h"

using std::cin;
using std::string;
using boost::timer;

const char *get_input(){
	char buf[512];
	memset(buf, 0, 512);
	cin.getline(buf, 512);
	int i = 0;
	for(; i < strlen(buf); ++i){
		if(buf[i] != ' ')
			break;
	}
	// if(buf[i] == EOF){
	// 	user_exit();
	// }
	return string(buf+i).c_str();
}

mbt_node *find_year(core *cc, char *year, const char *key){
	printf("find_year: [%s]\n", year);
	vector<mbt_node*> *vc = cc->_tree->find_key_nodes(key);
	for(int i = 0; i < vc->size(); ++i){

		// if((*vc)[i]->_children->size() > 0 && (*((*vc)[i]->_children))[0]->_tag != NULL){
		// 	printf("[%s]\n", (*((*vc)[i]->_children))[0]->_tag);
		// }else{
		// 	printf("NULL\n");
		// }

		if((*vc)[i]->_children->size() > 0 
			&& (*((*vc)[i]->_children))[0]->_tag != NULL
			&& strcmp((*((*vc)[i]->_children))[0]->_tag, year) == 0){
			return ((*((*vc)[i]->_children))[0]);
		}
	}
	return 0;
}

mbt_node *find_given_node(core *cc, const char *bufxx){
	char *buf = (char*)malloc(strlen(bufxx) + 2);
	sprintf(buf, "%s%c", bufxx, '\0');

	char *tok;
	int id;
	mbt_node *node;

	// printf("12323: %s %s\n", bufxx, buf);
	tok      = strtok(buf,  "|");
	tok      = strtok(NULL, "|");
	id = atoi(tok);

	node = cc->_tree->find_node(id);
	return node;

}

void find_nn_node(core *cc, mbt_node *q_node, const char *bufxx){
	char *buf = (char*)malloc(strlen(bufxx) + 2);
	sprintf(buf, "%s%c", bufxx, '\0');

	int nnid = -1;
	char *tok, *key;
	mbt_node *node;

	// printf("12323: %s %s\n", bufxx, buf);
	tok      = strtok(buf,  "|");
	key      = strtok(NULL, "|");

	printf("find_nn_node [%d]  [%s]\n", q_node->_id, key);
	nnid = cc->find_nn_node(q_node->_id, key);
	
	if(nnid < 0){
		printf("nn search failed!\n");
		return;
	}

	node = cc->_tree->find_node(nnid);
	if(node == NULL){
		printf("find target node failed. node id: %d\n", nnid);
		return;
	}

	node->show(1);

}

void find_nn_node_xx(core *cc, mbt_node *q_node, const char *bufxx){
	char *buf = (char*)malloc(strlen(bufxx) + 2);
	sprintf(buf, "%s%c", bufxx, '\0');

	int nnid = -1;
	char *tok, *key;
	mbt_node *node;

	// printf("12323: %s %s\n", bufxx, buf);
	tok      = strtok(buf,  "|");
	key      = strtok(NULL, "|");

	printf("find_nn_node [%d]  [%s]\n", q_node->_id, key);
	node = cc->_tree->find_nn_key_node(q_node->_id, key);
	
	// if(nnid < 0){
	// 	printf("nn search failed!\n");
	// 	return;
	// }

	// node = cc->_tree->find_node(nnid);
	if(node == NULL){
		printf("find target node failed. node id: %d\n", nnid);
		return;
	}

	node->show(1);
}

void p_menu(){
	printf("==================================\n");
	printf("q|node_id|\n");
	printf("n|keyword|\n");
	printf("x|keyword|\n");
	printf("==================================\n\n");
}

int main(int argc, char const *argv[])
{
	// build _tree;
	timer t2;
	core *cc = new core("nba_raw_data.log");
	cc->build_tree();
	printf("build tree time: %f s\n\n", t2.elapsed());
	// cc->_tree->visit();
	// cc->_tree->show_keys();
	// printf("#### %d\n", cc->_keywords->size());


	printf("print 'B' to build index.\n");
	printf("print 'L' to load index from a exist file.\n");

	const char *tttmp;
	tttmp = get_input();
	if(tttmp[0] == 'L'){
		timer t1;
		cc->load_index();
		printf("load index time: %f s\n\n", t1.elapsed());
		// cc->show_tvp();
	}else{
		// build a tvp;
		// cc->build_tvp("Matt Mazza");
		// cc->show_tvp();
		// cc->build_tvp("Max Morris");
		// cc->show_tvp();
		// cc->build_tvp("Northwestern University");
		// cc->show_tvp();
		timer t1;
		cc->build_index();
		
		// show;
		LOG("build_index finished!!!!\n");
		printf("build index time: %f s\n\n", t1.elapsed());
		// cc->show_tvp();

		// flush index;
		// cc->flush_index();
		// LOG("flush index finished!\n");
		// printf("flush index time: %f s\n\n", t.elapsed());
	}

	
	printf("\nnumber of nodes: [%d]\n", cc->_tree->_nb_nodes);
	printf("\nnumber of keywords: [%zu]\n", cc->_tree->_keywords->size());



	p_menu();


	const char *cmd_buf;
	mbt_node *node;
	while(1){
		cmd_buf = get_input();
		if(cmd_buf[0] == 'q'){
			timer t1;
			node = find_given_node(cc, string(cmd_buf).c_str());
			if(node == NULL){
				printf("Not found!\n");
				continue;
			}else{
				if(node->is_leaf() && node->_parent != NULL && node->_parent->_tag != NULL){
					printf("%s --> ", node->_parent->_tag);
				}
				node->show(1);
				printf("find node time: %f s\n\n", t1.elapsed());
			}
		}else if(cmd_buf[0] == 'n'){
			timer t1;
			find_nn_node(cc, node, string(cmd_buf).c_str());
			printf("nn search time: %f s\n\n", t1.elapsed());
		}else if(cmd_buf[0] == 'x'){
			timer t1;
			find_nn_node_xx(cc, node, string(cmd_buf).c_str());
			printf("nn x search time: %f s\n\n", t1.elapsed());
		}else{
			printf("do you want to exit? Y/N\n");
			cmd_buf = get_input();
			if(cmd_buf[0] == 'Y' || cmd_buf[0] == 'y'){
				exit(0);
			}
		}
	}


	return 0;
}