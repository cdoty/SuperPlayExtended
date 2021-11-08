// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <GLES2/gl2.h>

#include "Defines.h"
#include "DisplayBase.h"
#include "Functions.h"
#include "Log.h"
#include "PixelShader.h"
#include "System.h"
#include "VertexShader.h"

static const bool	gsc_bDynamicTextures	= false;	// Do not use dynamic textures

DisplayBase::DisplayBase()	:
	m_eglConfig(0),
	m_eglContext(EGL_NO_CONTEXT),
	m_eglDisplay(EGL_NO_DISPLAY),
	m_eglSurface(EGL_NO_SURFACE),
	m_iWidth(0),
	m_iHeight(0),
	m_fRenderWidth(0.0f),
	m_fRenderHeight(0.0f),
	m_fU2(0.0f),
	m_fV2(0.0f),
	m_ptOffset({}),
	m_iVertexArray(-1),
	m_iUVArray(-1),
	m_iTexture(-1),
	m_uVertexShader(0),
	m_uPixelShader(0),
	m_uShaderProgram(0),
#ifdef __IOS__
	m_uDefaultFrameBuffer(0),
#endif
	m_uFrameBuffer(0),
	m_uRenderTexture(0),
	m_iVertexBuffer(-1)
{
	m_clearColor[0]	= 0.0f;
	m_clearColor[1]	= 0.0f;
	m_clearColor[2]	= 0.0f;
	m_clearColor[3]	= 1.0f;
}

DisplayBase::~DisplayBase()
{
	close();
}

bool DisplayBase::initialize()
{
	if (false == initializeEGL())
	{
		return	false;
	}

	if (false == compileShaders())
	{
		return	false;
	}

	if (false == initializeGLES())
	{
		return	false;
	}

	return	true;
}

void DisplayBase::close()
{
	closeGLES();
	closeEGL();
}

void DisplayBase::clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_uFrameBuffer);

	// Use shader program
	glUseProgram(m_uShaderProgram);

	glClearColor(1.0f, 0.0f , 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_iWidth, m_iHeight);

	loadMatrix("matMatrix", m_matSimulatedTransform);
}

void DisplayBase::present()
{
#if defined __IOS__
	glBindFramebuffer(GL_FRAMEBUFFER, m_uDefaultFrameBuffer);
#else
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, m_iWidth, m_iHeight);

	loadMatrix("matMatrix", m_matRenderTransform);
	
	glEnableVertexAttribArray(m_iVertexArray);

	glEnableVertexAttribArray(m_iUVArray);
   
	VertexBuffer::Ptr	pVertexBuffer	= m_vecVertexBuffers[m_iVertexBuffer];
	
	int	iVertices;

	CustomVertex*	pBuffer	= pVertexBuffer->getBuffer(iVertices).get();

	int	iStride	= sizeof(CustomVertex);

	glVertexAttribPointer(m_iVertexArray, 2, GL_FLOAT, GL_FALSE, iStride, &pBuffer[0].fX);

	glVertexAttribPointer(m_iUVArray, 2, GL_FLOAT, GL_FALSE, iStride, &pBuffer[0].fU);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uRenderTexture);

	glUniform1i(m_iTexture, 0);

	// Draw the render buffer
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	// Disable vertex and texture coordinate arrays
	glDisableVertexAttribArray(m_iVertexArray);	
	glDisableVertexAttribArray(m_iUVArray);	

	if (m_eglDisplay != EGL_NO_DISPLAY && m_eglSurface != EGL_NO_SURFACE)
	{
		eglSwapBuffers(m_eglDisplay, m_eglSurface);
	}
}

bool DisplayBase::setupDevice()
{
	// Set culling mode
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	// Disable depth buffer
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	// Disable scissor
	glDisable(GL_SCISSOR_TEST);

	// Enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	return	true;
}

