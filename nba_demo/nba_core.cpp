#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "debug_log.h"
#include "mb_tree.h"

void add_node(mb_tree *t, char *buf){
	char *tag, *tok;
	int id, pid;
	tag = strtok(buf, "?");
	tok=strtok(NULL, "?");
	id = atoi(tok);
	tok=strtok(NULL, "?");
	pid=atoi(tok);
	LOG("\n\nLine: %d. [%d] [%s] [%d]\n", __LINE__, id, tag, pid);
	if(pid < 0){
		t->add_root(new mbt_node( id, tag));
	}else{
		t->add_node(new mbt_node( id, tag), pid);
	}
}

void build_tree(mb_tree *t){
	FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

	fp = fopen("./NBA_raw_data.log", "r");
    if (fp == NULL){
        LOG_X;
        exit(0);
    }

    while ((read = getline(&line, &len, fp)) != -1) {
        // printf("Retrieved line of length %zu:\n", read);
        // printf("%s", line);
		add_node(t, line);  
    }
    fclose(fp);
    if(line)
    	free(line);
}


int main(int argc, char const *argv[])
{
	mb_tree *t = new mb_tree();
	build_tree(t);

	t->visit();
	t->show_keys();



	return 0;
}
