#pragma once

class KRenderer;

class RenderSystem
{
public:
	void DrawSprites(KRenderer& kRenderer);
	void DrawColliders(KRenderer& kRenderer);
	void DrawToScreen(KRenderer& kRenderer);
};

