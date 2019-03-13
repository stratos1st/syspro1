#ifndef TREE_NODE
#define TREE_NODE

#include "../transaction_struct/transaction_struct.hpp"
#include <iostream>

class transaction_struct;

class tree_node{
private:
  transaction_struct *transaction;//pointer to transaction, null if leaf node
  unsigned int curr_money;//part of bitcoin money possesed before
  //transaction if transaction!=nullptr or current part of bitcoin money
  tree_node *to, *from;//pointer to children
public:
  tree_node(unsigned int money);
  ~tree_node();//deletes children

  void insert_tree_node(transaction_struct *trans);//breaks tree node into tow new nodes based on trenasaction
  unsigned int get_curr_money();//returns curr_money
  tree_node* get_to();//returns recver node
  tree_node* get_from();//returns sender node
  transaction_struct* get_transaction();//returns transaction
  bool is_leaf_node();//returns true if node is leaf

  void print_debug();
};

#endif
