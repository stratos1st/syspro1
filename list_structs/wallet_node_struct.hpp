#ifndef WALLET_NODE_STRUCT
#define WALLET_NODE_STRUCT

#include "../tree_node/tree_node.hpp"
#include "../bitcoin_struct/bitcoin_struct.hpp"

class tree_node;
class bitcoin_struct;

class wallet_node_struct{
public:
  wallet_node_struct *next;//pointer to the next node
  tree_node *leaf_node;//pointer to leaf_node that coresonds to money owned from a bitcoin
  bitcoin_struct *bitcoin;//pointer to bitcoin struct (needed to increase bitcoin->succ_trans)

  wallet_node_struct();
  ~wallet_node_struct();//deletes next
};
#endif
