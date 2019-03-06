#ifndef BITCOIN_STRUCT
#define BITCOIN_STRUCT

#include "../tree_node/tree_node.hpp"

class tree_node;

typedef struct{
  unsigned int succ_trans;
  tree_node *root;
}bitcoin_struct;

#endif
