LRUCache
========

This is my implementation of a LRU (Least Recently Used) Cache.
The cache will store values and keep track of what was accessed most recently.
This is great to help reduce the amount of repetitive malloc calls, for
example. 

This implementation differs from most in that:

1. It is associate
2. Get always returns a value

The big use for me was get() creating values. If a value is not found in the
cache, it creates the value T(Key) and returns that. This does not have to be
used, however, if you only use get() on values you are positive have been put()
into the cache.

To use this in a project, just copy lru.hpp into your own projects source tree.
The namespace it uses is clortox

Run tests
=========

You can run the test suite with the following:

```bash
make sanity

# Relies on gprof
make profile
```

Interface
=========

Constructors
------------

```
LRUCache()
LRUCache(size_t max_size)
```

Defaults size of cache to 50

Utility
-------

```
void clear()
```

Clears out the contents of the cache

```
bool empty() const
```

Returns true if the cache is empty

```
bool full() const
```

Returns true if the cache is full

```
size_t getMaxCacheSize() const
```

Returns the cache size, passed as `max_size` when constructing

Accessors
--------

```
T& get(const Key&)
T& operator[](const Key&)
T& operator[](Key&)
```

Returns T associated with Key. If none exists, creates T(key), inserts it into
the cache, and returns a reference to it

```
void put(const Key&, const T&)
```

Places (Key, T) into the cache as the most recent item

```
bool erase(const Key&)
```

Erases element associated with Key. Use this if you know that item is not
likely to accessed soon and would like to delete ahead of time

```
void pop_back()
```

Removes oldest element from the cache. Can be used, however is more often used
internally
