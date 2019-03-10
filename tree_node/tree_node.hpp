#ifndef TREE_NODE
#define TREE_NODE

#include "../transaction_struct/transaction_struct.hpp"
#include <iostream>

class transaction_struct;

class tree_node{
private:
  transaction_struct *transaction;
  unsigned int curr_money;
  tree_node *to, *from;
public:
  tree_node(unsigned int money);
  ~tree_node();

  void insert_tree_node(transaction_struct *trans);
  unsigned int get_curr_money();
  tree_node* get_to();
  tree_node* get_from();
  transaction_struct* get_transaction();

  void print_debug();
};

#endif
