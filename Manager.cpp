#include "Manager.h"
#include "EnvironmentID.h"
#include <iostream>

namespace ymwm::window {
	template<>
	void Manager::add<Window>([[maybe_unused]] EnvironmentID id) noexcept{
		std::cout << "Added window: " << id << std::endl;
	}

	template<>
	void Manager::add<Group>(EnvironmentID id) noexcept{
	}

	template<>
	void Manager::add<Monitor>(EnvironmentID id) noexcept{
	}

	std::vector<std::reference_wrapper<Window>> Manager::windows_in_group(EnvironmentID group_id) const noexcept{
		return {};
	}
}