bool DisplayBase::createRenderTarget(int _iWidth, int _iHeight, GLint _iFormat)
{
	glGenFramebuffers(1, &m_uFrameBuffer);

	GLenum	error	= glGetError();

	if (error != GL_NO_ERROR)
	{
		Log::instance()->logError("Unable to create frame buffer %d", error);

		return	false;
	}

	// Create render texture
	glGenTextures(1, &m_uRenderTexture);

	error	= glGetError();

	if (error != GL_NO_ERROR)
	{
		Log::instance()->logError("Unable to create render texture %d", error);

		return	false;
	}

	glBindTexture(GL_TEXTURE_2D, m_uRenderTexture);

	error	= glGetError();

	if (error != GL_NO_ERROR)
	{
		Log::instance()->logError("Unable to bind render texture %d", error);

		return	false;
	}

	int	iTextureWidth	= Functions::getNextPowerOfTwo(_iWidth);
	int	iTextureHeight	= Functions::getNextPowerOfTwo(_iHeight);
	
#if defined __IOS__
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iTextureWidth, iTextureHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, NULL);
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iTextureWidth, iTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
#endif

	error	= glGetError();

	if (error != GL_NO_ERROR)
	{
		Log::instance()->logError("Unable to create render texture %d", error);

		return	false;
	}

	m_fU2	= static_cast<float>(_iWidth) / static_cast<float>(iTextureWidth);
	m_fV2	= static_cast<float>(_iHeight) / static_cast<float>(iTextureHeight);

	// Set render texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, m_uFrameBuffer);

	error	= glGetError();

	if (error != GL_NO_ERROR)
	{
		Log::instance()->logError("Unable to bind frame buffer %d", error);

		return	false;
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uRenderTexture, 0);

	error	= glGetError();

	if (error != GL_NO_ERROR)
	{
		Log::instance()->logError("Unable to attach texture to framebuffer %d", error);

		return	false;
	}

	error	= glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (error != GL_FRAMEBUFFER_COMPLETE)
	{
		Log::instance()->logError("Unable to setup frame buffer %d", error);

		return	false;
	}

	if (false == createVertexBuffer())
	{
		Log::instance()->logError("Unable to create frame buffer vertex buffer");

		return	false;
	}

#if defined __IOS__
	glBindFramebuffer(GL_FRAMEBUFFER, m_uDefaultFrameBuffer);
#elif defined __ANDROID__ || defined ANGLE
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

	return	true;
}

int DisplayBase::createVertexBuffer(int _iVertices, bool _bDynamic)
{
	VertexBuffer::Ptr	pVertexBuffer	= VertexBuffer::create(_iVertices, _bDynamic);

	if (nullptr == pVertexBuffer)
	{
		return	-1;
	}

	int	iIndex	= (int)m_vecVertexBuffers.size();

	m_vecVertexBuffers.push_back(pVertexBuffer);

	return	iIndex;
}

VertexBuffer::Ptr DisplayBase::getVertexBuffer(int _iIndex) const
{
	if (_iIndex >= 0 && _iIndex < (int)m_vecVertexBuffers.size())
	{
		return	m_vecVertexBuffers[_iIndex];
	}

	return	nullptr;
}

void DisplayBase::removeVertexBuffer(int _iIndex)
{
	if (_iIndex >= 0 && _iIndex < (int)m_vecVertexBuffers.size())
	{
		m_vecVertexBuffers[_iIndex].reset();
	}
}

int DisplayBase::createIndexBuffer(int _iVertices, bool _bDynamic)
{
	IndexBuffer::Ptr	pIndexBuffer	= IndexBuffer::create(_iVertices, _bDynamic);

	if (nullptr == pIndexBuffer)
	{
		return	-1;
	}

	int	iIndex	= (int)m_vecIndexBuffers.size();

	m_vecIndexBuffers.push_back(pIndexBuffer);

	return	iIndex;
}

IndexBuffer::Ptr DisplayBase::getIndexBuffer(int _iIndex) const
{
	if (_iIndex >= 0 && _iIndex < (int)m_vecVertexBuffers.size())
	{
		return	m_vecIndexBuffers[_iIndex];
	}

	return	nullptr;
}

void DisplayBase::removeIndexBuffer(int _iIndex)
{
	if (_iIndex >= 0 && _iIndex < (int)m_vecVertexBuffers.size())
	{
		m_vecIndexBuffers[_iIndex].reset();
	}
}

