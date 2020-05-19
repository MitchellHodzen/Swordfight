#pragma once

class KRenderer;

class RenderSystem
{
public:
	void Draw(KRenderer& kRenderer, bool drawColliders);
private:
	void DrawColliders(KRenderer& kRenderer);
};

