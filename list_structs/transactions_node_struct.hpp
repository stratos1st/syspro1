#ifndef TRANSACTIONS_NODE_STRUCT
#define TRANSACTIONS_NODE_STRUCT

#include "../tree_node/tree_node.hpp"
#include "../bitcoin_struct/bitcoin_struct.hpp"

class transactions_node_struct{
public:
  transactions_node_struct *next;//pointer to the next node
  transaction_struct *trans;//pointer to transaction information

  transactions_node_struct();
  ~transactions_node_struct();//deletes next
};

#endif
