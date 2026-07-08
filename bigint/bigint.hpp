#ifndef BIGINT_HPP
# define BIGINT_HPP

#include <string>
#include <sstream>
#include <iostream>

class bigint {

    private: 
        std::string _num;
    public:
        bigint();
        bigint(unsigned int n);
        bigint(const bigint &other);
        bigint &operator=(const bigint &other);
        ~bigint();

        std::string getNum() const;

        bool operator>(const bigint &other) const;
        bool operator>=(const bigint &other) const;
        bool operator<(const bigint &other) const;
        bool operator<=(const bigint &other) const;
        bool operator==(const bigint &other) const;
        bool operator!=(const bigint &other) const;

        bigint operator+(const bigint &other) const;
        bigint &operator+=(const bigint &other);
        bigint &operator++(); //pre-inc
        bigint operator++(int); //post-inc

        bigint &operator<<=(unsigned int n);
        bigint &operator>>=(unsigned int n);
        bigint &operator>>=(const bigint &n);
        bigint operator>>(unsigned int n) const;
        bigint operator<<(unsigned int n) const;
};

std::ostream &operator<<(std::ostream &os, const bigint &a);

#endif