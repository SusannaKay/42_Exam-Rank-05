#include "bigint.hpp"

bigint::bigint()
{
    _num = "0";
}
bigint::bigint(unsigned int n)
{
    std::stringstream ss;
    ss << n;
    _num = ss.str();
}
bigint::bigint(const bigint &other)
{
    if (this != &other)
    {
        _num = other._num;
    }
}
bigint &bigint::operator=(const bigint &other)
{
    if (this != &other)
    {
        _num = other._num;
    }
    return (*this);
}
bigint::~bigint() {}

bool bigint::operator>(const bigint &other) const
{
    if (_num.length() > other._num.length())
        return (true);
    else if (_num.length() < other._num.length())
        return (false);
    else
    {
        for (size_t i = 0; i < _num.length(); i++)
        {
            if (_num[i] > other._num[i])
                return (true);
            else if (_num[i] < other._num[i])
                return (false);
        }
        return (false);
    }
}
bool bigint::operator>=(const bigint &other) const
{
    if (*this > other)
        return (true);
    if (*this == other)
        return (true);
    return (false);
}

std::string bigint::getNum() const
{
    return (_num);
}
bool bigint::operator<(const bigint &other) const
{
    if (_num.length() < other._num.length())
        return (true);
    else if (_num.length() > other._num.length())
        return (false);
    else
    {
        for (size_t i = 0; i < _num.length(); i++)
        {
            if (_num[i] < other._num[i])
                return (true);
            else if (_num[i] > other._num[i])
                return (false);
        }
        return (false);
    }
}

bool bigint::operator<=(const bigint &other) const
{
    if (*this < other)
        return (true);
    if (*this == other)
        return (true);
    return (false);
}
bool bigint::operator==(const bigint &other) const
{
    if (_num.compare(other._num) == 0)
        return (true);
    return (false);
}
bool bigint::operator!=(const bigint &other) const
{
    if (_num.compare(other._num) != 0)
        return (true);
    return (false);
}

bigint bigint::operator+(const bigint &other) const
{
    bigint tmp(*this);
    tmp += other;
    return (tmp);
}
bigint &bigint::operator+=(const bigint &other)
{
    std::string res;
    int carry = 0;
    int i;
    int j;
    int sum;

    i = _num.length() - 1;
    j = (other.getNum()).length() - 1;

    while (i >= 0 || j >= 0 || carry)
    {
        sum = carry;
        if (i >= 0)
        {
            sum += _num[i] - '0';
            i--;
        }
        if (j >= 0)
        {
            sum += (other.getNum())[j] - '0';
            j--;
        }
        res.insert(res.begin(), (sum % 10) + '0');
        carry = sum / 10;
    }
    _num = res;
    return (*this);
}
bigint &bigint::operator++()
{

    *this += bigint(1);

    return (*this);
}
bigint bigint::operator++(int)
{

    bigint tmp(*this);
    *this += bigint(1);
    return (tmp);
}

bigint &bigint::operator<<=(unsigned int n)
{
    if (_num == "0")
        return (*this);
    _num.append(n, '0');
    return (*this);
}
bigint &bigint::operator>>=(unsigned int n)
{
    if (_num.length() <= n)
        _num = "0";
    else
        _num.resize(_num.length() - n);
    return (*this);
}

bigint &bigint::operator>>=(const bigint &n)
{
    unsigned int shift = 0;
    std::string s = n.getNum();
    for (size_t i = 0; i < s.length(); i++)
    {
        shift = shift * 10 + (s[i] - '0');
    }
    return (*this >>= shift);
}

bigint bigint::operator>>(unsigned int n) const
{
    bigint tmp(*this);
    tmp >>= n;
    return (tmp);
}

bigint bigint::operator<<(unsigned int n) const
{
    bigint tmp(*this);
    tmp <<= n;
    return (tmp);
}

std::ostream &operator<<(std::ostream &os, const bigint &a)
{

    os << a.getNum();
    return (os);
}