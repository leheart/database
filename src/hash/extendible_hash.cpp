#include <list>

#include "hash/extendible_hash.h"
#include "page/page.h"

namespace scudb {

/*
 * constructor
 * array_size: fixed array size for each bucket
 */
template <typename K, typename V>
ExtendibleHash<K, V>::ExtendibleHash(size_t size) {
  BucketSize = size;
  GlobalDepth = 0;
  NofBucket = 1;
  Record* records = new Record[size];
  Buckets.push_back(records);
}
/*
 * helper function to calculate the hashing address of input key
 */
template <typename K, typename V>
size_t ExtendibleHash<K, V>::HashKey(const K &key) {
  int hash = 0;
  if(GlobalDepth==0){return 0;}
  else{
    int d_num = 1;
    for(int x = 0;x<GlobalDepth;x++){
      d_num *= 2;
    }
    hash = key%(d_num);
    return (size_t)hash;
  }
}

/*
 * helper function to return global depth of hash table
 * NOTE: you must implement this function in order to pass test
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetGlobalDepth() const {
  return GlobalDepth;
}

/*
 * helper function to return local depth of one specific bucket
 * NOTE: you must implement this function in order to pass test
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetLocalDepth(int bucket_id) const {
  if(bucket_id >= NofBucket){
    std::cout<<"Doesn't exist bucket:"<< bucket_id << std::endl;
    return -1;
  }
  else {return BucketDepth[bucket_id];}
}

/*
 * helper function to return current number of bucket in hash table
 */
template <typename K, typename V>
int ExtendibleHash<K, V>::GetNumBuckets() const {
  return NofBucket;
}

/*
 * lookup function to find value associate with input key
 */
template <typename K, typename V>
bool ExtendibleHash<K, V>::Find(const K &key, V &value) {
  size_t hash = 0;
  size_t cal = 0;
  int desBucket = 0;
  bool Exist = false;
  
  hash = HashKey(key);
  for(int x = 0; x < NofBucket; x++){
    int d_num = 1;
    for(int y = 0; y < (GlobalDepth - BucketDepth[x]);y++){
      d_num *= 2;
    }
    cal += (size_t)(d_num);
    if(cal > hash){
      desBucket = x;
      break;
    }
  }
  
  for(int x = 0; x < BucketSize; x++){
    if (Buckets[desBucket][x].Key == key){
      value = Buckets[desBucket][x].Value;
      Exist = true;
    }
  }
  return Exist;
}

/*
 * delete <key,value> entry in hash table
 * Shrink & Combination is not required for this project
 */
template <typename K, typename V>
bool ExtendibleHash<K, V>::Remove(const K &key) {
  V value;
  if(Find(key,value) == false) {return false;}
  else{
    size_t hash = 0;
    hash = HashKey(key);
    size_t cal = 0;
    int desBucket = 0;
    for(int x = 0; x < NofBucket; x++){
      int d_num = 1;
      for(int y = 0; y < (GlobalDepth - BucketDepth[x]);y++){
        d_num *= 2;
      }
      cal += (size_t)(d_num);
      if(cal > hash){
        desBucket = x;
        break;
      }
    }
    
    
    for(int x = 0; x < BucketSize; x++){
      if (Buckets[desBucket][x].Key == key){
        Buckets[desBucket][x].Key = 0;
        break;
      }
    }
    return true;
  }
}

/*
 * insert <key,value> entry in hash table
 * Split & Redistribute bucket when there is overflow and if necessary increase
 * global depth
 */
template <typename K, typename V>
void ExtendibleHash<K, V>::Insert(const K &key, const V &value) {
  size_t hash = 0;
  hash = HashKey(key);
  
  int desBucket = 0;
  size_t cal = 0;
  for(int x = 0; x < NofBucket; x++){
    int d_num = 1;
    for(int y = 0; y < (GlobalDepth - BucketDepth[x]);y++){
      d_num *= 2;
    }
    cal += (size_t)(d_num);
    if(cal > hash){
      desBucket = x;
      break;
    }
  }
  
  bool Full = true;
  for(int x = 0; x < BucketSize; x++){
    if (Buckets[desBucket][x].Key == INVALID_PAGE_ID){
      Buckets[desBucket][x].Key = key;
      Buckets[desBucket][x].Value = value;
      Full = false;
      break;
    }
  }
  if(Full){
    Record* records = new Record[BucketSize];
    Buckets.insert(Buckets.begin() + desBucket + 1,records);
    BucketDepth[desBucket] += 1;
    BucketDepth.insert(BucketDepth.begin() + desBucket + 1 ,BucketDepth[desBucket]);
    
    NofBucket++;
    if(BucketDepth[desBucket] > GlobalDepth){GlobalDepth++;}
    
    for(int x = 0; x < BucketSize; x++){
      size_t x_hash = 0;
      size_t x_cal = 0;
      int x_desBucket = 0;
      x_hash = HashKey(Buckets[desBucket][x].Key);
      for(int y = 0; y < NofBucket; y++){
        int d_num = 1;
        for(int n = 0; n < (GlobalDepth - BucketDepth[y]); n++){
          d_num *= 2;
        }
        x_cal += (size_t)d_num;
        if(x_cal > x_hash){
          x_desBucket = y;
          break;
        }
      }
      if(x_desBucket!=desBucket){
        Insert(Buckets[desBucket][x].Key,Buckets[desBucket][x].Value);
        Buckets[desBucket][x].Key = 0;
      }
    }
    Insert(key,value);
  }
}

template class ExtendibleHash<page_id_t, Page *>;
//template class ExtendibleHash<Page *, std::list<Page *>::iterator>;
// test purpose
template class ExtendibleHash<int, std::string>;
template class ExtendibleHash<int, std::list<int>::iterator>;
template class ExtendibleHash<int, int>;
} // namespace scudb
