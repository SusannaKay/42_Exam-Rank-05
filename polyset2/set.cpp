#include "set.hpp"

    set::set(searchable_bag &other): _bag(other){

    }
    set::~set(){}

    void set::insert(int n){
        if(_bag.has(n))
            return;
        _bag.insert(n);
    }
    void set::insert(int *items, int size){
        for (int i= 0; i < size; i++)
        {
            insert(items[i]);
        }
    }
    void set::print() const{
        _bag.print();
    }
    void set::clear(){
        _bag.clear();
    }
    bool set::has(int n) const{
        return (_bag.has(n));
    }