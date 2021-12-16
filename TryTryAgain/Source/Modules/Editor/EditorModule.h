#pragma once
class EditorModule
{
public:
	void Init(unsigned int texColorBuffer);
	void Update();
	void Shutdown();

private:
	unsigned int texColorBuffer;
};

