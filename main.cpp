#include "Environment.h"
#include "Command.h"

int main() {
	ymwm::environment::Environment e;
	e.show_handlers();
	ymwm::commands::Command cmd;
	e.execute(cmd);
	cmd.emplace<ymwm::commands::Cmd2>();
	e.execute(cmd);
}
