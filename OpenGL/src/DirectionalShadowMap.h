#pragma once

#include "ShadowMap.h"

class DirectionalShadowMap : public ShadowMap
{

public:

	DirectionalShadowMap();

	DirectionalShadowMap(int WIDTH, int HEIGHT);

	bool init() override;

	void writeBuffer() override;

	void readBuffer(GLenum unit) override;

	~DirectionalShadowMap();
};

