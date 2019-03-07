#include "block.hpp"

template<class T>
block<T>::block(unsigned int byte_max_sz){
    curr_pos=0;
    max_sz=byte_max_sz-sizeof(block);
    max_sz=max_sz/sizeof(T*);
    table=new T*[max_sz];
}

template<class T>
block<T>::~block(){

}

template<>
bitcoin_struct* block<bitcoin_struct>::find_in_block(char* id){
  for(unsigned int i=0;i<max_sz;i++)
    if(strcmp(table[i]->coin_id,id)==0)
      return table[i];
  return nullptr;
}

template<>
transaction_struct* block<transaction_struct>::find_in_block(char* id){
  for(unsigned int i=0;i<max_sz;i++)
    if(strcmp(table[i]->trans_id,id)==0)
      return table[i];
  return nullptr;
}

template<>
wallet_struct* block<wallet_struct>::find_in_block(char* id){
  for(unsigned int i=0;i<max_sz;i++)
    if(strcmp(table[i]->get_id(),id)==0)
      return table[i];
  return nullptr;
}

template<class T>
bool block<T>::insert_last(T* new_item){
  if(!is_full()){
    table[curr_pos]=new_item;
    curr_pos++;
    return true;
  }
  return false;
}

template<class T>
bool block<T>::is_full(){
  return curr_pos==max_sz;
}

template<class T>
void block<T>::print_debug(){
  std::cout << "block " << '\n';
  for(unsigned int i=0;i<max_sz;i++)
    table[i]->print_debug();
}

template<>
void block<transaction_struct>::print_debug(){
  std::cout << "block " << '\n';
  for(unsigned int i=0;i<max_sz;i++)
    std::cout << "transaction_struct trans_id= " <<table[i]->trans_id<<
    " money= "<<table[i]->money<< '\n';
}

template class block<wallet_struct>;
template class block<tree_node>;
template class block<transaction_struct>;
