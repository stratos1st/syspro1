#include "tree_node.hpp"

tree_node::tree_node(unsigned int money){
  transaction=nullptr;
  curr_money=money;
  to=nullptr;
  from=nullptr;
}

tree_node::~tree_node(){

}

void tree_node::insert_tree_node(transaction_struct *trans){
  transaction=trans;
  to=new tree_node(trans->money);
  from=new tree_node(curr_money-trans->money);
}

unsigned int tree_node::get_curr_money(){
  return curr_money;
}

tree_node* tree_node::get_to(){
  return to;
}

tree_node* tree_node::get_from(){
  return from;
}

transaction_struct* tree_node::get_transaction(){
  return transaction;
}

void tree_node::print_debug(){
  std::cout << "tree_node curr_money= "<<curr_money << '\n';
}
