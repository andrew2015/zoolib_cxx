/* -------------------------------------------------------------------------------------------------
Copyright (c) 2010 Andrew Green
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

#ifndef __ZCallable_ObjC_h__
#define __ZCallable_ObjC_h__ 1
#include "zconfig.h"

#include "zoolib/ZCallable.h"

#ifdef __OBJC__

#include "zoolib/ZCompat_NSObject.h"

#include <objc/message.h>

#if defined(__arm__)
	#include <tr1/type_traits>
#endif

namespace ZooLib {
namespace ZCallable_ObjC {

#if defined(__arm__)
	using namespace std::tr1;
#endif

// =================================================================================================
// MARK: - Helpers

enum
	{
	eUse_normal,
	eUse_stret
	#if defined(__i386__) || defined(__x86_64__)
		,eUse_fpret
	#endif
	};

// -----------------

template <bool> struct IfSmall { enum { value = eUse_normal }; };
template <> struct IfSmall<false> { enum { value = eUse_stret }; };

#if defined(__arm__)

	// On Arm void, integer, float, and enum (natural types) use objc_msgSend.
	// By inspection, objc_msgSend is also used for structures <= 4 bytes in size.
	template <class T,
		bool = is_void<T>::value || is_integral<T>::value
		|| is_floating_point<T>::value || is_enum<T>::value>
	struct Selector { enum { value = eUse_normal }; };

	template <class T> struct Selector<T, false>
		{ enum { value = IfSmall<(sizeof(T)<=4)>::value }; };

#else

	// On PPC and x86 all types <= 8 bytes use objc_msgSend, others use objc_msgSend_stret.
	template <class T> struct Selector { enum { value = IfSmall<(sizeof(T) <= 8)>::value }; };

	// On 32 bit x86 floats and doubles use objc_msgSend_fpret.
	#if defined(__i386__)
		template <> struct Selector<float> { enum { value = eUse_fpret }; };
		template <> struct Selector<double> { enum { value = eUse_fpret }; };
	#endif

	// On 32 and 64 bit x86, long doubles also use objc_msgSend_fpret.
	#if defined(__i386__) || defined(__x86_64__)
		template <> struct Selector<long double> { enum { value = eUse_fpret }; };
	#endif

#endif

// Explicit specialization for void, so we don't end up taking sizeof(void).
template <> struct Selector<void> { enum { value = eUse_normal }; };

// -----------------

template <class R, class FunctionPtr_t, int selector = Selector<R>::value>
struct MsgSend;

template <class R, class FunctionPtr_t>
struct MsgSend<R, FunctionPtr_t, eUse_normal>
	{ static FunctionPtr_t sMsgSend() { return (FunctionPtr_t)objc_msgSend; } };

template <class R, class FunctionPtr_t>
struct MsgSend<R, FunctionPtr_t, eUse_stret>
	{ static FunctionPtr_t sMsgSend() { return (FunctionPtr_t)objc_msgSend_stret; } };

#if defined(__i386__) || defined(__x86_64__)
template <class R, class FunctionPtr_t>
struct MsgSend<R, FunctionPtr_t, eUse_fpret>
	{ static FunctionPtr_t sMsgSend() { return (FunctionPtr_t)objc_msgSend_fpret; } };
#endif // defined(__i386__) || defined(__x86_64__)

// =================================================================================================
// MARK: - Base

// =================================================================================================
// MARK: - Callable

template <class Obj, class Signature> class Callable;

// =================================================================================================
// MARK: - Callable (specialization for 0 params)

template <class Obj,
	class R>
class Callable<Obj,R(void)>
:	public ZCallable<R(void)>
	{
public:
	typedef R (*FunctionPtr_t)(id,SEL);

	Callable(Obj iObj, SEL iSEL) : fObj(iObj), fSEL(iSEL) {}

// From ZCallable
	virtual ZQ<R> QCall()
		{ return MsgSend<R, FunctionPtr_t>::sMsgSend()(fObj, fSEL); }

private:
	Obj fObj;
	SEL fSEL;
	};

// =================================================================================================
// MARK: - Callable (specialization for 0 params, void return)

template <class Obj>
class Callable<Obj,void(void)>
:	public ZCallable<void(void)>
	{
public:
	typedef void (*FunctionPtr_t)(id,SEL);

	Callable(Obj iObj, SEL iSEL) : fObj(iObj), fSEL(iSEL) {}

// From ZCallable
	virtual ZQ<void> QCall()
		{
		MsgSend<void, FunctionPtr_t>::sMsgSend()(fObj, fSEL);
		return notnull;
		}

private:
	Obj fObj;
	SEL fSEL;
	};

// =================================================================================================
// MARK: - Callable variants

#define ZMACRO_Callable_Callable(X) \
\
template <class Obj, class R, ZMACRO_Callable_Class_P##X> \
class Callable<Obj,R(ZMACRO_Callable_P##X)> \
:	public ZCallable<R(ZMACRO_Callable_P##X)> \
	{ \
public: \
	typedef R (*FunctionPtr_t)(id,SEL,ZMACRO_Callable_P##X); \
\
	Callable(Obj iObj, SEL iSEL) : fObj(iObj), fSEL(iSEL) {}\
\
	virtual ZQ<R> QCall(ZMACRO_Callable_Pi##X) \
		{ return MsgSend<R, FunctionPtr_t>::sMsgSend() (fObj, fSEL, ZMACRO_Callable_i##X); } \
\
private: \
	Obj fObj; \
	SEL fSEL; \
	}; \
\
template <class Obj, ZMACRO_Callable_Class_P##X> \
class Callable<Obj,void(ZMACRO_Callable_P##X)> \
:	public ZCallable<void(ZMACRO_Callable_P##X)> \
	{ \
public: \
	typedef void (*FunctionPtr_t)(id,SEL,ZMACRO_Callable_P##X); \
\
	Callable(Obj iObj, SEL iSEL) : fObj(iObj), fSEL(iSEL) {} \
\
	virtual ZQ<void> QCall(ZMACRO_Callable_Pi##X) \
		{ \
		MsgSend<void, FunctionPtr_t>::sMsgSend() (fObj, fSEL, ZMACRO_Callable_i##X); \
		return notnull; \
		} \
\
private: \
	Obj fObj; \
	SEL fSEL; \
	};

ZMACRO_Callable_Callable(0)
ZMACRO_Callable_Callable(1)
ZMACRO_Callable_Callable(2)
ZMACRO_Callable_Callable(3)
ZMACRO_Callable_Callable(4)
ZMACRO_Callable_Callable(5)
ZMACRO_Callable_Callable(6)
ZMACRO_Callable_Callable(7)
ZMACRO_Callable_Callable(8)
ZMACRO_Callable_Callable(9)
ZMACRO_Callable_Callable(A)
ZMACRO_Callable_Callable(B)
ZMACRO_Callable_Callable(C)
ZMACRO_Callable_Callable(D)
ZMACRO_Callable_Callable(E)
ZMACRO_Callable_Callable(F)

#undef ZMACRO_Callable_Callable

} // namespace ZCallable_ObjC

// =================================================================================================
// MARK: - sCallable

template <class Signature>
ZRef<ZCallable<Signature> >
sCallable(id iObj, SEL iSEL)
	{
	if (iObj && iSEL)
		return new ZCallable_ObjC::Callable<id,Signature>(iObj, iSEL);
	return null;
	}

template <class Signature,class T>
ZRef<ZCallable<Signature> >
sCallable(const ZRef<T>& iObj, SEL iSEL)
	{
	if (iObj && iSEL)
		return new ZCallable_ObjC::Callable<ZRef<NSObject>,Signature>(iObj, iSEL);
	return null;
	}

} // namespace ZooLib

#endif // __OBJC__
#endif // __ZCallable_ObjC_h__
