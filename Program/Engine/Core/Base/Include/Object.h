#pragma once

#include <iostream>

namespace Rife::Base {

	class Object {
	public:
		Object();
		virtual ~Object();
        std::string getName() { return m_name; };
    protected:
		std::string m_name;
	};
}
