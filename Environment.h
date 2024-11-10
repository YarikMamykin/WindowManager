#pragma once

#include <memory>
#include <variant>
#include <list>
#include "Command.h"
#include "Event.h"
#include "Handlers.h"
#include "EventsMap.h"
#include "Error.h"
#include "ErrorHandler.h"

namespace ymwm::window {
	struct Manager;
}

namespace ymwm::environment {
	struct Environment {
		Environment(const EventsMap& events_map, ymwm::window::Manager& manager);
		~Environment();
		events::Event event() const noexcept;
		void show_handlers();
		inline void execute(const commands::Command& cmd){
			std::visit([&handlers=*m_handlers, &manager=m_manager](auto c){
					c.execute(handlers, manager);
					}, cmd);
		}
		bool exit_requested() const noexcept;
		private:
			std::unique_ptr<Handlers> m_handlers;
			ymwm::window::Manager& m_manager;
			ymwm::errors::Handler m_errors_handler;
			bool m_exit_requested;
	};
}
