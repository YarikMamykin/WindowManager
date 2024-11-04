#include "Environment.h"
#include "Command.h"
#include "Manager.h"
#include "Window.h"
#include "AbstractKeyCode.h"
#include <iostream>

int main() {
	ymwm::environment::Environment e;
	e.show_handlers();
	ymwm::commands::Command cmd;
	e.execute(cmd);
	cmd.emplace<ymwm::commands::Cmd2>();
	e.execute(cmd);
	ymwm::window::Manager m;
	m.add<ymwm::window::Window>(0);
	m.add<ymwm::window::Group>(0);
	m.add<ymwm::window::Monitor>(0);
	cmd.emplace<ymwm::commands::Cmd3>(m);
	e.execute(cmd);
	int kc = ymwm::AbstractKeyCode::A;
	std::cout << kc << std::endl;
}
