///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 8-23-21
// LRU cache implementation
//

#ifndef _LRUCACHE_HPP_
#define _LRUCACHE_HPP_

#include <unordered_map>
#include <list>
#include <cstddef>

#include <iostream>

namespace clortox {
    constexpr size_t __default_max_cache_size = 50;


    template<class Key, class T>
    struct value_iterator {
        typename std::list<Key>::iterator it;
        T value;
    };

    template<class Key, class T, class Hash = std::hash<Key>>
    class LRUCache{
    public:
        LRUCache();
        LRUCache(size_t);
        ~LRUCache();
        LRUCache(const LRUCache&);
        LRUCache& operator=(const LRUCache&);

        void swap(LRUCache&);

        //utility
        void          clear          ();
        inline bool   empty          () const;
        inline bool   full           () const;
        inline size_t getMaxCacheSize() const;

        //accessors functions
        T&            get       (const Key&);
        inline T&     operator[](const Key&);
        inline T&     operator[](Key&);
        void          put       (const Key&, const T&);
        bool          erase     (const Key&);
        void          pop_back  ();

#ifdef _LRU_DEBUG_
        bool          wasHit = false;
#endif

    private:
        //list
        std::list<Key> _list;
        //hashmap
        std::unordered_map<Key, value_iterator<Key, T>, Hash> _hash_map;

        size_t _max_cache_size;
    };
}

///////////////////////////////////////////////////////////////////////////////
// Implementation /////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////
// Default Ctor
template<class Key, class T, class Hash>
clortox::LRUCache<Key, T, Hash>::LRUCache() 
    : LRUCache(__default_max_cache_size) {};

template<class Key, class T, class Hash>
clortox::LRUCache<Key, T, Hash>::LRUCache(
        size_t max_size) : _max_cache_size(max_size){

    _hash_map.reserve(max_size);
}

///////////////////////////////////////
// Dector
template<class Key, class T, class Hash>
clortox::LRUCache<Key, T, Hash>::~LRUCache(){
    while(!empty()){
        pop_back();
    }
}

///////////////////////////////////////
// Copy ctor
template<class Key, class T, class Hash>
clortox::LRUCache<Key, T, Hash>::LRUCache(const LRUCache& rhs){
    _max_cache_size = rhs._max_cache_size;
    _hash_map       = rhs._hash_map;
    _list           = rhs._list;
}

///////////////////////////////////////
// Assignment operator
template<class Key, class T, class Hash>
clortox::LRUCache<Key, T, Hash>& clortox::LRUCache<
    Key, T, Hash>::operator=(const LRUCache& rhs){

    if(this != &rhs){
        _max_cache_size = rhs._max_cache_size;
        _hash_map       = rhs._hash_map;
        _list           = rhs._list;
    }
    return *this;
}

///////////////////////////////////////
// Swap function
template<class Key, class T, class Hash>
void clortox::LRUCache<Key, T, Hash>::swap(LRUCache& rhs){
    decltype(_hash_map) tmphashmap = rhs._hash_map;
    rhs._hash_map = _hash_map;
    _hash_map = tmphashmap;

    decltype(_list) tmplist = rhs._list;
    rhs._list = _list;
    _list = tmplist;

    decltype(_max_cache_size) tmpmax = rhs._max_cache_size;
    rhs._max_cache_size = _max_cache_size;
    _max_cache_size = tmpmax;
}

///////////////////////////////////////
// Clears out all elements from cache
// This should only be called if all 
// elements in the cache are not likely
// to be used again
template<class Key, class T, class Hash>
void clortox::LRUCache<Key, T, Hash>::clear(){
    for(auto it = _hash_map.begin(); it != _hash_map.end();){
        _list.erase(it->second.it);
        it = _hash_map.erase(it);
    }
}

///////////////////////////////////////
// Returns true if the cache is empty
template<class Key, class T, class Hash>
bool clortox::LRUCache<Key, T, Hash>::empty() const{
    return _list.empty();
}

///////////////////////////////////////
// Check if cache is full
template<class Key, class T, class Hash>
bool clortox::LRUCache<Key, T, Hash>::full() const{
    return _list.size() == _max_cache_size;
}


///////////////////////////////////////
// Returns the max cache size
template<class Key, class T, class Hash>
size_t clortox::LRUCache<Key, T, Hash>::getMaxCacheSize() const{
    return _max_cache_size;
}

///////////////////////////////////////////////////////////////////////////////
// Accessor functions /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////
// get element from cache. If it does
// not exist, create it
template<class Key, class T, class Hash>
T& clortox::LRUCache<Key, T, Hash>::get(const Key& key){
    if(_hash_map.find(key) != _hash_map.end()){ //cache hit
#ifdef _LRU_DEBUG_
        wasHit = true;
#endif
        //update list
        _list.erase(_hash_map[key].it);
        _list.push_front(key);
        _hash_map[key].it = _list.begin();

        return _hash_map[key].value;
    } else { //cache miss
#ifdef _LRU_DEBUG_
        wasHit = false;
#endif
        //if cache is full, remove oldest element
        if(_list.size() == _max_cache_size){
            _hash_map.erase(_list.back());
            _list.pop_back();
        }

        //add new element
        _list.push_front(key);

        _hash_map[key] = { .it = _list.begin(), .value = T(key) };
        return _hash_map[key].value;
    }
}

///////////////////////////////////////
// Index operator returns value with 
// assoc key. If it does not exist, we 
// create it
template<class Key, class T, class Hash>
T& clortox::LRUCache<Key, T, Hash>::operator[](const Key& key){
    return get(key);
}
template<class Key, class T, class Hash>
T& clortox::LRUCache<Key, T, Hash>::operator[](Key& key){
    return get(key);
}

///////////////////////////////////////
// Puts the key value pair into the 
// cache
template<class Key, class T, class Hash>
void clortox::LRUCache<Key, T, Hash>::put(const Key& key, const T& value){
    if(_hash_map.find(key) != _hash_map.end()){ //cache hit
#ifdef _LRU_DEBUG_
        wasHit = true;
#endif
        //update list
        _list.erase(_hash_map[key].it);
        _list.push_front(key);
        _hash_map[key].it = _list.begin();

        _hash_map[key].value = value;
    } else { //cache hit, insert new element
#ifdef _LRU_DEBUG_
        wasHit = false;
#endif
        //if cache is full, remove oldest element
        if(_list.size() == _max_cache_size){
            _hash_map.erase(_list.back());
            _list.pop_back();
        }

        //add new element
        _list.push_front(key);
        _hash_map[key] = { .it = _list.begin(), .value = value };
    }
}

///////////////////////////////////////
// Remove element from cache. Only do
// this if positive it will not be
// referenced again
template<class Key, class T, class Hash>
bool clortox::LRUCache<Key, T, Hash>::erase(const Key& key){
    if(_hash_map.find(key) != _hash_map.end()){ //cache hit
        //remove element
        _list.erase(_hash_map[key].it);
        _hash_map.erase(key);
        return true;
    } //not in cache, return false
    return false;
}

///////////////////////////////////////
// Remove oldest element
template<class Key, class T, class Hash>
void clortox::LRUCache<Key, T, Hash>::pop_back(){
    if(!_list.empty()){
        _hash_map.erase(_list.back());
        _list.pop_back();
    }
}

#endif //!_LRUCACHE_HPP_
