#include "hash_table.hpp"

template <class T>
hast_table<T>::hast_table(unsigned int sz, unsigned int max_block_bytes):block_bytes(max_block_bytes){
  max_sz=sz;
  table=new block<T>*[max_sz];
  for(unsigned int i=0;i<max_sz;i++)
    table[i]=nullptr;
  last=nullptr;
}

template <class T>
hast_table<T>::~hast_table(){

}

template <class T>
T* hast_table<T>::find(char* id){
  unsigned int pos=hash_function(id);
  block<T> *i=table[pos];
  T* ans=nullptr;
  while(i!=nullptr){
    ans=find(id);
    if(ans!=nullptr)
      return ans;
    i=i->next;
  }
  return nullptr;
}

template <class T>
void hast_table<T>::insert(T* new_item){
  unsigned int pos=hash_function(new_item);
  if (table[pos]==nullptr) {
    table[pos]=new block<T>(block_bytes);
    last=table[pos];
    table[pos]->insert_last(new_item);
  }
  else{
    if(!last->is_full())
      last->insert_last(new_item);
    else{
      last->next=new block<T>(block_bytes);
      last=last->next;
      last->insert_last(new_item);
    }
  }

}

template <class T>
void hast_table<T>::print_debug(){
  std::cout << "hash_table block_bytes= "<<block_bytes<<
  " max_sz= "<<max_sz << '\n';
  for(unsigned int i=0;i<max_sz;i++)
    table[i]->print_debug();
}

template <>
unsigned int hast_table<user_block_item>::hash_function(user_block_item* item){
  unsigned int sum=0;
  char* str=item->get_id();
  for(unsigned int i=0;i<strlen(str);i++)
    sum+=str[i];
  return sum%max_sz;
}

template <>
unsigned int hast_table<bitcoin_struct>::hash_function(bitcoin_struct* item){
  unsigned int sum=0;
  char* str=item->coin_id;
  for(unsigned int i=0;i<strlen(str);i++)
    sum+=str[i];
  return sum%max_sz;
}

template <>
unsigned int hast_table<transaction_struct>::hash_function(transaction_struct* item){
  unsigned int sum=0;
  char* str=item->trans_id;
  for(unsigned int i=0;i<strlen(str);i++)
    sum+=str[i];
  return sum%max_sz;
}

template <class T>
unsigned int hast_table<T>::hash_function(char* str){
  unsigned int sum=0;
  for(unsigned int i=0;i<strlen(str);i++)
    sum+=str[i];
  return sum%max_sz;
}


template class hast_table<user_block_item>;
template class hast_table<bitcoin_struct>;
template class hast_table<transaction_struct>;
