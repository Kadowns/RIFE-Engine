#pragma once
#ifndef INCLUDE_COMPONENT
#define INCLUDE_COMPONENT

namespace rifebase {
	class Component {
	public:
		
		virtual void awake() = 0;
		virtual void setup() = 0;
		virtual void update() = 0;
	protected:

	};
}



#endif //INCLUDE_COMPONENT
