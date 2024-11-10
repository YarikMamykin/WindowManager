#include "Environment.h"
#include "Command.h"
#include "EventsMap.h"
#include "Layout.h"
#include "Manager.h"
#include "AbstractKeyCode.h"
#include <iostream>

int main() {
	ymwm::EventsMap events_map{
		{ymwm::events::AbstractKeyPress{.code=ymwm::events::AbstractKeyCode::A}, ymwm::commands::ApplyLayout{ymwm::window::layouts::grid, 0}}
	};
	ymwm::window::Manager m;
	ymwm::environment::Environment e{events_map, m};
	e.show_handlers();
	ymwm::commands::Command cmd;
	while(not e.exit_requested())
	{
		e.execute(events_map.at(e.event()));
	}
}
