#pragma once

class IEditor
{
public:
    virtual void Draw() = 0;
    virtual void MenuItem() = 0;
    bool bDrawable = true;
};
