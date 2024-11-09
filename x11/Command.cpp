
#include "../Command.h"
#include "./Handlers.h"
#include "../Window.h"
#include "../Manager.h"
#include <iostream>

namespace ymwm::commands {
	void Cmd1::execute([[maybe_unused]] environment::Handlers& handlers,[[maybe_unused]]ymwm::window::Manager& manager) const {

		std::cout << "CMD 1: " << handlers.a << " " << handlers.b << std::endl;
	}

	void Cmd2::execute([[maybe_unused]] environment::Handlers& handlers,[[maybe_unused]]ymwm::window::Manager& manager)const  {
		std::cout << "CMD 2: " << handlers.a << " " << handlers.b << std::endl;
	}

	void Cmd3::execute([[maybe_unused]] environment::Handlers& handlers,ymwm::window::Manager& manager)const  {
		manager.add<window::Window>(213);
	}
	
	void AddGroup::execute([[maybe_unused]] environment::Handlers& handlers,ymwm::window::Manager& manager)const  {
		manager.add<window::Group>({});
	}

	void ApplyLayout::execute([[maybe_unused]] environment::Handlers& handlers,ymwm::window::Manager& manager)const  {
		manager.apply_layout(m_group_id, m_layout);
	}
}
