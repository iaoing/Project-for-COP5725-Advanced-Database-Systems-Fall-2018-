#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <boost/timer.hpp>
#include "debug_log.h"
#include "mb_tree.h"
#include "buildtvp.h"
#include "core.h"

using std::string;
using std::map;
using std::vector;
using boost::timer;

/*const char *nba_nonkey[35] = {"NBA", "league*", "team*", "year", "tname", "players", "division",
					  "player*", "pname", "position", "college",
					  "Oregon State University", "Northwestern University", "University of Wisconsin",
					  "Kansas State University", "University of Tennessee", "University of Minnesota",
					  "Jacksonville University", "Northeastern University", "Murray State University",
					  "California State University, Long Beach", "Alcorn State University",
					  "University of California, Santa Barbara", "Gardner-Webb University",
					  "Albany State University", "Coppin State University", "Morgan State University",
					  "Old Dominion University", "Millersville University of Pennsylvania",
					  "Grand Canyon University", "Oral Roberts University", "George Mason University",
					  "Wright State University", "Michael Carter-Williams", "Rondae Hollis-Jefferson"};*/

const char *nba_nonkey[11] = {"NBA", "league*", "team*", "year", "tname", "players", "division",
					  "player*", "pname", "position", "college"};

const char *dblp_nonkey[6] = {"DBLP", "inproceedings*", "year", "booktitle", "title", "author*"};



/////////////////////////////////////
// class core;
/////////////////////////////////////
void core::add_node(char *buf){
	char *tag, *tok;
	int id, pid;
	tag = strtok(buf, "?");
	tok=strtok(NULL, "?");
	id = atoi(tok);
	tok=strtok(NULL, "?");
	pid=atoi(tok);
	// LOG("\n\nLine: %d. [%d] [%s] [%d]\n", __LINE__, id, tag, pid);
	if(pid < 0){
		_tree->add_root(new mbt_node( id, tag));
	}else{
		_tree->add_node(new mbt_node( id, tag), pid);
	}
}

void core::build_tree(){
	// check;
	if(_fname == NULL || strlen(_fname) == 0){
		printf("strlen(_fname) == 0\n");
		return;
	}

	// build _tree;
	FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

	fp = fopen(_fname, "r");
    if (fp == NULL){
        LOG_X;
        exit(0);
    }

    while ((read = getline(&line, &len, fp)) != -1) {
		add_node(line);  
    }
    fclose(fp);
    if(line)
    	free(line);

    // add _keywords;
    _keywords = _tree->_keywords;
}

void core::build_tvp(const char *key){
	timer tcb1;
	// LOG("2435849383 key: [%s]\n", key);

	// check;
	if(is_nonkey(key)){
		// LOG("4320912345 [%s] is not a keyword for build tvp\n", key);
		return;
	}
	// LOG("7854321848331\n");

	// allocate memory and store key;
	char *buf = (char*)malloc(strlen(key) + 2);
	if(buf == NULL){
		LOG("buf == NULL!\n");
		exit(0);
	}
	sprintf(buf, "%s%c", key, '\0');
	// LOG("12385994218\n")

	// get keywords;
	std::vector<mbt_node*> *keywords = new std::vector<mbt_node*>;
	// LOG("34593218234012\n");
	_tree->find_key_nodes(keywords, buf);
	// vector<mbt_node*> *keywords = _tree->find_key_nodes(key);
	// LOG("keywords->size() --> [%d]\n", keywords->size());
	// LOG("243895u901243\n");


	// LOG("keywords size: [%zu]\n", keywords->size());


	// quick check;
	if(keywords->size() == 0)
		return;
	if(keywords->size() == 1){
		_one->insert(pair<char*, int>(buf, (*keywords)[0]->_id));
		return;
	}
	// LOG("17859342812\n");


	// build ct;
	CT *ct = new CT();
	ct->build_ct(_tree, keywords, key);
	// LOG("67543213456\n");
	// ct->ctree->visit();

	// build ect;
	ECT *ect = new ECT();
	ect->build_ect(_tree, ct, keywords, key);
	// LOG("7654123456778\n");
	// ect->ectree->visit();


	// build tvp;
	TVP *tvp = new TVP();
	tvp->build_tvp(_tree, ect, keywords, key);
	// LOG("43872271823\n");
	// tvp->show();

	// insert to index;
	_index->insert(pair<char*, TVP*>(buf, tvp));

	LOG("key: [%s], size: [%zu], build time: [%f s]\n", key, keywords->size(), tcb1.elapsed());
}

int core::find_nn_node(int id, char *key){
	// map<char*, int>::iterator iit = _one->find(key);
	// if(iit != _one->end()){
	// 	return iit->second;
	// }else{
	// 	printf("key: [%s] not found in _one!\n", key);
	// }

	// map<char*, TVP*>::iterator it = _index->find(key);
	// if(it != _index->end()){
	// 	return it->second->nn_search_id(id);
	// }else{
	// 	printf("key: [%s] not found in _index!\n", key);
	// }

	map<char*, int>::iterator iit = _one->begin();
	for(; iit != _one->end(); ++iit){
		if(strcmp(iit->first, key) == 0){
			return iit->second;
		}
	}

	// printf("[%s] not in _one\n", key);

	map<char*, TVP*>::iterator it = _index->begin();
	for(; it != _index->end(); ++it){
		if(strcmp(it->first, key) == 0){
			return it->second->nn_search_id(id);
		}
	}

	// printf("[%s] not in _one\n", key);

	return -1;
	
}

