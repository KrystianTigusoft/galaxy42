#pragma once

#include <iostream>
#include <ostream>
#include <string>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_int/limits.hpp>

typedef boost::multiprecision::number<
	boost::multiprecision::cpp_int_backend<64, 64,
		boost::multiprecision::signed_magnitude, boost::multiprecision::checked, void> >
basic_xint;

typedef boost::multiprecision::number<
	boost::multiprecision::cpp_int_backend<64, 64,
		boost::multiprecision::unsigned_magnitude, boost::multiprecision::checked, void> >
basic_uxint;


typedef boost::multiprecision::number<
	boost::multiprecision::cpp_int_backend<1024, 1024,
		boost::multiprecision::signed_magnitude, boost::multiprecision::checked, void> >
xbigint;

typedef boost::multiprecision::number<
	boost::multiprecision::cpp_int_backend<1024, 1024,
		boost::multiprecision::unsigned_magnitude, boost::multiprecision::checked, void> >
uxbigint;



typedef boost::multiprecision::number<
	boost::multiprecision::cpp_int_backend<1024, 65536,
		boost::multiprecision::signed_magnitude, boost::multiprecision::checked, void> >
t_correct_int;


// TODO move to lib
// idea from http://stackoverflow.com/questions/16337610/how-to-know-if-a-type-is-a-specialization-of-stdvector
template <typename T, template<typename...> class R>
struct is_specialization : std::false_type { };
template <template<typename...> class R, typename... ARGS>
struct is_specialization<R<ARGS...>, R> : std::true_type { };

struct safer_int_base { };



template<typename T>
class safer_int : public safer_int_base {
	public: // <-- TODO private
		T xi; ///< the xint implementig my basic type
	public:
		safer_int()
			: xi( T() )
		{
		}

		template<typename U> safer_int(const safer_int<U> & obj)
			: xi(obj.xi)
		{
			// TODO numeric_cast
		}
		safer_int(int64_t obj) // TODO
			: xi(obj)
		{
			// TODO numeric_cast
		}
		template<typename U> safer_int<T> & operator=(const safer_int<U> & obj) {
			xi=obj.xi;
			// TODO numeric_cast
			return *this;
		}
		//template<typename U> safer_int<T> & operator=(U obj) {
		safer_int<T> & operator=(int64_t obj) {
			xi=obj;
			// TODO numeric_cast
			return *this;
		}

		template<typename U> operator safer_int<U> () const { safer_int<U> ret; ret.xi=xi; return ret; } // TODO check range
		// operator const char* () const { return xi; } // TODO wtf?
		template<typename U> operator U () const { U ret; ret=xi; return ret; } // TODO check range, numeric_cast?

		template<typename U> bool operator==(const safer_int<U> & obj) const { return xi==obj.xi; }
		template<typename U> bool operator!=(const safer_int<U> & obj) const { return xi!=obj.xi; }
		template<typename U> bool operator==(const U & obj) const { return xi==obj; } // TODO check!
		template<typename U> bool operator!=(const U & obj) const { return xi!=obj; } // TODO check!
		// TODO ^ enable if numeric

		template<typename U> bool operator>(safer_int<U> obj) { return xi>obj.xi; }
		template<typename U> bool operator>=(safer_int<U> obj) { return xi>=obj.xi; }
		template<typename U> bool operator<(safer_int<U> obj) { return xi<obj.xi; }
		template<typename U> bool operator<=(safer_int<U> obj) { return xi<=obj.xi; }
		template<typename U> bool operator>(U obj) { return xi>obj; }
		template<typename U> bool operator>=(U obj) { return xi>=obj; }
		template<typename U> bool operator<(U obj) { return xi<obj; }
		template<typename U> bool operator<=(U obj) { return xi<=obj; }

		safer_int<T> & operator++() { xi++; return *this; }
		safer_int<T> operator++(int) { auto tmp=*this; xi++; return tmp; }
		safer_int<T> & operator--() { xi--; return *this; }
		safer_int<T> operator--(int) { auto tmp=*this; xi--; return tmp; }

		safer_int<T> operator-() { auto tmp=*this; tmp.xi = -tmp.xi; return tmp; }

		template<typename U> safer_int<T> & operator+=(safer_int<U> obj) { xi+=obj.xi; return *this; }
		template<typename U> safer_int<T> & operator-=(safer_int<U> obj) { xi-=obj.xi; return *this; }
		template<typename U> safer_int<T> & operator/=(safer_int<U> obj) { xi/=obj.xi; return *this; }
		template<typename U> safer_int<T> & operator*=(safer_int<U> obj) { xi*=obj.xi; return *this; }
		template<typename U> safer_int<T> & operator+=(U obj) { xi+=obj; return *this; }
		template<typename U> safer_int<T> & operator-=(U obj) { xi-=obj; return *this; }
		template<typename U> safer_int<T> & operator/=(U obj) { xi/=obj; return *this; }
		template<typename U> safer_int<T> & operator*=(U obj) { xi*=obj; return *this; }

