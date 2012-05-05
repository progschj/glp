#ifndef VECTOR_TRAITS_H
#define VECTOR_TRAITS_H

template<class T>
struct vector_traits
{
	typedef T element_type;
	static const size_t dimension = 1;
};

template<class T, size_t D>
struct vector_traits<T[D]>
{
	typedef T element_type;
	static const size_t dimension = D;
};

#endif
