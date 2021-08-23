///////////////////////////////////////////////////////////////////////////////
// Tyler Perkins
// 8-23-21
//

#include "lru/lru.hpp"
#include <iostream>
#include <cassert>
#include <stdlib.h>
#include <time.h>
#include <chrono>

using clortox::LRUCache;

class arr{
public:
    arr() { ptr = nullptr; };
    arr(int size){ 
        ptr = new int[size];
        for(int i = 0; i < size; ++i)
            ptr[i] = i;
    }
    ~arr(){ delete[] ptr; };
    int get(int index) const { return ptr[index]; };

private:
    int* ptr;
};

int main(int argc, char** argv){
    std::cout << "===== LRUCache Tests =====" << std::endl;

    std::cout << "Testing if cannonical..." << std::endl;
    {
        LRUCache<int, char> cache;
        assert(cache.getMaxCacheSize() == clortox::__default_max_cache_size);
        assert(cache.empty());
        
        LRUCache<int, char> ccache(25);
        assert(ccache.getMaxCacheSize() == 25);

        LRUCache<int, char> dcache(ccache);
        assert(dcache.getMaxCacheSize() == 25);

        ccache = cache;
        assert(ccache.getMaxCacheSize() == clortox::__default_max_cache_size);

        ccache.swap(dcache);
        assert(ccache.getMaxCacheSize() == 25);
        assert(dcache.getMaxCacheSize() == clortox::__default_max_cache_size);
    }
    std::cout << "Testing if cannonical passed!" << std::endl;

    std::cout << "Testing put functions..." << std::endl;
    {
        LRUCache<int, char> cache(5);
        cache.put(1, 'a');
        cache.put(26, 'z');
        cache.put(10, 'j');
        cache.put(3, 'b');

        assert(cache.get(26) == 'z');
        cache.put(10, 'j');
        assert(cache.get(10) == 'j');
        cache.put(2, 'b');
        assert(cache.get(2) == 'b');
        
        cache.put(1, 'a');
        assert(cache.get(1) == 'a');

    }
    std::cout << "Testing put functions passed!" << std::endl;

    std::cout << "Testing get functions with custom types..." << std::endl;
    {
        LRUCache<int, arr> cache(3);
        assert(cache.get(5).get(3) == 3);
        assert(cache.get(6).get(5) == 5);
        assert(cache.get(10).get(7) == 7);
        //cache hit
        assert(cache.get(6).get(5) == 5);

        //drop last element of cache
        assert(cache.get(2).get(1) == 1);
    }
    std::cout << "Testing get functions with custom types passed!" << std::endl;

    std::cout << "Testing operator[] with custom types..." << std::endl;
    {
        LRUCache<int, arr> cache(3);
        assert(cache[5].get(3) == 3);
        assert(cache[6].get(5) == 5);
        assert(cache[10].get(7) == 7);
        //cache hit
        assert(cache[6].get(5) == 5);

        //drop last element of cache
        assert(cache[2].get(1) == 1);
    }
    std::cout << "Testing operator[] with custom passed!" << std::endl;

    std::cout << "Testing erase function..." << std::endl;
    {
        LRUCache<int, char> cache(5);
        cache.put(1, 'a');
        cache.put(2, 'b');
        cache.put(3, 'c');
        cache.put(4, 'd');
        cache.put(5, 'e');

        assert(cache.full() == true);

        assert(cache.erase(26) == false);

        assert(cache.erase(1) == true);

        assert(cache.full() == false);
    }
    std::cout << "Testing erase function passed!" << std::endl;

    std::cout << "Testing pop_back..." << std::endl;
    {
        LRUCache<int, char> cache(5);
        cache.put(1, 'a');
        cache.put(2, 'b');
        cache.put(3, 'c');
        cache.put(4, 'd');
        cache.put(5, 'e');

        assert(cache.full() == true);

        cache.pop_back();
        
        assert(cache.full() == false);

    }
    std::cout << "Testing pop_back passed!" << std::endl;

    std::cout << "Performance testing..." << std::endl;
    {
        srand(time(NULL));
        size_t hits = 0;

        std::cout << "Cache size        : 1000\n";
        std::cout << "Range of elements : 10000\n";
        std::cout << "Iterations        : 1000000\n";

        LRUCache<int, arr> cache(1000);

        auto start = std::chrono::high_resolution_clock::now();

        for(size_t i = 0; i < 1000000; ++i){
            cache.get(rand() % 10000 + 1);
#ifdef _LRU_DEBUG_
            if(cache.wasHit)
                hits++;
#endif
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Running time: " << 
            std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()
            << " milliseconds" << std::endl;
#ifdef _LRU_DEBUG_
        std::cout << "Cache hits: " << hits << std::endl;
#endif

    }
    std::cout << "Performance testing finished!" << std::endl;

    std::cout << "===== LRUCache Tests =====" << std::endl;
}
