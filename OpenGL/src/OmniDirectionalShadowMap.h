#pragma once

#include "ShadowMap.h" 

class OmniDirectionalShadowMap : public ShadowMap 
{

public:
	OmniDirectionalShadowMap();

	OmniDirectionalShadowMap(int WIDTH, int HEIGHT);

	bool init() override;

	void writeBuffer() override;

	void readBuffer(GLenum unit) override;

	~OmniDirectionalShadowMap();

};

