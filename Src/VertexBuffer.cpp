// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "CustomVertex.h"
#include "Display.h"
#include "Log.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()	:
	m_pVertexBuffer(NULL),
	m_iVertices(0),
	m_bDynamic(false)
{				   
}

VertexBuffer::~VertexBuffer()
{
	close();
}

VertexBuffer::Ptr VertexBuffer::create(int _iVertices, bool _bDynamic)
{
	INSTANCE(pVertexBuffer, VertexBuffer())

	if (false == pVertexBuffer->initialize(_iVertices, _bDynamic))
	{
		pVertexBuffer.reset();
	}

	return	pVertexBuffer;
}

bool VertexBuffer::initialize(int _iVertices, bool _bDynamic)
{
	m_iVertices	= _iVertices;
	m_bDynamic	= _bDynamic;

	SHAREDBUFFER(m_pVertexBuffer, CustomVertex, m_iVertices);
	
	if (nullptr == m_pVertexBuffer)
	{
		Log::instance()->logError("Unable to allocate memory for vertex buffer");

		return	false;
	}

	return	true;
}

void VertexBuffer::close()
{
	m_iVertices	= 0;
}

SHAREDTYPE(CustomVertex) VertexBuffer::getBuffer(int& _iVertices) const
{
	_iVertices	= m_iVertices;
		
	return	m_pVertexBuffer;
}

void VertexBuffer::releaseBuffer()
{
}
