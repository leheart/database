/**
 * LRU implementation
 */
#include "buffer/lru_replacer.h"
#include "page/page.h"

namespace scudb {

template <typename T> LRUReplacer<T>::LRUReplacer() {}

template <typename T> LRUReplacer<T>::~LRUReplacer() {}

/*
 * Insert value into LRU
 */
template <typename T> void LRUReplacer<T>::Insert(const T &value) {
  for(int i = 0; i<(int)Value.size(); i++){
    if(Value[i] == value){
      Value.erase(Value.begin() + i);
    }
  }
  Value.insert(Value.begin(),value);
}

/* If LRU is non-empty, pop the head member from LRU to argument "value", and
 * return true. If LRU is empty, return false
 */
template <typename T> bool LRUReplacer<T>::Victim(T &value) {
  if(Value.size() == 0) return false;
  else{
    bool Success = false;
    for(int i = (int)Value.size() - 1; i>=0; i--){
      value = Value[i];
      Value.pop_back();
      Success = true;
      break;
    }
    return Success;
  }
}

/*
 * Remove value from LRU. If removal is successful, return true, otherwise
 * return false
 */
template <typename T> bool LRUReplacer<T>::Erase(const T &value) {
  bool Success = false;
  for(int i = (int)Value.size() - 1; i>=0; i--){
    if(Value[i]==value){
      Value.erase(Value.begin() + i);
      Success = true;
      break;
    }
  }
  return Success;
}

template <typename T> size_t LRUReplacer<T>::Size() { return (size_t)Value.size(); }


template class LRUReplacer<Page *>;
// test only
template class LRUReplacer<int>;

} // namespace scudb