		template<typename U> safer_int<T> operator+(safer_int<U> obj) { auto tmp=*this; tmp.xi+=obj.xi; return tmp; }
		template<typename U> safer_int<T> operator-(safer_int<U> obj) { auto tmp=*this; tmp.xi-=obj.xi; return tmp; }
		template<typename U> safer_int<T> operator*(safer_int<U> obj) { auto tmp=*this; tmp.xi*=obj.xi; return tmp; }
		template<typename U> safer_int<T> operator/(safer_int<U> obj) { auto tmp=*this; tmp.xi/=obj.xi; return tmp; }
		template<typename U> safer_int<T> operator+(U obj) { auto tmp=*this; tmp.xi+=obj; return tmp; }
		template<typename U> safer_int<T> operator-(U obj) { auto tmp=*this; tmp.xi-=obj; return tmp; }
		template<typename U> safer_int<T> operator*(U obj) { auto tmp=*this; tmp.xi*=obj; return tmp; }
		template<typename U> safer_int<T> operator/(U obj) { auto tmp=*this; tmp.xi/=obj; return tmp; }
		// TODO check also for comparsion with buildint T, because it could be that build-in T is wider then some cpp_int

		void print(std::ostream& ostr) const { ostr<<xi; }
};

namespace std {

template <typename T>
class numeric_limits<safer_int<T>> {
	public:
		typedef T number_type;
		static number_type (min)() { return std::numeric_limits<number_type>::min(); };
		static number_type (max)() { return std::numeric_limits<number_type>::max(); };

		static number_type lowest() { return (min)(); }
		static number_type epsilon() { return 0; }
		static number_type round_error() { return 0; }
		static number_type infinity() { return 0; }
    static number_type quiet_NaN() { return 0; }
    static number_type signaling_NaN() { return 0; }
    static number_type denorm_min() { return 0; }
};

/*
template <typename T>
inline safer_int<T>
get_max(const boost::mpl::true_&, const boost::mpl::true_&) {
	typedef safer_int<T> result_type;
  static const result_type val = std::numeric_limits<decltype(T::xi)>::max();
  return val;
}
*/

} // std



// intended for comparsions like   int > xint:
// #define CONDITION typename std::enable_if<! std::is_base_of<safer_int_base, U>{}>::type* = nullptr >
#define CONDITION typename std::enable_if< std::is_integral<U>{}>::type* = nullptr
template<typename U, typename T, CONDITION >
bool operator>(U obj, safer_int<T> safeint) { return !( (safeint < obj) || (safeint==obj) ); }

template<typename U, typename T, CONDITION >
bool operator>=(U obj, safer_int<T> safeint) { return !( (safeint < obj) || (safeint==obj) ); }

template<typename U, typename T, CONDITION >
bool operator<(U obj, safer_int<T> safeint) { return !( (safeint < obj) || (safeint==obj) ); }

template<typename U, typename T, CONDITION >
bool operator<=(U obj, safer_int<T> safeint) { return !( (safeint < obj) || (safeint==obj) ); }
#undef CONDITION

// intended for comparsions like boost::number > xint
// #define CONDITION typename std::enable_if< is_specialization<U,boost::multiprecision::number>{}>::type* = nullptr
// template<typename U, typename T, CONDITION >

// boost::multiprecision::number<boost::multiprecision::backends::cpp_int_backend<1024, 65536, 1, 1, void>, 0> >
// boost::multiprecision::number<boost::multiprecision::backends::cpp_int_backend<U1, U2, U3, U4, U5>, U6> >
#define UTYPE boost::multiprecision::number<boost::multiprecision::backends::cpp_int_backend<U1, U2, U3, U4, U5>, U6> >
template<typename T,
	unsigned U1, unsigned U2,
	boost::multiprecision::cpp_integer_type U3, boost::multiprecision::cpp_int_check_type U4,
	typename U5,
	boost::multiprecision::expression_template_option U6>
bool operator>(boost::multiprecision::number<boost::multiprecision::backends::cpp_int_backend<U1, U2, U3, U4, U5>, U6> obj,
 safer_int<T> safeint) { return !( (safeint.xi < obj) || (safeint.xi==obj) ); }

/*
template<typename U, typename T>
bool operator>=(U obj, safer_int<T> safeint) { return !( (safeint < obj) || (safeint==obj) ); }

template<typename U, typename T>
bool operator<(U obj, safer_int<T> safeint) { return !( (safeint < obj) || (safeint==obj) ); }

template<typename U, typename T>
bool operator<=(U obj, safer_int<T> safeint) { return !( (safeint < obj) || (safeint==obj) ); }
*/
// #undef CONDITION
#undef UTYPE
// output / streams:

template <typename T>
std::ostream& operator<<(std::ostream& ostr, safer_int<T> obj) {
	obj.print(ostr);
	return ostr;
}

typedef safer_int<basic_xint> xint;
typedef basic_uxint uxint;

template<typename T>
uxint xsize(const T & obj) {
	return uxint( obj.size() );
}

/*void foo() {
	std::cerr << is_specialization< t_correct_int , boost::multiprecision::number >::value << std::endl;
}*/

template <typename T> bool overflow_impossible_in_assign(T, T) { return true; }

template <typename T, typename U>
bool overflow_impossible_in_assign(T target, U value) {

	//	if (aaa > bbb) return true;
	//	if (bbb > aaa) return false; // !

	const auto themax = std::numeric_limits<decltype(target)>::max();
//	std::cerr << "NOW: " << std::numeric_limits<decltype(target)>::max() << std::endl;
//	std::cerr << "value="<<value<<" vs themax="<<themax << std::endl;

		if (value < std::numeric_limits<decltype(target)>::min()) return false;
		if (value > themax) return false;

//	std::cerr<<"^--- ok"<<std::endl;
	return true; // no overflow
}



