#include "block.hpp"

template<class T>
block<T>::block(unsigned int byte_max_sz){
    curr_pos=0;
    next=nullptr;
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
user_block_item* block<user_block_item>::find_in_block(char* id){
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
  std::cout << "block currpos= "<<curr_pos<<" max_sz= "<<max_sz << '\n';
  for(unsigned int i=0;i<curr_pos;i++)
    table[i]->print_debug();
}

template class block<user_block_item>;
template class block<bitcoin_struct>;
template class block<transaction_struct>;
