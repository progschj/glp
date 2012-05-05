/*
 * MathMatrix.h - Copyright (c) 2011 Jakob Progsch
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
 * MathMatrix.h provides a simple static Matrix template class with
 * a basic expression template ansatz for Matrix operations.
 */

#ifndef Matrix_H
#define Matrix_H

#include "MathVector.h"

template<class T, unsigned D1, unsigned D2> class Matrix;

//base class for all expression templates
template<class T, unsigned D1, unsigned D2, class A>
class MatrixExpr {
public:
    inline operator const A&() const
    {
        return *static_cast<const A*>(this);
    }
};

//better use macros instead of copy pasting this stuff all over the place
#define MAKE_MAT_MAT_EXPRESSION(NAME, EXPR, FUNCTION)                   \
template<class T, unsigned D1, unsigned D2, class A, class B>           \
class NAME : public MatrixExpr<T, D1, D2, NAME<T, D1, D2, A, B> > {     \
public:                                                                 \
    NAME(const A& pa, const B& pb) : a(pa), b(pb)   { }                 \
    inline T operator()(unsigned i, unsigned j) const { return EXPR; }  \
private:                                                                \
    const A& a;                                                         \
    const B& b;                                                         \
};                                                                      \
                                                                        \
template<class T, unsigned D1, unsigned D2, class A, class B>           \
inline NAME<T,D1,D2,A,B>                                                \
FUNCTION(const MatrixExpr<T,D1,D2,A> &a, const MatrixExpr<T,D1,D2,B> &b)\
{                                                                       \
    return NAME<T,D1,D2,A,B>(a, b);                                     \
}

#define MAKE_MAT_SCAL_EXPRESSION(NAME, EXPR, FUNCTION)                  \
template<class T, unsigned D1, unsigned D2, class A>                    \
class NAME : public MatrixExpr<T, D1, D2, NAME<T, D1, D2, A> > {        \
public:                                                                 \
    NAME(const A& pa, const T& pb) : a(pa), b(pb)   { }                 \
    inline T operator()(unsigned i, unsigned j) const { return EXPR; }  \
private:                                                                \
    const A& a;                                                         \
    const T& b;                                                         \
};                                                                      \
                                                                        \
template<class T, unsigned D1, unsigned D2, class A>                    \
inline NAME<T,D1,D2,A>                                                  \
FUNCTION(const  MatrixExpr<T,D1,D2,A> &a, const T &b)                   \
{                                                                       \
    return NAME<T,D1,D2,A>(a, b);                                       \
}

#define MAKE_SCAL_MAT_EXPRESSION(NAME, EXPR, FUNCTION)                  \
template<class T, unsigned D1, unsigned D2, class A>                    \
class NAME : public MatrixExpr<T, D1, D2, NAME<T, D1, D2, A> > {        \
public:                                                                 \
    NAME(const T& pa, const A& pb) : a(pa), b(pb)   { }                 \
    inline T operator()(unsigned i, unsigned j) const { return EXPR; }  \
private:                                                                \
    const T& a;                                                         \
    const A& b;                                                         \
};                                                                      \
                                                                        \
template<class T, unsigned D1, unsigned D2, class A>                    \
inline NAME<T,D1,D2,A>                                                  \
FUNCTION(const T &a, const MatrixExpr<T,D1,D2,A> &b)                    \
{                                                                       \
    return NAME<T,D1,D2,A>(a, b);                                       \
}

#define MAKE_MAT_EXPRESSION(NAME, EXPR, FUNCTION)                       \
template<class T, unsigned D1, unsigned D2, class A>                    \
class NAME : public MatrixExpr<T, D1, D2, NAME<T, D1, D2, A> > {        \
public:                                                                 \
    NAME(const A& pa) : a(pa)   { }                                     \
    inline T operator()(unsigned i, unsigned j) const { return EXPR; }  \
private:                                                                \
    const A& a;                                                         \
};                                                                      \
                                                                        \
template<class T, unsigned D1, unsigned D2, class A>                    \
inline NAME<T,D1,D2,A>                                                  \
FUNCTION(const MatrixExpr<T,D1,D2,A> &a)                                \
{                                                                       \
    return NAME<T,D1,D2,A>(a);                                          \
}

