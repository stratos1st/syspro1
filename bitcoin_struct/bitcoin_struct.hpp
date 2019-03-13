#ifndef BITCOIN_STRUCT
#define BITCOIN_STRUCT

#include "../tree_node/tree_node.hpp"
#include <string.h>

class tree_node;

class bitcoin_struct{
private:
  void print_recursive(tree_node *node);// used in print_history
  unsigned int unspent_recursive(tree_node *node);// used in unspent_money
public:
  char coin_id[51];
  unsigned int succ_trans;//number of successiful transactions
  tree_node *root;// pointer to tree root
  const unsigned int value;// starting bitcoin value

  bitcoin_struct(char* id, unsigned int val);//initializer
  ~bitcoin_struct();// deletes tree

  char* get_id();//returns coin_id
  void print_history();//prints coin history
  unsigned int unspent_money();// returns unspent money

  void print_debug();
};

#endif
