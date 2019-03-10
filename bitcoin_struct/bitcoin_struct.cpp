#include "bitcoin_struct.hpp"

bitcoin_struct::bitcoin_struct(char* id, unsigned int val):value(val){
  strcpy(coin_id, id);
  root=new tree_node(val);
}

bitcoin_struct::~bitcoin_struct(){

}

char* bitcoin_struct::get_id(){
  return coin_id;
}

void bitcoin_struct::print_history(){
  if(root->is_leaf_node())
    std::cout<<"no transactions made with tis bitcoin\n";
  print_recursive(root);
}

void bitcoin_struct::print_recursive(tree_node *node){
  if (node->is_leaf_node())
      return;
  print_recursive(node->get_from());
  node->get_transaction()->print_transaction();
  print_recursive(node->get_to());
}

void bitcoin_struct::print_debug(){
  std::cout << "bitcoin_struct coin_id= "<<coin_id<<" value= "<<value
  <<" succ_trans= "<<succ_trans << '\n';
  root->print_debug();
}
