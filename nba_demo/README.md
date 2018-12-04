# project for database

#### Bing Jiao     bj18c@my.fsu.edu

#### source code


1. ./buildtvp.cpp: the main code for building index scheme:
  	- flow: tree --> CT --> ECT --> TVP
  	- TVP (Tree Voronoi Partition) is the class for building the fianl index for a tree-based dataset
  	- ECT (Extended Compact Tree) is the class for building the extended compact tree for a tree-basted dataset
  	- CT (Compact Tree) is the class for building the compact tree for a tree-basted dataset

2. ./core.cpp: the main code for building a index for nba-datset

3. ./mb_tree.cpp: mulit-branch tree for this project

4. ./debuf_log.h: write debug information to strerr for debug. You can use it just `include` it and then `LOG(SOME_INFO)`, and then `./core_tets 2>err.log` to flush the strerr to a log file.



#### how to install
```bash
$make
```

#### how to use
```bash
$./core_test
# this application will build a tree firstly
# then it will load the index or build index that depand on your select

# input 'q|id_numer|' to given a node, eg:
$q|72648|

# input 'n|keyword|' for a nearest query query, eg:
$n|Kobe Bean Bryant|
# this will return a node assoicated with keyword which is nearest for node-72648 by the index;

# input 'x|keyword|' for a nearest query, eg:
$x|Kobe Bean Bryant|
# this will return a node assoicated with keyword which is nearest for node-72648 by the BFT;
```

