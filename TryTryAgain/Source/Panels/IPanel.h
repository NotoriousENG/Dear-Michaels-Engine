#pragma once

namespace Panels {
	class IPanel
	{
	public:

		bool isActive = true;
		virtual void Draw() = 0;
		virtual void MenuItem() {};
	};
}
