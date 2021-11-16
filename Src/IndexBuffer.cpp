// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "IndexBuffer.h"
#include "Log.h"
#include "SystemDisplay.h"

IndexBuffer::IndexBuffer()	:
	m_iIndices(0),
	m_bDynamic(false)
{
}

IndexBuffer::~IndexBuffer()
{
	close();
}

IndexBuffer::Ptr IndexBuffer::create(int _iIndices, bool _bDynamic)
{
	INSTANCE(pIndexBuffer, IndexBuffer())

	if (false == pIndexBuffer->initialize(_iIndices, _bDynamic))
	{
		pIndexBuffer.reset();
	}

	return	pIndexBuffer;
}

bool IndexBuffer::initialize(int _iIndices, bool _bDynamic)
{
	m_iIndices	= _iIndices;
	m_bDynamic	= _bDynamic;

	SHAREDBUFFER(m_pIndexBuffer, GLushort, m_iIndices);

	if (nullptr == m_pIndexBuffer)
	{
		Log::instance()->logError("Unable to create index buffer");

		return	false;
	}

	return	true;
}

void IndexBuffer::close()
{
	m_iIndices	= 0;
}

SHAREDTYPE(GLushort) IndexBuffer::getBuffer(int& _iIndices) const
{
	_iIndices	= m_iIndices;

	return	m_pIndexBuffer;
}

void IndexBuffer::releaseBuffer()
{
}
