
#include "lib/game_objects/context.hpp"

int main() {
	Context::ContextInit init = {
		800,
		600,
		"Glitch Game"
	};
	Context ctx(init);
	ctx.run();
}