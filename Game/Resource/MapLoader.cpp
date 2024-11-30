#include "MapLoader.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <exception>
#include <format>

#include <Resource/WadFile.hpp>

DoomMapLoader::DoomMapLoader(const std::string&& fileName, const std::string&& mapName)
	: wadFile(fileName), mapName(mapName)
{
	/* We don't want to load the map in the constructor */
}

std::unique_ptr<Level> DoomMapLoader::loadLevel()
{
	std::unique_ptr<Level> level = std::make_unique<Level>();

	// TODO: Implement

	return std::move(level);
}