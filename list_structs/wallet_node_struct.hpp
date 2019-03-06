#ifndef WALLET_NODE_STRUCT
#define WALLET_NODE_STRUCT

#include "../tree_node/tree_node.hpp"
#include "../bitcoin_struct/bitcoin_struct.hpp"

struct a{
  struct a *next;
  tree_node *leaf_node;
  bitcoin_struct *bitcoin;
};

typedef struct a wallet_node_struct;

#endif
