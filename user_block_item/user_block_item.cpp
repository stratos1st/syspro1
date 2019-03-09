#include "user_block_item.hpp"

user_block_item::user_block_item(){
  wallet=nullptr;
  start=nullptr;
}

user_block_item::~user_block_item(){

}

void user_block_item::insert_first(transaction_struct* new_transaction){
  transactions_node_struct *new_node=new transactions_node_struct;
  new_node->next=start;
  new_node->trans=new_transaction;
  start=new_node;
}

char* user_block_item::get_id(){
  return wallet->get_id();
}

void user_block_item::print_debug(){
  std::cout << "user_block_item " << '\n';
  wallet->print_debug();
  transactions_node_struct *tmp=start;
  while (tmp!=nullptr) {
    std::cout << "transactions_node_struct trans_id= "<<tmp->trans->trans_id<<
    " money= "<<tmp->trans->money<< '\n';
    tmp=tmp->next;
  }
}

bool user_block_item::is_empty(){
  return start==nullptr;
}
