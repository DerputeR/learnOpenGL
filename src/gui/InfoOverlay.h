#pragma once

namespace GUI {
	namespace Debug	{
		void ShowOverlay(bool* open);
		template <typename T>
		class NamedValueItemReference {
			std::string name;
			T &value;
		};
	}
}

