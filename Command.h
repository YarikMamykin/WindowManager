#pragma once

#include "EnvironmentID.h"
#include "Layout.h"
#include <variant>

namespace ymwm::environment {
	struct Handlers;
}
namespace ymwm::window{
	struct Manager;
}

namespace ymwm::commands {
	struct Cmd1 {
		void execute([[maybe_unused]] environment::Handlers& handlers)const;
	};

	struct Cmd2 {
		void execute([[maybe_unused]] environment::Handlers& handlers)const;
	};

	struct Cmd3 {
		ymwm::window::Manager& manager;
		inline Cmd3(ymwm::window::Manager& manager) : manager(manager){}
		void execute([[maybe_unused]] environment::Handlers& handlers)const;
	};

	struct AddGroup {
		ymwm::window::Manager& manager;
		window::layouts::Layout layout;
		EnvironmentID group_id;
		inline AddGroup(ymwm::window::Manager& manager, window::layouts::Layout layout) : manager(manager), layout(window::layouts::simple){}
		void execute([[maybe_unused]] environment::Handlers& handlers)const;
	};

	struct ApplyLayout {
		ymwm::window::Manager& manager;
		window::layouts::Layout layout;
		EnvironmentID group_id;
		inline ApplyLayout(ymwm::window::Manager& manager, window::layouts::Layout layout) : manager(manager), layout(window::layouts::simple){}
		void execute([[maybe_unused]] environment::Handlers& handlers)const;
	};

	using Command = std::variant<Cmd1, Cmd2, Cmd3, AddGroup, ApplyLayout>;
}
