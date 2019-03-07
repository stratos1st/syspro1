#ifndef TRANSACTIONS_NODE_STRUCT
#define TRANSACTIONS_NODE_STRUCT

#include "../tree_node/tree_node.hpp"
#include "../bitcoin_struct/bitcoin_struct.hpp"

typedef struct transactions_node_struct_{
  struct transactions_node_struct_ *next;
  transaction_struct *trans;
}transactions_node_struct;

#endif
