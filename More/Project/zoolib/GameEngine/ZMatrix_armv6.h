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

#ifndef __ZMatrix_armv6_h__
#define __ZMatrix_armv6_h__ 1
#include "zconfig.h"
#include "zoolib/ZCONFIG_API.h"

#include "zoolib/ZCompat_arm.h"
#include "zoolib/ZMatrix.h"

#ifndef ZCONFIG_API_Desired__Matrix_armv6
	#define ZCONFIG_API_Desired__Matrix_armv6 1
#endif

#ifndef ZCONFIG_API_Avail__Matrix_armv6
	#if ZCONFIG_API_Desired__Matrix_armv6
		#if __arm__
			// clang 4 is having trouble with fldmias instructions, disable for now.
			#if defined(_ARM_ARCH_6) && (not defined(__clang_major__) || __clang_major__ < 4)
				#define ZCONFIG_API_Avail__Matrix_armv6 1
			#endif
		#endif
	#endif
#endif

#ifndef ZCONFIG_API_Avail__Matrix_armv6
	#define ZCONFIG_API_Avail__Matrix_armv6 0
#endif

#if ZCONFIG_API_Enabled(Matrix_armv6)

namespace ZooLib {
namespace ZMatrix_armv6 {

// =================================================================================================
#pragma mark -
#pragma mark 

void Matrix4Mul(const float* src_mat_1, const float* src_mat_2, float* dst_mat);

void Matrix4Vector4Mul(const float* src_mat, const float* src_vec, float* dst_vec);

void Matrix4Vector3Mul(const float* src_mat, const float* src_vec, float* dst_vec);

void Matrix4Invert(const float* src_mat, float* dst_mat);

} // namespace ZMatrix_armv6
} // namespace ZooLib

#endif // ZCONFIG_API_Enabled(Matrix_armv6)

#endif // __ZMatrix_armv6_h__
