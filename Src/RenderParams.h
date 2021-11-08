#pragma once

struct RenderParams
{
	int		iTextureHash;	// Texture hash
	float	fX;				// Position
	float	fY;
	float	fWidth;			// Frame size
	float	fHeight;
	float	fU1;			// Frame UVs
	float	fV1;
	float	fU2;
	float	fV2;

	RenderParams() : iTextureHash(0), fX(0.0f), fY(0.0f), fWidth(0.0f), fHeight(0.0f), fU1(0.0f), fV1(0.0f), fU2(0.0f), fV2(0.0f) {}
};