int DisplayBase::createTexture(const char* _szTextureName, int _iImageIndex, Texture::TextureFormat _eTextureFormat, bool _bDynamic)
{
	int	iHash	= Functions::getHash(_szTextureName);

	if (m_mapTextures.find(iHash) == m_mapTextures.end())
	{
		Texture::Ptr	pTexture	= Texture::create(_iImageIndex, _eTextureFormat, false == gsc_bDynamicTextures ? false : _bDynamic);

		if (nullptr == pTexture)
		{
			return	-1;
		}

		TextureData	textureData;

		textureData.pTexture		= pTexture;
		textureData.iReferenceCount	= 1;

		m_mapTextures[iHash]		=  textureData;
	}
	
	else
	{
		m_mapTextures[iHash].iReferenceCount++;
	}

	return	iHash;
}

Texture::Ptr DisplayBase::getTexture(int _iHash)
{
	if (m_mapTextures.find(_iHash) == m_mapTextures.end())
	{
		return	nullptr;
	}
	
	return	m_mapTextures[_iHash].pTexture;
}

void DisplayBase::removeTexture(int _iHash)
{
	TextureMap::const_iterator	itFound	= m_mapTextures.find(_iHash);

	if (itFound != m_mapTextures.end())
	{
		m_mapTextures[_iHash].iReferenceCount--;
		
		if (m_mapTextures[_iHash].iReferenceCount <= 0)
		{
			m_mapTextures[_iHash].pTexture.reset();
			
			m_mapTextures.erase(itFound);
		}
	}
}

void DisplayBase::bindTexture(int _iHash)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mapTextures[_iHash].pTexture->getTexture());

	glUniform1i(m_iTexture, 0);
}

bool DisplayBase::drawTriangles(int _iVertexBufferIndex, int _iVertices, int _iIndexBufferIndex, int _iTriangles)
{
	if (_iVertexBufferIndex < 0 || _iVertexBufferIndex >= (int)m_vecVertexBuffers.size())
	{
		Log::instance()->logError("Vertex buffer index is invalid.Debug session ended");

		return	false;
	}
	
	if (_iIndexBufferIndex < 0 || _iIndexBufferIndex >= (int)m_vecIndexBuffers.size())
	{
		Log::instance()->logError("Index buffer index is invalid.");

		return	false;
	}

	VertexBuffer::Ptr	pVertexBuffer	= m_vecVertexBuffers[_iVertexBufferIndex];
	
	if (nullptr == pVertexBuffer)
	{
		Log::instance()->logError("Vertext buffer is invalid.");

		return	false;
	}

	int	iVertexCount;

	glEnableVertexAttribArray(m_iVertexArray);
	glEnableVertexAttribArray(m_iUVArray);
   
	CustomVertex*	pBuffer	= pVertexBuffer->getBuffer(iVertexCount).get();

	int	iStride	= sizeof(CustomVertex);

	glVertexAttribPointer(m_iVertexArray, 2, GL_FLOAT, false, iStride, &pBuffer[0].fX);
	glVertexAttribPointer(m_iUVArray, 2, GL_FLOAT, false, iStride, &pBuffer[0].fU);

	if (_iIndexBufferIndex != -1)
	{
		int	iIndexCount;

		GLushort*	pIndexBuffer	= m_vecIndexBuffers[_iIndexBufferIndex]->getBuffer(iIndexCount).get();

		glDrawElements(GL_TRIANGLES, _iTriangles * 3, GL_UNSIGNED_SHORT, pIndexBuffer);
	}
	
	else
	{
		glDrawArrays(GL_TRIANGLE_STRIP, 0, _iVertices);
	}
	
	// Disable vertex and texture coordinate arrays
	glDisableVertexAttribArray(m_iVertexArray);	
	glDisableVertexAttribArray(m_iUVArray);	

	return	true;
}

void DisplayBase::setClipRect(const Rect& _rctClip)
{
	glScissor(_rctClip.iLeft, m_iHeight - _rctClip.bottom(), _rctClip.iWidth, _rctClip.iHeight);

	glEnable(GL_SCISSOR_TEST);
}

void DisplayBase::resetClipRect()
{
	glDisable(GL_SCISSOR_TEST);
}

void DisplayBase::setClearColor(float _fRed, float _fGreen, float _fBlue, float _fAlpha)
{
	m_clearColor[0]	= _fRed;
	m_clearColor[1]	= _fGreen;
	m_clearColor[2]	= _fBlue;
	m_clearColor[3]	= _fAlpha;
}

