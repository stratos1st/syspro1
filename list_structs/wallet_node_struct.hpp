#ifndef WALLET_NODE_STRUCT
#define WALLET_NODE_STRUCT

#include "../tree_node/tree_node.hpp"
#include "../bitcoin_struct/bitcoin_struct.hpp"

class tree_node;
class bitcoin_struct;

class wallet_node_struct{
public:
  wallet_node_struct *next;
  tree_node *leaf_node;
  bitcoin_struct *bitcoin;

  wallet_node_struct();
  ~wallet_node_struct();
};


#endif
