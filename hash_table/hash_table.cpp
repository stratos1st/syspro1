#include "hash_table.hpp"

template <class T>
hash_table<T>::hash_table(unsigned int sz, unsigned int max_block_bytes):block_bytes(max_block_bytes){
  max_sz=sz;
  table=new block<T>*[max_sz];
  last=new block<T>*[max_sz];
  for(unsigned int i=0;i<max_sz;i++){
    table[i]=new block<T>(block_bytes);
    last[i]=table[i];
  }
}

template <class T>
hash_table<T>::~hash_table(){

}

template <class T>
T* hash_table<T>::find(char* id){
  unsigned int pos=hash_function(id);
  block<T> *i=table[pos];
  T* ans=nullptr;
  while(i!=nullptr){
    ans=i->find_in_block(id);
    if(ans!=nullptr)
      return ans;
    i=i->next;
  }
  return nullptr;
}

template <class T>
void hash_table<T>::insert(T* new_item){
  unsigned int pos=hash_function(new_item);
  if(!last[pos]->is_full())
    last[pos]->insert_last(new_item);
  else{
    last[pos]->next=new block<T>(block_bytes);
    last[pos]=last[pos]->next;
    last[pos]->insert_last(new_item);
  }
  //std::cout<<"\n"<<pos<<"\n";
}

template <class T>
void hash_table<T>::print_debug(){
  std::cout << "hash_table block_bytes= "<<block_bytes<<
  " max_sz= "<<max_sz << '\n';
  block<T>* tmp;
  for(unsigned int i=0;i<max_sz;i++){
    tmp=table[i];
    while(tmp!=nullptr){
      tmp->print_debug();
      tmp=tmp->next;
    }
  }
}

template <class T>
unsigned int hash_table<T>::hash_function(T* item){
  unsigned int sum=0;
  char* str=item->get_id();
  for(unsigned int i=0;i<strlen(str);i++)
    sum+=str[i];
  return sum%max_sz;
}

template <class T>
unsigned int hash_table<T>::hash_function(char* str){
  unsigned int sum=0;
  for(unsigned int i=0;i<strlen(str);i++)
    sum+=str[i];
  return sum%max_sz;
}


template class hash_table<user_block_item>;
template class hash_table<bitcoin_struct>;
template class hash_table<transaction_struct>;
