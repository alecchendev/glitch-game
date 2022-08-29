
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "lib/context/context.hpp"
#include "data/world0.cpp"
#include "lib/game/player.hpp"
#include "lib/game/world.hpp"
#include "lib/game/block.hpp"
#include "lib/graphics/shader.h"

int main() {

	// Read in world from world data
	std::vector<Block> blocks;
	blocks.push_back(Block(world_data::world0[2], world_data::world0[3]));
	blocks.push_back(Block(world_data::world0[4], world_data::world0[5]));
	World world(
		world_data::world0[0],
		world_data::world0[1],
		blocks
	);

	// Create context initialization struct
	Context::ContextInit init = {
		800,
		600,
		"Glitch Game",
		world
	};

	// Create and run context
	Context ctx(init);
	ctx.run();

	return 0;
}
