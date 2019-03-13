#include "wallet_node_struct.hpp"

wallet_node_struct::wallet_node_struct(){
  next=nullptr;
  leaf_node=nullptr;
  bitcoin=nullptr;
}

wallet_node_struct::~wallet_node_struct(){
  if(next!=nullptr)
    delete next;
}