void DisplayBase::setClearColor(uint8_t _red, uint8_t _green, uint8_t _blue, uint8_t _alpha)
{
	m_clearColor[0]	= (float)_red / 255.0f;
	m_clearColor[1]	= (float)_green / 255.0f;
	m_clearColor[2]	= (float)_blue / 255.0f;
	m_clearColor[3]	= (float)_alpha / 255.0f;
}

void DisplayBase::setClearColor(uint32_t _color)
{
	uint8_t	alpha	= (_color & 0xFF000000);
	uint8_t	red		= (_color & 0x00FF0000) >> 24;
	uint8_t	green	= (_color & 0x0000FF00) >> 16;
	uint8_t	blue	= (_color & 0x000000FF) >> 8;

	m_clearColor[0]	= (float)red / 255.0f;
	m_clearColor[1]	= (float)green / 255.0f;
	m_clearColor[2]	= (float)blue / 255.0f;
	m_clearColor[3]	= (float)alpha / 255.0f;
}

void DisplayBase::closeEGL()
{
	if (m_eglDisplay != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

		if (m_eglContext != EGL_NO_CONTEXT)
		{
			eglDestroyContext(m_eglDisplay, m_eglContext);
		}

		if (m_eglSurface != EGL_NO_SURFACE)
		{
			eglDestroySurface(m_eglDisplay, m_eglSurface);
		}

		eglTerminate(m_eglDisplay);
	}

	m_eglContext	= EGL_NO_CONTEXT;
	m_eglDisplay	= EGL_NO_DISPLAY;
	m_eglSurface	= EGL_NO_SURFACE;
}

bool DisplayBase::initializeGLES()
{
	// Setup device
	if (false == setupDevice())
	{
		return	false;
	}

	// Create render target
	if (false == createRenderTarget(m_iWidth, m_iHeight))
	{
		return	false;
	}

	return	true;
}

void DisplayBase::closeGLES()
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool DisplayBase::createVertexBuffer()
{
	setupTransform();

	m_iVertexBuffer	= createVertexBuffer(4, false);

	if (-1 == m_iVertexBuffer)
	{
		return	false;
	}

	if (false == setupVertices())
	{
		return	false;
	}

	return	true;
}