//create actual functions and operators
MAKE_MAT_MAT_EXPRESSION (MATEMulExpr, a(i,j) * b(i,j), multiply_elements)
MAKE_MAT_MAT_EXPRESSION (MATEDivExpr, a(i,j) / b(i,j), divide_elements)
MAKE_MAT_MAT_EXPRESSION (MATAddExpr,  a(i,j) + b(i,j), operator+)
MAKE_MAT_MAT_EXPRESSION (MATSubExpr,  a(i,j) - b(i,j), operator-)
MAKE_MAT_SCAL_EXPRESSION(MATDivExpr,  a(i,j) / b, operator/)
MAKE_MAT_SCAL_EXPRESSION(MATMulExpr1, a(i,j) * b, operator*)
MAKE_SCAL_MAT_EXPRESSION(MATMulExpr2, a * b(i,j), operator*)
MAKE_MAT_EXPRESSION     (MATNegExpr, -a(i,j), operator-)

//transposition
template<class T, unsigned D1, unsigned D2, class A>
class TransExpr : public MatrixExpr<T, D1, D2, TransExpr<T, D1, D2, A> > {
public:
    TransExpr(const A& pa) : a(pa)  { }
    inline T operator()(unsigned i, unsigned j) const {
        return a(j,i);
    }
private:
    const A& a;
};

template<class T, unsigned D1, unsigned D2, class A>
inline TransExpr<T,D2,D1,A>
Transpose(const MatrixExpr<T,D1,D2,A> &a)
{
    return TransExpr<T,D2,D1,A>(a);
}

//Row, Column vector and submatrix proxies
template<class T, unsigned D1, unsigned D2, class A>
class RowVectorExpr : public VectorExpr<T, D2, RowVectorExpr<T, D1, D2, A> > {
public:
    RowVectorExpr(const A& pa, const unsigned& i) : a(pa), index(i) { }
    inline T operator[](unsigned i) const {
        return a(index, i);
    }
private:
    const A& a;
    const unsigned index;
};

template<class T, unsigned D1, unsigned D2, class A>
inline RowVectorExpr<T,D1,D2,A>
Row(const MatrixExpr<T,D1,D2,A> &a, const unsigned &index)
{
    return RowVectorExpr<T,D1,D2,A>(a, index);
}

template<class T, unsigned D1, unsigned D2, class A>
class ColumnVectorExpr : public VectorExpr<T, D1, ColumnVectorExpr<T, D1, D2, A> > {
public:
    ColumnVectorExpr(const A& pa, const unsigned& i) : a(pa), index(i)  { }
    inline T operator[](unsigned i) const {
        return a(i, index);
    }
private:
    const A& a;
    const unsigned index;
};

template<class T, unsigned D1, unsigned D2, class A>
inline ColumnVectorExpr<T,D1,D2,A>
Column(const MatrixExpr<T,D1,D2,A> &a, const unsigned &index)
{
    return ColumnVectorExpr<T,D1,D2,A>(a, index);
}

template<class T, unsigned D1, unsigned D2, class A>
class SubMatrixExpr : public MatrixExpr<T, D1, D2, SubMatrixExpr<T, D1, D2, A> > {
public:
    SubMatrixExpr(const A& pa, const unsigned& i, const unsigned& j)
        : a(pa), offseti(i), offsetj(j) { }
    inline T operator()(unsigned i, unsigned j) const
    {
        return a(i+offseti, j+offsetj);
    }
private:
    const A& a;
    const unsigned offseti, offsetj;
};

template<unsigned D3, unsigned D4, class T, unsigned D1, unsigned D2, class A>
inline SubMatrixExpr<T,D3,D4,A>
SubMatrix(const MatrixExpr<T,D1,D2,A> &a, const unsigned &i, const unsigned &j)
{
    return SubMatrixExpr<T,D3,D4,A>(a, i, j);
}

//matrix-vector and vector-matrix multiplications
template<class T, unsigned D1, unsigned D2, class A, class B>
class MatVecMulExpr : public VectorExpr<T, D1, MatVecMulExpr<T, D1, D2, A, B> > {
public:
    MatVecMulExpr(const A& pa, const B& pb) : a(pa), b(pb)  { }
    inline T operator[](unsigned i) const {
        return dot(Row(a, i), b);
    }
private:
    const A& a;
    const B& b;
};

