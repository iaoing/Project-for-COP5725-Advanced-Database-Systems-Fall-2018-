# project for database

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

