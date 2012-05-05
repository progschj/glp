/*
 * GraphicsMatrices.h - Copyright (c) 2012 Jakob Progsch
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

#ifndef GRAPHICS_MATRICES_H
#define GRAPHICS_MATRICES_H

#include "MathMatrix.h"
#include <cmath>

template<class T>
Matrix<T, 4, 4> FrustumMatrix(T left, T right, T bottom, T top, T near, T far)
{
    Matrix<T, 4, 4> res;    
    res(0,0) = 2*near/(right-left);
    res(0,2) = (right+left)/(right-left);
    res(1,1) = 2*near/(top-bottom);
    res(1,2) = (top+bottom)/(top-bottom);
    res(2,2) = -(far+near)/(far-near);
    res(2,3) = -2*far*near/(far-near);
    res(3,2) = -1;
    return res;
}

template<class T>
Matrix<T, 4, 4> InverseFrustumMatrix(T left, T right, T bottom, T top, T near, T far)
{
    Matrix<T, 4, 4> res;    
    res(0,0) = (right-left)/(2*near);
    res(0,3) = (right+left)/(2*near);
    res(1,1) = (top-bottom)/(2*near);
    res(1,3) = (top+bottom)/(2*near);
    res(2,3) = -1;
    res(3,2) = -(far-near)/(2*far*near);
    res(3,3) = (far+near)/(2*far*near);
    return res;
}

template<class T>
Matrix<T, 4, 4> InfiniteFrustumMatrix(T left, T right, T bottom, T top, T near)
{
    Matrix<T, 4, 4> res;    
    res(0,0) = 2*near/(right-left);
    res(0,2) = (right+left)/(right-left);
    res(1,1) = 2*near/(top-bottom);
    res(1,2) = (top+bottom)/(top-bottom);
    res(2,2) = -1;
    res(2,3) = -2*near;
    res(3,2) = -1;
    return res;
}

template<class T>
Matrix<T, 4, 4> OrthoMatrix(T left, T right, T bottom, T top, T near, T far)
{
    Matrix<T, 4, 4> res;    
    res(0,0) = 2/(right-left);
    res(1,1) = 2/(top-bottom);
    res(2,2) = -2/(far-near);
    res(0,3) = -(right+left)/(right-left);
    res(1,3) = -(top+bottom)/(top-bottom);
    res(2,3) = -(far+near)/(far-near);
    res(3,3) = 1;
    return res;
}

template<class T>
Matrix<T, 4, 4> ObliqueMatrix(T left, T right, T bottom, T top, T near, T far, Vector<T, 3> dir)
{
    dir.normalize();
    dir /= dir[1];
    Matrix<T, 4, 4> res;    
    res(0,0) = 2/(right-left);
    res(0,1) = -2*dir[0]/(right-left);
    res(1,1) = 2*dir[2]/(top-bottom);
    res(1,2) = -2/(top-bottom);
    res(2,0) = -2*dir[0]/(near-far);
    res(2,1) = -2*dir[1]/(near-far);
    res(2,2) = -2*dir[2]/(near-far);
    res(0,3) = -(right+left)/(right-left);
    res(1,3) = -(top+bottom)/(top-bottom);
    res(2,3) = (far+near)/(far-near);
    res(3,3) = 1;
    return res;
}

template<class T>
Matrix<T, 4, 4> RotationMatrix(T angle, Vector<T,3> dir)
{
    dir.normalize();
    
    T x = dir[0];
    T y = dir[1];
    T z = dir[2];
    T s = std::sin(angle);
    T c = std::cos(angle);
    Matrix<T, 4, 4> res;    
    res(0,0) = x*x*(1-c)+c;
    res(1,0) = y*x*(1-c)+z*s;
    res(2,0) = x*z*(1-c)-y*s;

    res(0,1) = x*y*(1-c)-z*s;
    res(1,1) = y*y*(1-c)+c;
    res(2,1) = y*z*(1-c)+x*s;

    res(0,2) = x*z*(1-c)+y*s;
    res(1,2) = y*z*(1-c)-x*s;
    res(2,2) = z*z*(1-c)+c;

    res(3,3) = 1;
    return res;
}

template<class T>
Matrix<T, 4, 4> RotationMatrix(T angle, T x, T y, T z)
{
    return RotationMatrix(angle, Vector<T, 3>(x,y,z));
}

template<class T>
Matrix<T, 4, 4> TranslationMatrix(Vector<T,3> v)
{
    Matrix<T, 4, 4> res;    
    res(0,0) = 1;
    res(1,1) = 1;
    res(2,2) = 1;
    res(3,3) = 1;
    res(0,3) = v[0];
    res(1,3) = v[1];
    res(2,3) = v[2];
    return res;
}

template<class T>
Matrix<T, 4, 4> TranslationMatrix(T x, T y, T z)
{
    return TranslationMatrix(Vector<T, 3>(x,y,z));
}

template<class T>
Matrix<T, 4, 4> ScaleMatrix(Vector<T,3> v)
{
    Matrix<T, 4, 4> res;    
    res(0,0) = v[0];
    res(1,1) = v[1];
    res(2,2) = v[2];
    res(3,3) = 1;
    return res;
}

template<class T>
Matrix<T, 4, 4> ScaleMatrix(T x, T y, T z)
{
    return ScaleMatrix(Vector<T, 3>(x,y,z));
}
#endif
