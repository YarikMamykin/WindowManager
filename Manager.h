#pragma once

#include <map>
#include <optional>
#include "./Window.h"
#include "EnvironmentID.h"
#include "Layout.h"

namespace ymwm::window {

	struct Group {
		layouts::Layout layout{layouts::simple};
		EnvironmentID monitor;
		std::optional<EnvironmentID> focused; // Focused window.
	};

	struct Monitor {
		EnvironmentID id;
		EnvironmentID active_group;
		int x;
		int y;
		int w;
		int h;
	};


	struct Manager {
		template<typename Entity>
		void add([[maybe_unused]] EnvironmentID id) noexcept;

		template<typename Entity>
		void remove(EnvironmentID id) noexcept;

		std::vector<std::reference_wrapper<Window>> windows_in_group(EnvironmentID group_id) const noexcept;

		void apply_layout(EnvironmentID group_id, layouts::Layout layout) noexcept;

		private:
			std::vector<Window> m_windows;
			std::vector<Group> m_groups;
			std::map<EnvironmentID, Monitor> m_monitor;
			EnvironmentID m_active_monitor;
	};
}
