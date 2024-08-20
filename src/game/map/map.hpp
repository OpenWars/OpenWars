/**
 *
   ___                __        __             
  / _ \ _ __   ___ _ _\ \      / /_ _ _ __ ___ 
 | | | | '_ \ / _ \ '_ \ \ /\ / / _` | '__/ __|
 | |_| | |_) |  __/ | | \ V  V / (_| | |  \__ \
  \___/| .__/ \___|_| |_|\_/\_/ \__,_|_|  |___/
       |_|     

Copyright (C) 2024 OpenWars Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef __openwars__game__map__map__hpp__
#define __openwars__game__map__map__hpp__

#include "../../nuclei.hpp"
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

	extern map_t * load_map(const char *path, const char *err);
};

#endif
