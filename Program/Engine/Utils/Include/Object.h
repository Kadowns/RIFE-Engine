#pragma once
#ifndef INCLUDE_OBJECT
#define INCLUDE_OBJECT

#include <iostream>

namespace Rife::Base {

	class Object {
	public:
		Object();
		~Object();
		std::string getName();
    protected:
		std::string m_name;
	};
}
#endif // !INCLUDE_OBJECT