bool DisplayBase::setupVertices()
{
	VertexBuffer::Ptr	pVertexBuffer	= getVertexBuffer(m_iVertexBuffer);

	int	iVertices;
	
	CustomVertex*	pBuffer	= (CustomVertex*)pVertexBuffer->getBuffer(iVertices).get();

	float	fOffsetX	= static_cast<float>(m_ptOffset.iX);
	float	fOffsetY	= static_cast<float>(m_ptOffset.iY);

	// (0)*--*(2)
	//    | /|
	// (1)*--*(3)
	// UVs are flipped to adjust for OpenGL's coordinate system
	int	iIndex	= 0;

#ifdef __IOS__
	const GameHeader&	gameHeader	= System::getGameHeader();

	float	fScreenWidth	= static_cast<float>(gameHeader.iWindowedWidth);
	float	fScreenHeight	= static_cast<float>(gameHeader.iWindowedHeight);

	if (true == gameHeader.bPortrait)
	{
		pBuffer[iIndex].fX	= (320 - gameHeader.iScreenWidth) / 2;
		pBuffer[iIndex].fY	= 8 + (256 - gameHeader.iScreenHeight) / 2;
	}

	else
	{
		pBuffer[iIndex].fX	= fOffsetX;
		pBuffer[iIndex].fY	= fOffsetY;
	}
#else
	pBuffer[iIndex].fX	= fOffsetX;
	pBuffer[iIndex].fY	= fOffsetY;
#endif

	pBuffer[iIndex].fU	= 0.0;
	pBuffer[iIndex].fV	= m_fV2;
	
	iIndex++;

#ifdef __IOS__
	if (true == gameHeader.bPortrait)
	{
		pBuffer[iIndex].fX	= (320 - gameHeader.iScreenWidth) / 2;
		pBuffer[iIndex].fY	= 8 + 256 - (256 - gameHeader.iScreenHeight) / 2;
	}

	else
	{
		pBuffer[iIndex].fX	= fOffsetX;
		pBuffer[iIndex].fY	= fScreenHeight - fOffsetY;
	}
#else
	pBuffer[iIndex].fX	= fOffsetX;
	pBuffer[iIndex].fY	= fOffsetY + m_fRenderHeight;
#endif

	pBuffer[iIndex].fU	= 0.0f;
	pBuffer[iIndex].fV	= 0.0f;
	
	iIndex++;

#ifdef __IOS__
	if (true == gameHeader.bPortrait)
	{
		pBuffer[iIndex].fX	= 320 - (320 - gameHeader.iScreenWidth) / 2;
		pBuffer[iIndex].fY	= 8 + (256 - gameHeader.iScreenHeight) / 2;
	}

	else
	{
		pBuffer[iIndex].fX	= fScreenWidth - fOffsetX;
		pBuffer[iIndex].fY	= fOffsetY;
	}
#else
	pBuffer[iIndex].fX	= fOffsetX + m_fRenderWidth;
	pBuffer[iIndex].fY	= fOffsetY;
#endif

	pBuffer[iIndex].fU	= m_fU2;
	pBuffer[iIndex].fV	= m_fV2;
	
	iIndex++;

#ifdef __IOS__
	if (true == gameHeader.bPortrait)
	{
		pBuffer[iIndex].fX	= 320 - (320 - gameHeader.iScreenWidth) / 2;
		pBuffer[iIndex].fY	= 8 + 256 - (256 - gameHeader.iScreenHeight) / 2;
	}

	else
	{
		pBuffer[iIndex].fX	= fScreenWidth - fOffsetX;
		pBuffer[iIndex].fY	= fScreenHeight - fOffsetY;
	}
#else
	pBuffer[iIndex].fX	= fOffsetX + m_fRenderWidth;
	pBuffer[iIndex].fY	= fOffsetY + m_fRenderHeight;
#endif

	pBuffer[iIndex].fU	= m_fU2;
	pBuffer[iIndex].fV	= 0.0f;

	pVertexBuffer->releaseBuffer();

	return	true;
}

void DisplayBase::setupTransform()
{
	float	fScreenWidth	= (float)gsc_iGameWidth;
	float	fScreenHeight	= (float)gsc_iGameHeight;
	float	fScaleX			= (float)m_iWidth / fScreenWidth;
	float	fScaleY			= (float)m_iHeight / fScreenHeight;

	m_ptOffset.iX	= 0;
	m_ptOffset.iY	= 0;

	float	fScale;

	if (fScaleX >= fScaleY)
	{
		m_ptOffset.iX	= ((m_iWidth - static_cast<int>(fScreenWidth * fScaleY)) / 2);

		fScale	= fScaleY;
	}
	
	else
	{
		m_ptOffset.iY	= ((m_iHeight - static_cast<int>(fScreenHeight * fScaleX)) / 2);

		fScale	= fScaleX;
	}

	m_fRenderWidth	= fScreenWidth * fScale;
	m_fRenderHeight	= fScreenHeight * fScale;

	// Create simulated transform
	Matrix4f	matProjection	= Matrix4f::createOrtho(0.0f, (float)gsc_iGameWidth, (float)gsc_iGameHeight, 0.0f, 0.0f, 1.0f);
	Matrix4f	matScale		= Matrix4f::createScale(1.0f, 1.0f, 1.0f);
	Matrix4f	matTranslate	= Matrix4f::createTranslation(static_cast<float>(0.0f), static_cast<float>(0.0f), 0.0f);
	
	m_matSimulatedTransform	= matProjection * matScale * matTranslate;

	// Create render transform
	matProjection	= Matrix4f::createOrtho(0.0f, static_cast<float>(m_iWidth), static_cast<float>(m_iHeight), 0.0f, 0.0f, 1.0f);
	matScale		= Matrix4f::createScale(1.0f, 1.0f, 1.0f);
	matTranslate	= Matrix4f::createTranslation(static_cast<float>(m_ptOffset.iX), static_cast<float>(m_ptOffset.iY), 0.0f);
	
	m_matRenderTransform	= matProjection * matScale * matTranslate;
}

