#ifndef BITCOIN_STRUCT
#define BITCOIN_STRUCT

#include "../tree_node/tree_node.hpp"

class tree_node;

typedef struct{
  char coin_id[51];
  unsigned int succ_trans;
  tree_node *root;
}bitcoin_struct;

#endif
