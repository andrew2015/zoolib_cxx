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

#ifndef __ZooLib_QueryEngine_Walker_Embed_h__
#define __ZooLib_QueryEngine_Walker_Embed_h__ 1
#include "zconfig.h"

#include "zoolib/QueryEngine/Walker.h"
#include "zoolib/RelationalAlgebra/RelHead.h"

namespace ZooLib {
namespace QueryEngine {

// =================================================================================================
#pragma mark - Walker_Embed

class Walker_Embed : public Walker
	{
public:
	Walker_Embed(const ZP<Walker>& iWalker_Parent, const RelationalAlgebra::RelHead& iBoundNames,
		const string8& iColName, const ZP<Walker>& iWalker_Embedee);

	virtual ~Walker_Embed();

// From QueryEngine::Walker
	virtual void Rewind();

	virtual ZP<Walker> Prime(
		const std::map<string8,size_t>& iOffsets,
		std::map<string8,size_t>& oOffsets,
		size_t& ioBaseOffset);

	virtual bool QReadInc(Val_DB* ioResults);

// Our protocol
	ZP<Walker> GetParent()
		{ return fWalker_Parent; }

	ZP<Walker> GetEmbedee()
		{ return fWalker_Embedee; }

private:
	ZP<Walker> fWalker_Parent;
	const RelationalAlgebra::RelHead fBoundNames;
	const string8 fColName;
	ZP<Walker> fWalker_Embedee;

	size_t fOutputOffset;
	RelationalAlgebra::RelHead fEmbedeeRelHead;
	std::vector<size_t> fEmbedeeOffsets;
	};

} // namespace QueryEngine
} // namespace ZooLib

#endif // __ZooLib_QueryEngine_Walker_Embed_h__
