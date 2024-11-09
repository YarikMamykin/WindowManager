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
		void execute([[maybe_unused]] environment::Handlers& handlers,[[maybe_unused]]ymwm::window::Manager& manager)const;
	};

	struct Cmd2 {
		void execute([[maybe_unused]] environment::Handlers& handlers,[[maybe_unused]]ymwm::window::Manager& manager)const;
	};

	struct Cmd3 {
		void execute([[maybe_unused]] environment::Handlers& handlers,ymwm::window::Manager& manager)const;
	};

	struct AddGroup {
		void execute([[maybe_unused]] environment::Handlers& handlers,ymwm::window::Manager& manager)const;
	};

	struct ApplyLayout {
		window::layouts::Layout m_layout;
		EnvironmentID m_group_id;
		inline ApplyLayout(window::layouts::Layout layout,EnvironmentID group_id) : m_layout(layout), m_group_id(group_id){}
		void execute([[maybe_unused]] environment::Handlers& handlers,ymwm::window::Manager& manager)const;
	};

	using Command = std::variant<Cmd1, Cmd2, Cmd3, AddGroup, ApplyLayout>;
}
