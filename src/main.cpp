
#include <glm/glm.hpp>
#include <vector>

#include "lib/context/context.hpp"
#include "data/world0.cpp"
#include "lib/game/player.hpp"
#include "lib/game/world.hpp"
#include "lib/game/block.hpp"

int main() {
	std::vector<Block> blocks;
	blocks.push_back(Block(world0[2], world0[3]));
	blocks.push_back(Block(world0[4], world0[5]));
	Context::ContextInit init = {
		800,
		600,
		"Glitch Game",
		World(
			world0[0],
			world0[1],
			blocks
		),
	};
	Context ctx(init);
	ctx.run();

	return 0;
}
