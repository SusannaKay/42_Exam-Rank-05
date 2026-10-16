#ifndef VECT2_HPP
# define VECT2_HPP

# include <iostream>
# include <string>



class vect2 {

	public :
		int _v[2];

		vect2();
		vect2(int x, int y);
		vect2(const vect2 &other);
		vect2 &operator=(const vect2 &other);
		~vect2();

		vect2 operator+(const vect2 &other) const;
		vect2 operator-() const; // meno unario
		vect2 operator-(const vect2 &other) const; // meno binario
		vect2 operator*(int n) const;
		vect2 &operator+=(const vect2 &other);
		vect2 &operator-=(const vect2 &other);
		vect2 &operator*=(int n);

		bool operator==(const vect2 &other) const;
		vect2 &operator++(); // pre incr
		vect2 operator++(int); //post incr
		vect2 &operator--();
		vect2 operator--(int);
		bool operator!=(const vect2 &other) const;
		int &operator[](int n); // assegnazione valore
		const int &operator[](int n) const; // lettura valore
};
std::ostream &operator<<(std::ostream &os,const vect2 &other);
vect2 operator*(int n, const vect2 &other);


#endif
