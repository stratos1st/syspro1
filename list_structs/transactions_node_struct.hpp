#ifndef TRANSACTIONS_NODE_STRUCT
#define TRANSACTIONS_NODE_STRUCT

#include "../tree_node/tree_node.hpp"
#include "../bitcoin_struct/bitcoin_struct.hpp"

class transactions_node_struct{// TODO make it class for destructor
public:
  transactions_node_struct *next;
  transaction_struct *trans;

  ~transactions_node_struct(){
    if(next!=nullptr)
      delete next;
  }
};

#endif
