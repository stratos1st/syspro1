#include "transactions_node_struct.hpp"

transactions_node_struct::transactions_node_struct(){
  trans=nullptr;
  next=nullptr;
}

transactions_node_struct::~transactions_node_struct(){
  if(next!=nullptr)
    delete next;
}