bool DisplayBase::compileShaders()
{
	m_uPixelShader	= glCreateShader(GL_FRAGMENT_SHADER);

	if (0 == m_uPixelShader)
	{
		Log::instance()->logError("Unable to create pixel shader %d", glGetError());

		return	false;
	}

	const GLchar*	pShader	= gsc_szPixelShader;	

	glShaderSource(m_uPixelShader, 1, &pShader, NULL);
	 
	glCompileShader(m_uPixelShader);
	
	GLint	bCompiled;

	glGetShaderiv(m_uPixelShader, GL_COMPILE_STATUS, &bCompiled);

	if (false == bCompiled)
	{
		logShaderError(m_uPixelShader, "Pixel Shader");

		return	false;
	}

	m_uVertexShader	= glCreateShader(GL_VERTEX_SHADER);

	if (0 == m_uVertexShader)
	{
		Log::instance()->logError("Unable to create vertex shader %d", glGetError());

		return	false;
	}

	pShader	= gsc_szVertexShader;

	glShaderSource(m_uVertexShader, 1, &pShader, NULL);
	 
	glCompileShader(m_uVertexShader);
	
	glGetShaderiv(m_uVertexShader, GL_COMPILE_STATUS, &bCompiled);

	if (false == bCompiled)
	{
		logShaderError(m_uVertexShader, "Vertex Shader");

		return	false;
	}

	m_uShaderProgram	= glCreateProgram();

	if (0 == m_uShaderProgram)
	{
		Log::instance()->logError("Unable to create program %d", glGetError());

		return	false;
	}

	glAttachShader(m_uShaderProgram, m_uVertexShader);
	glAttachShader(m_uShaderProgram, m_uPixelShader);
	glLinkProgram(m_uShaderProgram);

	GLint	bLinked;

	glGetProgramiv(m_uShaderProgram, GL_LINK_STATUS, &bLinked);
	
	if (false == bLinked)
	{
		logProgramError(m_uShaderProgram);

		return	false;
	}

	m_iVertexArray	= glGetAttribLocation(m_uShaderProgram, "vecVertex");
	
	if (-1 == m_iVertexArray)
	{
		Log::instance()->logError("Unable find vertex shader attribute");

		return	false;
	}
	
	m_iUVArray	= glGetAttribLocation(m_uShaderProgram, "vecUV");
	
	if (-1 == m_iUVArray)
	{
		Log::instance()->logError("Unable find texture UV shader attribute");

		return	false;
	}
	
	m_iTexture	= glGetUniformLocation(m_uShaderProgram, "pTexture");

	if (-1 == m_iTexture)
	{
		Log::instance()->logError("Unable find texture shader attribute");

		return	false;
	}

	return	true;
}

void DisplayBase::loadMatrix(const char* _szMatrixName, const Matrix4f& _matMatrix) const
{
	// Load matrix
	int iMatrix	= glGetUniformLocation(m_uShaderProgram, _szMatrixName);

	if (iMatrix != -1)
	{
		glUniformMatrix4fv(iMatrix, 1, GL_FALSE, _matMatrix.data);
	}
}

void DisplayBase::logShaderError(GLuint _uShaderID, const char* _szName) const
{
	int	iInfoLogLength;
	int	iActualSize;

	glGetShaderiv(_uShaderID, GL_INFO_LOG_LENGTH, &iInfoLogLength);

	char*	szInfoLog	= new char[iInfoLogLength];

	glGetShaderInfoLog(_uShaderID, iInfoLogLength, &iActualSize, szInfoLog);

	Log::instance()->logError("Shader error in %s: %s", _szName, szInfoLog);

	delete []	szInfoLog;
}

void DisplayBase::logProgramError(GLuint _uProgramID) const
{
	int	iInfoLogLength;
	int	iActualSize;

	glGetProgramiv(_uProgramID, GL_INFO_LOG_LENGTH, &iInfoLogLength);

	char*	szInfoLog	= new char[iInfoLogLength];

	glGetProgramInfoLog(_uProgramID, iInfoLogLength, &iActualSize, szInfoLog);

	Log::instance()->logError("Program error: %s", szInfoLog);

	delete []	szInfoLog;
}