template<class T, unsigned D1, unsigned D2, class A, class B>
inline MatVecMulExpr<T,D1,D2,A,B>
operator*(const MatrixExpr<T,D1,D2,A> &a, const VectorExpr<T,D2,B> &b)
{
    return MatVecMulExpr<T,D1,D2,A,B>(a, b);
}

template<class T, unsigned D1, unsigned D2, class A, class B>
class VecMatMulExpr : public VectorExpr<T, D2, VecMatMulExpr<T, D1, D2, A, B> > {
public:
    VecMatMulExpr(const A& pa, const B& pb) : a(pa), b(pb)  { }
    inline T operator[](unsigned i) const {
        return dot(Column(a, i), b);
    }
private:
    const A& a;
    const B& b;
};

template<class T, unsigned D1, unsigned D2, class A, class B>
inline VecMatMulExpr<T,D1,D2,A,B>
operator*(const VectorExpr<T,D1,B> &b, const MatrixExpr<T,D1,D2,A> &a)
{
    return VecMatMulExpr<T,D1,D2,A,B>(a, b);
}

template<class T, unsigned D1, unsigned D2, class A, class B>
class MatMatMulExpr : public MatrixExpr<T, D1, D2, MatMatMulExpr<T, D1, D2, A, B> > {
public:
    MatMatMulExpr(const A& pa, const B& pb) : a(pa), b(pb)  { }
    inline T operator()(unsigned i, unsigned j) const {
        return dot(Row(a,i),Column(b,j));
    }
private:
    const A& a;
    const B& b;
};

template<class T, unsigned D1, unsigned D2, unsigned D3, class A, class B>
MatMatMulExpr<T,D1,D3,A,B >
operator*(const MatrixExpr<T,D1,D2,A> &a, const MatrixExpr<T,D2,D3,B> &b)
{
    return MatMatMulExpr<T,D1,D3,A,B>(a, b);
}

template<class T, unsigned D1, unsigned D2>
class Identity : public MatrixExpr<T, D1, D2, Identity<T, D1, D2> > {
public:
    inline T operator()(unsigned i, unsigned j) const {
        return i==j?1:0;
    }
};

//actual Matrix class
template<class T, unsigned D1, unsigned D2>
class Matrix : public MatrixExpr<T, D1, D2, Matrix<T,D1,D2> > {
public:
    static const unsigned Dim1 = D1;
    static const unsigned Dim2 = D2;
    typedef T Type;

    Matrix()
    {
        std::fill(data, data+Dim1*Dim2, T());
    }

    template<class A>
    Matrix(const MatrixExpr<T, D1, D2, A>& a)
    {
        const A& ao ( a );
        for(unsigned j = 0; j<Dim2; ++j)
            for(unsigned i = 0; i<Dim1; ++i)
                operator()(i,j) = ao(i,j);
    }

    T* raw() {
        return data;
    }

    T& operator() (unsigned i, unsigned j) {
        return data[i+j*Dim1];
    }
    const T& operator() (unsigned i, unsigned j) const {
        return data[i+j*Dim1];
    }

    const Matrix& operator*=(const T &b)
    {
        for(unsigned i = 0; i<Dim1*Dim2; ++i)
            data[i] *= b;
        return *this;
    }

    const Matrix& operator/=(const T &b)
    {
        for(unsigned i = 0; i<Dim1*Dim2; ++i)
            data[i] /= b;
        return *this;
    }

    template<class A>
    const Matrix& operator+=(const MatrixExpr<T, D1, D2, A>& a)
    {
        const A& ao ( a );
        for(unsigned j = 0; j<Dim2; ++j)
            for(unsigned i = 0; i<Dim1; ++i)
                operator()(i,j) += ao(i,j);
        return *this;
    }

    template<class A>
    const Matrix& operator-=(const MatrixExpr<T, D1, D2, A>& a)
    {
        const A& ao ( a );
        for(unsigned j = 0; j<Dim2; ++j)
            for(unsigned i = 0; i<Dim1; ++i)
                operator()(i,j) -= ao(i,j);
        return *this;
    }

    template<class A>
    const Matrix& operator*=(const MatrixExpr<T, D1, D2, A>& a)
    {
        *this = eval(*this * a);
        return *this;
    }

