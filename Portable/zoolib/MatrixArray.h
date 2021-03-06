/* -------------------------------------------------------------------------------------------------
Copyright (c) 2012 Andrew Green
http://www.zoolib.org

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------------------------- */

#ifndef __ZooLib_MatrixArray_h__
#define __ZooLib_MatrixArray_h__ 1
#include "zconfig.h"

#include "zoolib/Matrix.h"

namespace ZooLib {

// =================================================================================================
#pragma mark -

template <class E, size_t C, size_t R, size_t Length_p>
class MatrixArray
	{
public:
	MatrixArray() {}
	MatrixArray(const null_t&) {}

	const Matrix<E,C,R>& operator[](size_t i) const
		{ return fArray[i]; }

	Matrix<E,C,R>& operator[](size_t i)
		{ return fArray[i]; }

	Matrix<E,C,R> fArray[Length_p];
	};

// =================================================================================================
#pragma mark - operator== and operator!=

template <class E, size_t C, size_t R, size_t Length_p>
bool operator==(const MatrixArray<E,C,R,Length_p>& iL, const MatrixArray<E,C,R,Length_p>& iR)
	{
	for (size_t ii = 0; ii < Length_p; ++ii)
		{
		if (iL.fArray[ii] != iR.fArray[ii])
			return false;
		}
	return true;
	}

template <class E, size_t C, size_t R, size_t Length_p>
bool operator!=(const MatrixArray<E,C,R,Length_p>& iL, const MatrixArray<E,C,R,Length_p>& iR)
	{ return not (iL == iR); }

// =================================================================================================
#pragma mark -

template <class E, size_t C, size_t R, size_t Length_p, class P>
MatrixArray<E,C,R,Length_p> operator*(const MatrixArray<E,C,R,Length_p>& iLeft, const P& iRight)
	{
	MatrixArray<E,C,R,Length_p> result(null);
	for (size_t ii = 0; ii < Length_p; ++ii)
		result[ii] = iLeft[ii] * iRight;
	return result;
	}

template <class E, size_t C, size_t R, size_t Length_p, class P>
MatrixArray<E,C,R,Length_p>& operator*=(MatrixArray<E,C,R,Length_p>& ioLeft, const P& iRight)
	{
	for (size_t ii = 0; ii < Length_p; ++ii)
		ioLeft[ii] *= iRight;
	return ioLeft;
	}

// =================================================================================================
#pragma mark -

template <class E, size_t C, size_t R, size_t Length_p, class P>
MatrixArray<E,C,R,Length_p> operator/(const MatrixArray<E,C,R,Length_p>& iLeft, const P& iRight)
	{
	MatrixArray<E,C,R,Length_p> result(null);
	for (size_t ii = 0; ii < Length_p; ++ii)
		result[ii] = iLeft[ii] / iRight;
	return result;
	}

template <class E, size_t C, size_t R, size_t Length_p, class P>
MatrixArray<E,C,R,Length_p>& operator/=(MatrixArray<E,C,R,Length_p>& ioLeft, const P& iRight)
	{
	for (size_t ii = 0; ii < Length_p; ++ii)
		ioLeft[ii] /= iRight;
	return ioLeft;
	}

// =================================================================================================
#pragma mark -

template <class E, size_t C, size_t R, size_t Length_p, class P>
MatrixArray<E,C,R,Length_p> operator+(const MatrixArray<E,C,R,Length_p>& iLeft, const P& iRight)
	{
	MatrixArray<E,C,R,Length_p> result(null);
	for (size_t ii = 0; ii < Length_p; ++ii)
		result[ii] = iLeft[ii] + iRight;
	return result;
	}

template <class E, size_t C, size_t R, size_t Length_p, class P>
MatrixArray<E,C,R,Length_p>& operator+=(MatrixArray<E,C,R,Length_p>& ioLeft, const P& iRight)
	{
	for (size_t ii = 0; ii < Length_p; ++ii)
		ioLeft[ii] += iRight;
	return ioLeft;
	}

// =================================================================================================
#pragma mark -

template <class E, size_t C, size_t R, size_t Length_p, class P>
MatrixArray<E,C,R,Length_p> operator-(const MatrixArray<E,C,R,Length_p>& iLeft, const P& iRight)
	{
	MatrixArray<E,C,R,Length_p> result(null);
	for (size_t ii = 0; ii < Length_p; ++ii)
		result[ii] = iLeft[ii] - iRight;
	return result;
	}

template <class E, size_t C, size_t R, size_t Length_p, class P>
MatrixArray<E,C,R,Length_p>& operator-=(MatrixArray<E,C,R,Length_p>& ioLeft, const P& iRight)
	{
	for (size_t ii = 0; ii < Length_p; ++ii)
		ioLeft[ii] -= iRight;
	return ioLeft;
	}

} // namespace ZooLib

#endif // __ZooLib_MatrixArray_h__
