/**
    * Interface for all resource caches to derive from.
    *
    * Author: Skylar Payne
    * Date: 7/21/2013
    * File: IResourceCache.h
**/

#pragma once

#include "HashMap.h"

template<class T>
class IResourceCache
{
protected:
    hash_map<const char*, T*> _Resources;
public:
    /**
     * @brief IResourceCache initializes the resource hash table
     * @param size the size of the hash table
     */
    IResourceCache(unsigned int buckets) : _Resources(buckets) { }

    /**
     * @brief ~IResourceCache unloads all resources in the hash table
     */
    virtual ~IResourceCache() { Unload(); }

    /**
     * @brief Add adds a resource to the hash table, if not already there.
     * @param file the resource to add
     * @return true if successfully added or already existed in hash table, false otherwise
     */
    virtual bool Add(const char* file) = 0;

    /**
     * @brief Remove removes a resource from the hash table
     * @param file the resource to remove
     */
    virtual void Remove(const char* file) = 0;

    /**
     * @brief Get gets a resource
     * @param file the resource to get
     * @return the resource, if found, nullptr otherwise.
     */
    virtual T* Get(const char* file) = 0;

    /**
     * @brief Unload unloads all resources from the cache
     */
    void Unload()
    {
        _Resources.clear();
    }
};
