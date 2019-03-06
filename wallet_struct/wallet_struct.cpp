#include "wallet_struct.hpp"

wallet_struct::wallet_struct(char *usrid){
  send_money_total=0;
  recv_money_total=0;
  total_money=0;
  start=nullptr;
  strcpy(wallet_id,usrid);
}

wallet_struct::~wallet_struct(){

}

void wallet_struct:: add_initial_coin(bitcoin_struct *coin){
  wallet_node_struct *tmp=start;
  start=new wallet_node_struct;
  start->next=tmp;
  start->bitcoin=coin;
  start->leaf_node=coin->root;
}

wallet_node_struct* wallet_struct::send_money(transaction_struct *trans, bool increase_flag){
  start->leaf_node->insert_tree_node(trans);
  send_money_total+=trans->money;
  total_money-=trans->money;
  if(increase_flag)
    start->bitcoin->succ_trans++;

  tree_node *rcver_tree_node=start->leaf_node->get_to();
  bitcoin_struct *coin=start->bitcoin;
  start->leaf_node=start->leaf_node->get_from();
  if(start->leaf_node->get_curr_money()==0){//delete from start
    wallet_node_struct *tmp=start->next;
    delete start;
    start=tmp;
  }

  wallet_node_struct *ans=new wallet_node_struct;
  ans->leaf_node=rcver_tree_node;
  ans->bitcoin=coin;

  return ans;
}

void wallet_struct::recv_money(wallet_node_struct *new_wallet_list_node){
  recv_money_total+=new_wallet_list_node->leaf_node->get_curr_money();
  total_money+=new_wallet_list_node->leaf_node->get_curr_money();
  wallet_node_struct *tmp=start;
  start=new_wallet_list_node;
  new_wallet_list_node->next=tmp;
}

unsigned int wallet_struct::get_possible_transaction_money(){
  return start->leaf_node->get_curr_money();
}

bool wallet_struct::is_transaction_possible(transaction_struct *trans){
  return trans->money<=total_money;
}

void wallet_struct::print_debug(){
  std::cout << "wallet_struct send_money_total= "<<send_money_total<<
    " recv_money_total= "<<recv_money_total<<
    " total_money= "<<total_money << '\n';
  wallet_node_struct *tmp=start;
  while (tmp!=nullptr) {
    tmp->leaf_node->print_debug();
    std::cout << "bitcoin succ_trans= "<<tmp->bitcoin->succ_trans << '\n';
    tmp=tmp->next;
  }
}
