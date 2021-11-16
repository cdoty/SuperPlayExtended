#pragma once

#include "IndexBuffer.h"
#include "Rect.h"
#include "SystemGL.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VMath/vmath.h"

class DisplayBase
{
	public:
		// Destructor
		virtual ~DisplayBase();

		// Initialize
		bool initialize();

		// Close
		void close();

		// Clear
		void clear();

		// Present
		void present();

		// Setup device
		bool setupDevice();

		// Create render target
		bool createRenderTarget(int _iWidth, int _iHeight, GLint _iFormat = GL_RGBA);
		
		// Create vertex buffer
		int createVertexBuffer(int _iVertices, bool _bDynamic);

		// Get vertex buffer
		VertexBuffer::Ptr getVertexBuffer(int _iIndex) const;

		// Remove vertex buffer
		void removeVertexBuffer(int _iIndex);

		// Create index buffer
		int createIndexBuffer(int _iVertices, bool _bDynamic);

		// Get index buffer
		IndexBuffer::Ptr getIndexBuffer(int _iIndex) const;

		// Remove index buffer
		void removeIndexBuffer(int _iIndex);

		// Create texture
		int createTexture(const std::string& _strTextureName, int _iImageIndex, Texture::TextureFormat _eTextureFormat, bool _bDynamic = false);
		
		// Get texture
		Texture::Ptr getTexture(int _iHash);

		// Bind texture
		void bindTexture(int _iHash);

		// Remove texture
		void removeTexture(int _iHash);

		// Draw triangles
		bool drawTriangles(int _iVertexBufferIndex, int _iVertices = 0, int _iIndexBufferIndex = -1, int _iTriangles = 0);
		bool drawTriangles(VertexBuffer::Ptr _pVertexBuffer, int _iVertices = 0, IndexBuffer::Ptr _pIndexBuffer = nullptr, int _iTriangles = 0);
		
		// Set clip rect
		void setClipRect(const Rect& _rctClip);

		// Reset clip rect
		void resetClipRect();

		// Set border color
		void setBorderColor(float _fRed, float _fGreen, float _fBlue, float _fAlpha);
		void setBorderColor(uint8_t _red, uint8_t _green, uint8_t _blue, uint8_t _alpha);
		void setBorderColor(uint32_t _color);

		// Set clear color
		void setClearColor(float _fRed, float _fGreen, float _fBlue, float _fAlpha);
		void setClearColor(uint8_t _red, uint8_t _green, uint8_t _blue, uint8_t _alpha);
		void setClearColor(uint32_t _color);

	protected:
		struct TextureData
		{
			Texture::Ptr	pTexture;			// Texture
			int				iReferenceCount;	// Reference count
		
			TextureData() : iReferenceCount(0) {}
		};

		typedef	std::unordered_map<int, TextureData> TextureMap;

		std::vector<VertexBuffer::Ptr>	m_vecVertexBuffers;	// Vertex buffers
		std::vector<IndexBuffer::Ptr>	m_vecIndexBuffers;	// Index buffers

		EGLConfig	m_eglConfig;				// EGL config
		EGLContext	m_eglContext;				// EGL context
		EGLDisplay	m_eglDisplay;				// EGL display
		EGLSurface	m_eglSurface;				// EGL surface
		int			m_iWidth;					// Display width
		int			m_iHeight;					// Display height
		float		m_fRenderWidth;				// Render size
		float		m_fRenderHeight;
		float		m_fU2;						// Render texture UVs
		float		m_fV2;
		Matrix4f	m_matSimulatedTransform;	// Transform to 'simulated' resolution
		Matrix4f	m_matRenderTransform;		// Transform for render texture
		Point		m_ptOffset;					// Offset
		TextureMap	m_mapTextures;				// Textures
		GLint		m_iVertexArray;				// Shader variables
		GLint		m_iUVArray;
		GLint		m_iTexture;
		GLuint		m_uVertexShader;			// Shader
		GLuint		m_uPixelShader;
		GLuint		m_uShaderProgram;			// Shader program
		GLuint		m_uDefaultFrameBuffer;		// Default frame buffer
		GLuint		m_uFrameBuffer;				// Frame buffer
		GLuint		m_uRenderTexture;			// Render texture
		int			m_iVertexBuffer;			// Vertex buffer
		float		m_borderColor[4];			// Border color
		float		m_clearColor[4];			// Clear color
		
		// Constructor
		DisplayBase();

		// Initialize EGL
		virtual bool initializeEGL() = 0;

		// Close EGL
		void closeEGL();

		// Initialize GLES
		bool initializeGLES();

		// Close GLES
		void closeGLES();

		// Create vertex buffer
		bool createVertexBuffer();

		// Setup vertices
		bool setupVertices();

		// Setup transform
		void setupTransform();

		// Compile shaders
		bool compileShaders();

		// Load matrix
		void loadMatrix(const char* _szMatrixName, const Matrix4f& _matMatrix) const;

		// Log shader error
		void logShaderError(GLuint _uShaderID, const char* _szName) const;

		// Log program error
		void logProgramError(GLuint _uProgramID) const;
};
					 