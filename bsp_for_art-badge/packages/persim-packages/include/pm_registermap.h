#pragma once

#include "pm_vector.h"
#include <algorithm>

PERSIMMON_NAMESPACE_BEGIN

template<typename KT, typename VT> class RegisterMap
{
    struct Item
    {
        KT key;
        VT *value;
        bool operator==(KT k) const { return key == k; }
    };

    struct Less
    {
        bool operator()(const Item &lhs, KT rhs) const { return lhs.key < rhs; }
    };

public:
    RegisterMap() {}
    ~RegisterMap()
    {
        typename Vector<Item>::iterator it = m_t.begin();
        for (; it != m_t.end(); ++it) delete it->value;
    }
    VT *find(const KT &key) const
    {
        typename Vector<Item>::const_iterator it
            = std::lower_bound(m_t.begin(), m_t.end(), key, Less());
        return it != m_t.end() && *it == key ? it->value : nullptr;
    }

    void insert(KT key, VT *value)
    {
        typename Vector<Item>::iterator pos = std::lower_bound(m_t.begin(), m_t.end(), key, Less());
        if (pos != m_t.end() && *pos == key)
        {
            delete pos->value;
            pos->value = value;
        }
        else
        {
            Item item = {key, value};
            m_t.insert(pos, item);
        }
    }

private:
    Vector<Item> m_t;
};

PERSIMMON_NAMESPACE_END
