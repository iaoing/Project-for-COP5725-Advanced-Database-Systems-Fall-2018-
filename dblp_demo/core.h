#ifndef __NNS_CORE_H__
#define __NNS_CORE_H__


#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include "mb_tree.h"
#include "buildtvp.h"

using std::map;
using std::pair;
using std::string;



extern const char *nba_nonkey[11];
extern const char *dblp_nonkey[7];



class core
{
public:
	core(){
		_tree = new mb_tree();
		_keywords = new vector<char*>();
		_index = new map<char*, TVP*>();
		_one = new map<char*, int>();
		memset(_fname, 0, sizeof(_fname));
	};

	core(const char *path){
		_tree = new mb_tree();
		_keywords = new vector<char*>();
		_index = new map<char*, TVP*>();
		_one = new map<char*, int>();
		memset(_fname, 0, sizeof(_fname));
		sprintf(_fname, "%s%c", path, '\0');

		if(strncmp(path, "nba", strlen("nba")) == 0){
			nonkey = nba_nonkey;
			nonkey_len = 11;
		}else if(strncmp(path, "dblp", strlen("dblp")) == 0){
			nonkey = dblp_nonkey;
			nonkey_len = 7;
		}
	};

	~core();

public:
	void build_tree();
	void build_tvp(const char *key);
	void build_index();
	int find_nn_node(int id, char *key);
	void flush_index();
	void load_index();
	void load_line(const char *line);

private:
	int is_nonkey(const char *key);
	void add_node(char *buf, int line_num, int *parent_id, int *nodes_cnt);
	void add_lines(vector<string> *lines, int *nodes_cnt);

public:
	mb_tree *_tree;
	vector<char*> *_keywords;
	map<char*, TVP*> *_index;
	map<char*, int> *_one;
	const char **nonkey;
	int nonkey_len;
	char _fname[512];

// for debug;
public:
	void show_tvp();
};




#endif