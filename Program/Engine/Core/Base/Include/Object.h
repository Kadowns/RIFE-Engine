#pragma once

#include <typeindex>
#include <typeinfo>

#include <iostream>
#include <fstream>

#define NAME(name) std::string(#name)

namespace Rife::Base {

	class Object {
	public:
		Object();
        virtual ~Object() {};
        std::string getName() { return m_name; }
        virtual void serialize(std::ofstream& file) = 0;
    protected:
		std::string m_name;
	};
}
