#pragma once

#include "Macros.h"
#include "SystemGL.h"

class IndexBuffer
{
	public:
		PTR(IndexBuffer)

		// Destructor
		~IndexBuffer();

		// Create
		static Ptr create(int _iIndices, bool _bDynamic = false);

		// Initialize
		bool initialize(int _iIndices, bool _bDynamic);

		// Close
		void close();

		// Get buffer
		SHAREDTYPE(GLushort) getBuffer(int& _iIndices) const;

		// Release buffer
		void releaseBuffer();

		// Get index count
		int getIndexCount() const {return m_iIndices;}

	private:
		SHAREDPTR(GLushort, m_pIndexBuffer);	// Index buffer

		int		m_iIndices;	// Number of indices
		bool	m_bDynamic;	// Dynamic

		// Constructor
		IndexBuffer();
};
