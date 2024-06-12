#ifndef __openwars__game__map__map__h__
#define __openwars__game__map__map__h__

#include <raylib.h>
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace Map {
	typedef struct {
		bool		is_ready;
		uint16_t	id;
		uint16_t	width;
		uint16_t	height;
		// Big-Endian R8G8B8A8 format.
		uint8_t		*data;
	} map_tile_t;
	
	typedef std::unordered_map<uint16_t, map_tile_t *> map_tiles_index_t;
	
	typedef struct {
		// The map width and height, in tiles.
		uint16_t			width;
		uint16_t			height;
		// A table that links the tiles IDs, to the tiles themselves.
		map_tiles_index_t	index;
		// A pointer to the map tiles data.
		uint16_t			*tiles;
		// The map's thumbnail.
		map_tile_t			thumb;
		
		// The decoder status code. If negative, that's bad.
		int					status_code;
		const char			*status_message = nullptr;
		
		// The name of the mod for which this map was made.
		// It may be a Git repository link.
		// E.g.: "openwars", or "https://github.com/ilaicraftYT/OpenWars".
		char				*mod;
	} map_t;
	
	map_t load(const char *name);
	void draw(map_t map, int screenWidth, int screenHeight);
} // namespace Map

#endif
