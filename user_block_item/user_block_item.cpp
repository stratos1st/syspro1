#include "user_block_item.hpp"

user_block_item::user_block_item(){
  wallet=nullptr;
  start=nullptr;
}

user_block_item::~user_block_item(){
  //delete wallet;
  // transactions_node_struct* current = start;
  // transactions_node_struct* next;
  // while (current != NULL){
  //      next = current->next;
  //      delete current;
  //      current = next;
  //  }
  delete start;
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

void user_block_item::print_list(time_t time1, time_t time2){
  transactions_node_struct *tmp=start;
  while (tmp!=nullptr) {
    if((time1==0 && time2==0) || (difftime(tmp->trans->date,time1)>=0.0 && difftime(tmp->trans->date,time2)<=0.0))
      tmp->trans->print_transaction();
    tmp=tmp->next;
  }
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
