#pragma once
#ifndef INCLUDE_OBJECT
#define INCLUDE_OBJECT

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
#endif // !INCLUDE_OBJECT