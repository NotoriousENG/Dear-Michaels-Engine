#pragma once

#include <vector>
#include <memory>
#include <Modules/Editor/IEditor.h>

class EditorModule
{
public:
	void Init(unsigned int texColorBuffer);
	void Update();
	void Shutdown();

    std::vector<std::unique_ptr<IEditor>> editors;

    bool show_demo_window = false;

private:
	unsigned int texColorBuffer;
};

