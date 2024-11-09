#include "../Environment.h"
#include "./Handlers.h"
#include <iostream>
#include <memory>
#include "../EventsMap.h"
#include "../AbstractKeyCode.h"

namespace ymwm::environment {
	Environment::Environment(const EventsMap& events_map, ymwm::window::Manager& manager) 
		: m_handlers(std::make_unique<Handlers>(10,23.3)), m_manager(manager){
		// Grab keys by events
	}
	Environment::~Environment() = default;
	void Environment::show_handlers() {
		std::cout << m_handlers->a << " " << m_handlers->b << std::endl;
	}

	events::Event Environment::event() const noexcept{
		// Gets environment from x11 event loop
		// and returns it
		return ymwm::events::AbstractKeyPress{.code = ymwm::events::AbstractKeyCode::A};
	}
}
