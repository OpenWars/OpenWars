#ifndef __openwars__game__map__map__hpp__
#define __openwars__game__map__map__hpp__

#include "../../defs.hpp"
#include "../../visual/visual.hpp"

namespace OpenWars {
	typedef struct {
		u16			id;
		u16			textures_len;
		texture_t	*textures;
	} map_tile_t;

	typedef struct {
		// The map width and height, in tiles.
		u16			width;
		u16			height;
		// A table that links the tiles IDs, to the tiles themselves.
		map_tile_t	*tiles_map;
		// A pointer to the map tiles data.
		u16			*tiles;
		// The map's thumbnail.
		texture_t	*thumbnail;
		// The name of the game for which this map was made.
		// It may be a Git repository link.
		// E.g.: "openwars", or "https://github.com/ilaicraftYT/OpenWars".
		char		*mod;
	} map_t;

	extern ErrorOr<map_t *> load_map(const char *path);
};

#endif
