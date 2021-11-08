#pragma once

#ifdef __APPLE__
#include <TargetConditionals.h>
#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
#include <OpenGLES/ES1/gl.h>
#else
#include <OpenGL/gl.h>
#endif
#elif defined __ANDROID__ || defined ANGLE || defined ROKU || defined STEAMLINK
#include <GLES2/gl2.h>
#else
#include <GL/gl.h>
#endif

#include "CustomVertex.h"
#include "Macros.h"

class VertexBuffer
{
	public:
		PTR(VertexBuffer)

		// Destructor
		~VertexBuffer();

		// Create
		static Ptr create(int _iVertices, bool _bDynamic = false);

		// Initialize
		bool initialize(int _iVertices, bool _bDynamic);

		// Close
		void close();

		// Get buffer
		SHAREDTYPE(CustomVertex) getBuffer(int& _iVertices) const;

		// Release buffer
		void releaseBuffer();

		// Get vertex count
		int getVertexCount() const {return m_iVertices;}

	private:
		SHAREDPTR(CustomVertex, m_pVertexBuffer);	// Vertices

		int		m_iVertices;	// Number of vertices
		bool	m_bDynamic;		// Dynamic vertex buffer?

		// Constructor
		VertexBuffer();
};