void core::build_index(){
	for(int i = 0; i < _keywords->size(); ++i){
		if((*_keywords)[i] != NULL){
			// LOG("\n[%d] of [%zu] --> [%s]\n", i, _keywords->size(), (*_keywords)[i]);
			build_tvp((*_keywords)[i]);
		}
	}
}

int core::is_nonkey(const char *key){
	for(int i = 0; i < nonkey_len; ++i){

		// LOG("[%s] [%s]\n", key, nonkey[i]);

		if(strcmp(key, nonkey[i]) == 0){
			return 1;
		}
	}
	return 0;
}

void core::flush_index(){
	char iname[512];
	sprintf(iname, "%s.tvp.index%c", _fname, '\0');
	int fd = open(iname, O_RDWR | O_APPEND | O_CREAT, 0666);
	if(fd < 0){
		printf("open index file failed!\n");
		LOG_X;
		exit(0);
	}


	char buf[128];
	map<char*, int>::iterator itr = _one->begin();
	for(; itr != _one->end(); ++itr){
		memset(buf, '\0', 128);
		sprintf(buf, "%s?%d?\n", itr->first, itr->second);
		write(fd, buf, strlen(buf));
	}

	map<char*, TVP*>::iterator it = _index->begin();
	for(; it != _index->end(); ++it){
		memset(buf, '\0', 128);
		sprintf(buf, "%s?", it->first);
		string str;
		str += string(buf);
		str += it->second->flush();
		str += "\n";
		write(fd, str.c_str(), str.size());
		// fsync(fd);
		// write(fd, buf, strlen(buf));
		// fsync(fd);
		// it->second->flush(fd);
	}

	fsync(fd);
	close(fd);
}


void core::load_index(){
	char iname[512];
	sprintf(iname, "%s.tvp.index%c", _fname, '\0');
	std::ifstream fin(iname, std::ios::in);
	if (!fin){
		printf("Open %s failed!\n", iname);
		exit(1);
	}
	std::string line;
	while(getline(fin, line)){
		if(line.size() < 8){
			// LOG("182933EDOI1 line: [%s]\n", line.c_str());
			continue;
		}
		load_line(line.c_str());
	}

	fin.close();
}

void core::load_line(const char *line){
	TVP *tvp = new TVP();
	tvp->raw_tree = NULL;
	tvp->adv_tree = NULL;
	tvp->tvp_index = new vector<tvp_node>;

	char buf[strlen(line) + 2];
	sprintf(buf, "%s%c", line, '\0');

	char *tok, *key, *skey;
	int a, b, n, cnt = 0;

	for(int i = 0; i < strlen(buf); ++i){
		if(buf[i] == '?'){
			++cnt;
		}
	}

	// LOG("748532918 cnt: [%d] [%s]\n", cnt, buf);

	if(cnt < 2){
		LOG("12342313 load failed! line: [%s]\n", buf);
		return;
	}

	key = strtok(buf, "?");

	skey = (char*)malloc(strlen(key) + 2);
	sprintf(skey, "%s%c", key, '\0');
	// LOG("[%s] [%s]\n", skey, buf);

	if(cnt == 2){
		tok = strtok(NULL, "?");
		n = atoi(tok);
		_one->insert(pair<char*, int>(skey, n));
		return;
	}

	if(cnt < 4 || (cnt - 1) % 3 != 0){
		LOG("455412343 load failed! line: [%s]\n", buf);
		return;
	}

	// LOG("1234895289 push to. line: [%s] [%d]\n", line, ((cnt - 1) / 3));
	
	// tok = strtok(NULL, "?");
	// a = atoi(tok);	
	// tok = strtok(NULL, "?");
	// b = atoi(tok);
	// tok = strtok(NULL, "?");
	// n = atoi(tok);
	// tvp_node tnode;
	// tnode.a = a; tnode.b = b; tnode.n = n;

	// LOG("[%d] [%d] [%d]\n", a, b, n);

	// tvp->tvp_index->push_back(tnode);

	for(int i = 0; i < ((cnt - 1) / 3); ++i){
		tok = strtok(NULL, "?");
		if(tok == NULL){
			LOG_X;
			break;
		}
		a = atoi(tok);	
		tok = strtok(NULL, "?");
		if(tok == NULL){
			LOG_X;
			break;
		}
		b = atoi(tok);
		tok = strtok(NULL, "?");
		if(tok == NULL){
			LOG_X;
			break;
		}
		n = atoi(tok);
		tvp_node tnode;
		tnode.a = a; tnode.b = b; tnode.n = n;

		// LOG("[%d] of [%d] --> [%d] [%d] [%d]\n", i, ((cnt - 1) / 3), a, b, n);

		tvp->tvp_index->push_back(tnode);
	}

	// LOG("1u28934u91283");

	_index->insert(pair<char*, TVP*>(skey, tvp));

}

///////////////////////////////////////////
// for debug;
///////////////////////////////////////////
void core::show_tvp(){
	// traverse _index;
	map<char*, TVP*>::iterator it = _index->begin();
	for(; it != _index->end(); ++it){
		printf("\n\n++++++++++++%s++++++++++++++++\n", it->first);
			it->second->show();
		printf("\n----------------------------\n\n");
	}

	// traverse _one;
	printf("\n----------------------------\n\n");
	map<char*, int>::iterator itr = _one->begin();
	for(; itr != _one->end(); ++itr){
		printf("\n\n+++++++++++++%s+++++++++++++++\n", itr->first);
			printf("-1  -1  -->  %d\n", itr->second);
		// printf("\n----------------------------\n\n");
	}
}

