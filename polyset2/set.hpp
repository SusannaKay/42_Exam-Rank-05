#ifndef SET_HPP
#define SET_HPP

#include "searchable_bag.hpp"

class set : public searchable_bag
{

private:
    searchable_bag &_bag;

public:
    set(searchable_bag &other);
    ~set();

    void insert(int n);
    void insert(int *items, int size);
    void print() const;
    void clear();
    bool has(int n) const;
};

#endif