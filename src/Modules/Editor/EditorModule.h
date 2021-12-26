#pragma once

#include <Scene/Editor/SceneEditor.h>

class EditorModule
{
public:
	void Init(unsigned int texColorBuffer);
	void Update();
	void Shutdown();

	SceneEditor se;

private:
	unsigned int texColorBuffer;
};

