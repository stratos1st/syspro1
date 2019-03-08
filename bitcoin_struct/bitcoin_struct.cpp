#include "bitcoin_struct.hpp"

bitcoin_struct::bitcoin_struct(char* id, unsigned int val):value(val){
  strcpy(coin_id, id);
}

bitcoin_struct::~bitcoin_struct(){

}

char* bitcoin_struct::get_id(){
  return coin_id;
}

void bitcoin_struct::print_debug(){
  std::cout << "bitcoin_struct coin_id= "<<coin_id<<" value= "<<value
  <<" succ_trans= "<<succ_trans << '\n';
}
