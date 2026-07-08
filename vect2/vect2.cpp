#include "vect2.hpp"

vect2::vect2()
{
	_v[0] = 0;
	_v[1] = 0;
}

vect2::vect2(int x, int y)
{
	_v[0] = x;
	_v[1] = y;
}

vect2::vect2(const vect2 &other)
{
	if (this != &other)
	{
		_v[0] = other._v[0];
		_v[1] = other._v[1];
	}
}

vect2 &vect2::operator=(const vect2 &other)
{
	if (this != &other)
	{
		_v[0] = other._v[0];
		_v[1] = other._v[1];
	}
	return (*this);
}

vect2::~vect2(){}

vect2 vect2::operator-(const vect2 &other){
	vect2 tmp(*this);
	tmp -= other;
	return tmp;
}

vect2 vect2::operator*(int n) const{
	vect2 tmp(*this);
	tmp *= n;
	return (tmp);
}
vect2 &vect2::operator+=(const vect2 &other){
	_v[0] = _v[0] + other._v[0];
	_v[1] = _v[1] + other._v[1];
	return (*this);
}

vect2 vect2::operator+(const vect2 &other) const{
	vect2 tmp(*this);
	tmp += other;
	return tmp;

}
vect2 &vect2::operator-=(const vect2 &other){
	_v[0] = _v[0] - other._v[0];
	_v[1] = _v[1] - other._v[1];
	return (*this);
}

vect2 &vect2::operator*=(int n){
	_v[0] = _v[0] * n;
	_v[1] = _v[1] * n;
	return (*this);
}
bool vect2::operator==(vect2 &other){
	return(_v[0]==other._v[0] && _v[1] == other._v[1]);
}
const bool vect2::operator==(const vect2 &other){
	return(_v[0]==other._v[0] && _v[1] == other._v[1]);
}
vect2 &vect2::operator++(){
	_v[0] += 1;
	_v[1] += 1;
	return *this;
}
vect2 vect2::operator++(int){
	vect2 tmp(*this);
	_v[0] += 1;
	_v[1] += 1;
	return(tmp);

}
vect2 &vect2::operator--(){
	_v[0] -= 1;
	_v[1] -= 1;
	return *this;
}
vect2 vect2::operator--(int){
	vect2 tmp(*this);
	_v[0] -= 1;
	_v[1] -= 1;
	return(tmp);
}
bool vect2::operator!=(vect2 &other){
	return(_v[0]!=other._v[0] && _v[1] != other._v[1]);
}
const bool vect2::operator!=(const vect2 &other){
	return(_v[0]!=other._v[0] && _v[1] != other._v[1]);
}

int &vect2::operator[](int n) {
	return (_v[n]);
}
const int &vect2::operator[](int n) const{
	return (_v[n]);
}
vect2 vect2::operator-() const{
	vect2 tmp(*this);
	tmp *= -1;
	return tmp;
}

std::ostream &operator<<(std::ostream &os,const vect2 &other){

	os << "{" << other._v[0] << ", " << other._v[1] << "}";
	return (os);
}
vect2 operator*(int n, const vect2 &other){

	vect2 tmp(other);
	tmp *= n;
	return tmp;
}