    template<class A>
    Matrix& operator=(const MatrixExpr<T, D1, D2, A>& a)
    {
        const A& ao ( a );
        for(unsigned j = 0; j<Dim2; ++j)
            for(unsigned i = 0; i<Dim1; ++i)
                operator()(i,j) = ao(i,j);
        return *this;
    }

private:
    T data[Dim1*Dim2];
};

template<class T, unsigned D1, unsigned D2>
class InPlaceMatrix : public MatrixExpr<T, D1, D2, InPlaceMatrix<T,D1,D2> > {
public:
    static const unsigned Dim1 = D1;
    static const unsigned Dim2 = D2;
    typedef T Type;

    InPlaceMatrix(T *const d) : data(d)
    { }

    template<class A>
    InPlaceMatrix(const MatrixExpr<T, D1, D2, A>& a)
    {
        const A& ao ( a );
        for(unsigned j = 0; j<Dim2; ++j)
            for(unsigned i = 0; i<Dim1; ++i)
                operator()(i,j) = ao(i,j);
    }

    T* raw() {
        return data;
    }

    T& operator() (unsigned i, unsigned j) {
        return data[i+j*Dim1];
    }
    const T& operator() (unsigned i, unsigned j) const {
        return data[i+j*Dim1];
    }

    const InPlaceMatrix& operator*=(const T &b)
    {
        for(unsigned i = 0; i<Dim1*Dim2; ++i)
            data[i] *= b;
        return *this;
    }

    const InPlaceMatrix& operator/=(const T &b)
    {
        for(unsigned i = 0; i<Dim1*Dim2; ++i)
            data[i] /= b;
        return *this;
    }

    template<class A>
    const InPlaceMatrix& operator+=(const MatrixExpr<T, D1, D2, A>& a)
    {
        const A& ao ( a );
        for(unsigned j = 0; j<Dim2; ++j)
            for(unsigned i = 0; i<Dim1; ++i)
                operator()(i,j) += ao(i,j);
        return *this;
    }

    template<class A>
    const InPlaceMatrix& operator-=(const MatrixExpr<T, D1, D2, A>& a)
    {
        const A& ao ( a );
        for(unsigned j = 0; j<Dim2; ++j)
            for(unsigned i = 0; i<Dim1; ++i)
                operator()(i,j) -= ao(i,j);
        return *this;
    }

    template<class A>
    const InPlaceMatrix& operator*=(const MatrixExpr<T, D1, D2, A>& a)
    {
        const A& ao ( a );
        *this = eval(*this * a);
        return *this;
    }

    template<class A>
    InPlaceMatrix& operator=(const MatrixExpr<T, D1, D2, A>& a)
    {
        const A& ao ( a );
        for(unsigned j = 0; j<Dim2; ++j)
            for(unsigned i = 0; i<Dim1; ++i)
                operator()(i,j) = ao(i,j);
        return *this;
    }

private:
    T *const data;
};

template<unsigned D1, unsigned D2, class T>
InPlaceMatrix<T,D1,D2> MakeMatrix(T *const p)
{
    return InPlaceMatrix<T,D1,D2>(p);
}

template<class T, unsigned D1, unsigned D2, class A>
inline Matrix<T, D1, D2> eval(const MatrixExpr<T, D1, D2, A>& a)
{
    return Matrix<T, D1, D2>(a);
}

template<class T, unsigned D1, unsigned D2, class A>
std::ostream& operator<<(std::ostream& out, const MatrixExpr<T, D1, D2, A>& a)
{
    const A& ao ( a );
    for(unsigned i = 0; i<D1; ++i)
    {
        out << '|' << ao(i, 0);
        for(unsigned j = 1; j<D2; ++j)
            out << ", " << ao(i, j);
        out << "|\n";
    }

    return out;
}

template<class T, unsigned D1, unsigned D2>
struct vector_traits< Matrix<T,D1,D2> >
{
    typedef T element_type;
    static const size_t dimension = D1*D2;
};

#undef MAKE_MAT_MAT_EXPRESSION
#undef MAKE_MAT_SCAL_EXPRESSION
#undef MAKE_SCAL_MAT_EXPRESSION
#undef MAKE_MAT_EXPRESSION

#endif
