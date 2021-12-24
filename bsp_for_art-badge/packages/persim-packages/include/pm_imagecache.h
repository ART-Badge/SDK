#pragma once

#include "pm_internal.h"
#include "pm_hashmap.h"
#include "pm_image.h"
#include "pm_lrucache.h"

PERSIMMON_NAMESPACE_BEGIN

class Mutex;
class IODevice;
class ImageNode;

class ImageTables
{
public:
    class Registry
    {
    public:
        Registry();
        ~Registry();
        Image::Data *lookup(const IODevice *io);
        void insert(Image::Data *data);
        void remove(Image::Data *data);

    private:
        class Node;
        struct Private;
        HashMap<Node> m_map;
    };

    class Cache
    {
    public:
        Cache();
        ~Cache();
        void setSizeLimit(size_t limit);

        size_t size() const { return lru.size(); }
        void insert(const SharedDataRef<Image::Data> &ref);
        void flush(const IODevice *dev);
        void flush(size_t size = 0);

    private:
        class Node;
        struct Private;
        LRUCache<Node> lru;
    };

    static ImageTables *instance();
    Registry &registry() { return _registry; }
    Cache &cache() { return _cache; }
    Mutex &mutex() { return *_mutex; }

protected:
    ImageTables();
    ~ImageTables();

private:
    static ImageTables *_instance;
    Mutex *_mutex; // must allocated in local RAM
    Registry _registry;
    Cache _cache;
};

PERSIMMON_NAMESPACE_END
