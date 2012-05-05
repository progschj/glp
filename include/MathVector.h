/*
 * MathVector.h - Copyright (c) 2011 Jakob Progsch
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *    1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 *    2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 *    3. This notice may not be removed or altered from any source
 *    distribution.
 */

/*
 * MathVector.h provides a simple static vector template class with
 * a basic expression template ansatz for vector operations.
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <algorithm>
#include <ostream>

#include <boost/static_assert.hpp>
#include "vector_traits.h"

template<class T, unsigned D> class Vector;

//base class for all expression templates
template<class T, unsigned D, class A>
class VectorExpr {
public:
    inline operator const A&() const
    {
        return *static_cast<const A*>(this);
    }
};

//better use macros instead of copy pasting this stuff all over the place
#define MAKE_VEC_VEC_EXPRESSION(NAME, EXPR, FUNCTION)					\
template<class T, unsigned D, class A, class B>							\
class NAME : public VectorExpr<T, D, NAME<T, D, A, B> > {				\
public:																	\
	NAME(const A& pa, const B& pb) : a(pa), b(pb)	{ }					\
	inline T operator[](unsigned i) const { return EXPR; }				\
private:																\
	const A& a;															\
    const B& b;															\
};																		\
																		\
template<class T, unsigned D, class A, class B>							\
inline NAME<T,D,A,B>													\
FUNCTION(const  VectorExpr<T,D,A> &a, const VectorExpr<T,D,B> &b)		\
{																		\
	return NAME<T,D,A,B>(a, b);											\
}

#define MAKE_VEC_SCAL_EXPRESSION(NAME, EXPR, FUNCTION)					\
template<class T, unsigned D, class A>									\
class NAME : public VectorExpr<T, D, NAME<T, D, A> > {					\
public:																	\
	NAME(const A& pa, const T& pb) : a(pa), b(pb)	{ }					\
	inline T operator[](unsigned i) const { return EXPR; }				\
private:																\
	const A& a;															\
    const T& b;															\
};																		\
																		\
template<class T, unsigned D, class A>									\
inline NAME<T,D,A>														\
FUNCTION(const  VectorExpr<T,D,A> &a, const T &b)						\
{																		\
	return NAME<T,D,A>(a, b);											\
}

#define MAKE_SCAL_VEC_EXPRESSION(NAME, EXPR, FUNCTION)					\
template<class T, unsigned D, class A>									\
class NAME : public VectorExpr<T, D, NAME<T, D, A> > {					\
public:																	\
	NAME(const T& pa, const A& pb) : a(pa), b(pb)	{ }					\
	inline T operator[](unsigned i) const { return EXPR; }				\
private:																\
	const T& a;															\
    const A& b;															\
};																		\
																		\
template<class T, unsigned D, class A>									\
inline NAME<T,D,A>														\
FUNCTION(const T &a, const VectorExpr<T,D,A> &b)						\
{																		\
	return NAME<T,D,A>(a, b);											\
}

#define MAKE_VEC_EXPRESSION(NAME, EXPR, FUNCTION)						\
template<class T, unsigned D, class A>									\
class NAME : public VectorExpr<T, D, NAME<T, D, A> > {					\
public:																	\
	NAME(const A& pa) : a(pa)	{ }										\
	inline T operator[](unsigned i) const { return EXPR; }				\
private:																\
	const A& a;															\
};																		\
																		\
template<class T, unsigned D, class A>									\
inline NAME<T,D,A>														\
FUNCTION(const VectorExpr<T,D,A> &a)									\
{																		\
	return NAME<T,D,A>(a);												\
}

//create actual functions and operators
MAKE_VEC_VEC_EXPRESSION (EMulExpr,  a[i] * b[i],  multiply_elements)
MAKE_VEC_VEC_EXPRESSION (EDivExpr,  a[i] / b[i],  divide_elements)
MAKE_VEC_VEC_EXPRESSION (AddExpr,   a[i] + b[i],  operator+)
MAKE_VEC_VEC_EXPRESSION (SubExpr,   a[i] - b[i],  operator-)
MAKE_VEC_SCAL_EXPRESSION(DivExpr,   a[i] / b,     operator/)
MAKE_VEC_SCAL_EXPRESSION(MulExpr1,  a[i] * b,     operator*)
MAKE_SCAL_VEC_EXPRESSION(MulExpr2,  a * b[i],     operator*)
MAKE_VEC_EXPRESSION     (NegExpr,  -a[i],         operator-)

MAKE_VEC_VEC_EXPRESSION (EMaxExpr,  std::max(a[i],b[i]),  max)
MAKE_VEC_VEC_EXPRESSION (EMinExpr,  std::min(a[i],b[i]),  min)

//sub vector proxy
template<class T, unsigned D, class A>
class SubVectorExpr : public VectorExpr<T, D, SubVectorExpr<T, D, A> > {
public:
    SubVectorExpr(const A& pa, const unsigned& o)
        : a(pa), offset(o) { }
    inline T operator[](unsigned i) const
    {
        return a[i+offset];
    }
private:
    const A& a;
    const unsigned offset;
};

template<unsigned D1, class T, unsigned D2, class A>
inline SubVectorExpr<T,D1,A>
SubVector(const VectorExpr<T,D2,A> &a, const unsigned &o)
{
    return SubVectorExpr<T,D1,A>(a, o);
}

//actual vector class
template<class T, unsigned D>
class Vector : public VectorExpr<T, D, Vector<T,D> > {
public:
    static const unsigned Dim = D;
    typedef T Type;

    Vector()
    {
        std::fill(data, data+Dim, T());
    }

    Vector(const T &p1)
    {
        BOOST_STATIC_ASSERT(Dim==1);
        data[0] = p1;
    }

    Vector(const T &p1, const T &p2)
    {
        BOOST_STATIC_ASSERT(Dim==2);
        data[0] = p1;
        data[1] = p2;
    }

    Vector(const T &p1, const T &p2, const T &p3)
    {
        BOOST_STATIC_ASSERT(Dim==3);
        data[0] = p1;
        data[1] = p2;
        data[2] = p3;
    }

    Vector(const T &p1, const T &p2, const T &p3, const T &p4)
    {
        BOOST_STATIC_ASSERT(Dim==4);
        data[0] = p1;
        data[1] = p2;
        data[2] = p3;
        data[3] = p4;
    }

    T* raw() {
        return data;
    }

	
    template<class T2, class A>
    Vector(const VectorExpr<T2, D, A>& a)
    {
        const A& ao ( a );
        for(unsigned i = 0; i<Dim; ++i)
            data[i] = ao[i];
    }

    T& operator[] (unsigned i) {
        return data[i];
    }
    const T& operator[] (unsigned i) const {
        return data[i];
    }

    const Vector& operator*=(const T &b)
    {
        for(unsigned i = 0; i<Dim; ++i)
            data[i] *= b;
        return *this;
    }

    const Vector& operator/=(const T &b)
    {
        for(unsigned i = 0; i<Dim; ++i)
            data[i] /= b;
        return *this;
    }

    template<class A>
    const Vector& operator+=(const VectorExpr<T, D, A>& a)
    {
        const A& ao ( a );
        for(unsigned i = 0; i<Dim; ++i)
            data[i] += ao[i];
        return *this;
    }

    template<class A>
    const Vector& operator-=(const VectorExpr<T, D, A>& a)
    {
        const A& ao ( a );
        for(unsigned i = 0; i<Dim; ++i)
            data[i] -= ao[i];
        return *this;
    }

    template<class A>
    Vector& operator=(const VectorExpr<T, D, A>& a)
    {
        const A& ao ( a );
        for(unsigned i = 0; i<Dim; ++i)
            data[i] = ao[i];
        return *this;
    }

    Vector& normalize()
    {
        *this /= abs(*this);
        return *this;
    }
private:
    T data[Dim];
};

//actual InPlaceVector class
template<class T, unsigned D>
class InPlaceVector : public VectorExpr<T, D, InPlaceVector<T,D> > {
public:
    static const unsigned Dim = D;
    typedef T Type;

    InPlaceVector(T *const d) : data(d)
    { }

    T* raw() {
        return data;
    }

    T& operator[] (unsigned i) {
        return data[i];
    }
    const T& operator[] (unsigned i) const {
        return data[i];
    }

    const InPlaceVector& operator*=(const T &b)
    {
        for(unsigned i = 0; i<Dim; ++i)
            data[i] *= b;
        return *this;
    }

    const InPlaceVector& operator/=(const T &b)
    {
        for(unsigned i = 0; i<Dim; ++i)
            data[i] /= b;
        return *this;
    }

    template<class A>
    const InPlaceVector& operator+=(const VectorExpr<T, D, A>& a)
    {
        const A& ao ( a );
        for(unsigned i = 0; i<Dim; ++i)
            data[i] += ao[i];
        return *this;
    }

    template<class A>
    const InPlaceVector& operator-=(const VectorExpr<T, D, A>& a)
    {
        const A& ao ( a );
        for(unsigned i = 0; i<Dim; ++i)
            data[i] -= ao[i];
        return *this;
    }

    template<class A>
    InPlaceVector& operator=(const VectorExpr<T, D, A>& a)
    {
        const A& ao ( a );
        for(unsigned i = 0; i<Dim; ++i)
            data[i] = ao[i];
        return *this;
    }

    InPlaceVector& normalize()
    {
        *this /= abs(*this);
        return *this;
    }
private:
    T *const data;
};

template<unsigned D, class T>
InPlaceVector<T,D> MakeVector(T *const p)
{
    return InPlaceVector<T,D>(p);
}

template<class T, unsigned D, class A>
inline Vector<T, D> eval(const VectorExpr<T, D, A>& a)
{
    return Vector<T, D>(a);
}

//"reduction" functions that don't return expression templates
template<class T, unsigned D, class A>
inline T sum(const VectorExpr<T, D, A>& a)
{
    const A& ao ( a );
    T res = 0;
    for(unsigned i = 0; i<D; ++i)
        res += ao[i];
    return res;
}

template<class T, unsigned D, class A, class B>
inline T dot(const VectorExpr<T, D, A>& a, const VectorExpr<T, D, B>& b)
{
    return sum(multiply_elements(a, b));
}

template<class T, class A, class B>
inline Vector<T, 3> cross(const VectorExpr<T, 3, A>& a, const VectorExpr<T, 3, B>& b)
{
    const A& ao ( a );
	const B& bo ( b );
	return Vector<T, 3>(ao[1]*bo[2]-ao[2]*bo[1], ao[2]*bo[0]-ao[0]*bo[2], ao[0]*bo[1]-ao[1]*bo[0]);
}

template<class T, unsigned D, class A>
inline T squared_norm(const VectorExpr<T, D, A>& a)
{
    const A& ao ( a );
    T res = 0;
    for(unsigned i = 0; i<D; ++i)
    {
        T tmp = ao[i];
        res += tmp*tmp;
    }
    return res;
}

template<class T, unsigned D, class A>
inline T abs(const VectorExpr<T, D, A>& a)
{
    return std::sqrt(squared_norm(a));
}

template<class T, unsigned D, class A>
inline T norm(const VectorExpr<T, D, A>& a)
{
    return std::sqrt(squared_norm(a));
}

template<class T, unsigned D, class A>
inline Vector<T,D> normalize(const VectorExpr<T, D, A>& a)
{
    return eval(a/norm(a));
}

template<class T, unsigned D, class A>
std::ostream& operator<<(std::ostream& out, const VectorExpr<T, D, A>& a)
{
    const A& ao ( a );
    out << '(' << ao[0];
    for(unsigned i = 1; i<D; ++i)
    {
        out << ", " << ao[i];
    }
    out << ')';

    return out;
}

template<class T, unsigned D>
struct vector_traits< Vector<T,D> >
{
	typedef T element_type;
	static const size_t dimension = D;
};

#undef MAKE_VEC_VEC_EXPRESSION
#undef MAKE_VEC_SCAL_EXPRESSION
#undef MAKE_SCAL_VEC_EXPRESSION
#undef MAKE_VEC_EXPRESSION

#endif
