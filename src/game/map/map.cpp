#ifndef __openwars__game__map__map__cxx__
#define __openwars__game__map__map__cxx__

#include "tiles.cpp"
#include "../../config/config.h"
#include "codeve.cpp"

#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <unordered_map>

uint16_t read_u16be(uint8_t *ptr, uint32_t offset) {
	uint8_t *data = ptr + offset;
	
	return	(((uint16_t)*data++ << 8) |
			((uint16_t)*data++));
};

uint32_t read_u32be(uint8_t *ptr, uint32_t offset) {
	uint8_t *data = ptr + offset;
	
	return	(((uint32_t)*data++ << 24) |
			((uint32_t)*data++ << 16) |
			((uint32_t)*data++ << 8) |
			((uint32_t)*data++));
};

uint64_t read_u64be(uint8_t *ptr, uint32_t offset) {
	uint8_t *data = ptr + offset;
	
	return	(((uint64_t)*data++ << 56) |
			((uint64_t)*data++ << 48) |
			((uint64_t)*data++ << 40) |
			((uint64_t)*data++ << 32) |
			((uint64_t)*data++ << 24) |
			((uint64_t)*data++ << 16) |
			((uint64_t)*data++ << 8) |
			((uint64_t)*data++));
};

Map::map_t Map::load(const char *name) {
	// TODO : Add error handling.
	char *path = std::strcat(std::strcat(Config::CONFIG_PATH, "/maps/"), name);
	std::ifstream rs(path);
	
	Map::map_t ret_map;
	
	// The file structure of [OpenWars] maps contains some similarities to the
	// image format, [Portable Network Graphics](https://www.w3.org/TR/png/):
	// 
	// It starts with a "magic number" (Big-Endian 0x894f57734d0d0a1a).
	// Then, a serie of "chunks" come after.
	// 
	// A chunk consists of four parts:
	// 
	// - The chunk body length (Big-Endian unsigned 32-bit integer).
	// - Four case-sensitive, ASCII characters, that sets the chunk name.
	// - The chunk body itself.
	// - A Big-Endian 32-byte checksum of the entire chunk, excluding itself.
	// 
	// + The first letter indicates whether the chunk is critical (uppercase)
	// or ancillary (lowercase).
	// If the chunk is critical but the decoder doesn't supports it, it must
	// abort the decoding process, or supply the user with an appropiate
	// warning.
	// + The second letter indicates whether the chunk is chunk is "public"
	// (uppercase) or not (lowercase).
	// + The third letter indicates whether the chunk is safe to copy by
	// editors that don't recognize it (uppercase) or not (lowercase).
	// + The fourth letter is reserved for future expansion, and must be
	// uppercase.
	// 
	// "MHDR" contains:
	// 
	// - The map's width (Big-Endian unsigned 16-bit integer) in tiles.
	// - The map's height (Big-Endian unsigned 16-bit integer) in tiles.
	// - The map's background tile (Big-Endian unsigned 16-bit integer).
	// 
	// "MROW" contains a single row of map tiles.
	// 
	// "MEND" marks the file end; its data field is empty.
	// If the decoder detects this header, it will stop reading, even if the
	// file still has bytes left.
	// 
	// "MTEX" contains a serie of (ASCIIZ) strings, which indicate the decoder
	// which external textures to load ("game/textures/<texture>").
	// E.g.: "grass.png", "dirt.png", "bridge.png", et cetera.
	// An entry in "MTEX" may be empty, however, it may be trated as a valid
	// one (maybe it will be *width:1 height:1 alpha:0*).
	// 
	// "MIMG" contains a custom texture to be loaded.
	// E.g.: R8G8B8A8 raw image, with its width and height.
	// 
	
	// We initialize Codeve256.
	Codeve1::init_codeve256();
	Codeve1::Codeve256 hash;
	uint8_t *chunk_digest = nullptr;
	
	// ̵~6̵4̵0̵  64 KiB ought to be enough for anybody.
	uint8_t sbuff[0x10000];
	uint8_t chunk_hash[32];
	
	// TODO : Add error handling.
	rs.read((char *)sbuff, 8);
	
	// Check for signature.
	if(read_u64be(sbuff, 0) != 0x894f57734d0d0a1a) {
		ret_map.status_code = -1;
		ret_map.status_message = "Signature not matching.";
		return ret_map;
	}
	
	bool pending_chunks = true, mhdr_ready = false;
	uint32_t chunk_length;
	char chunk_name[5];
	chunk_name[4] = '\0';
	
	uint32_t file_offset;
	
	uint16_t map_bg_tile = 0xffff;
	
	while(pending_chunks) {
		file_offset = 0;
		
		// We read chunk body length.
		chunk_length = read_u32be(sbuff, 0);
		
		// We read the chunk name.
		rs.read((char *)chunk_name, 4);
		
		// If the chunk body length is too big, and the chunk is critical,
		// we throw an exception. If it's not, we just skip over it.
		if(chunk_length > sizeof(sbuff)) {
			if(chunk_name[0] & 0b00100000) {
				ret_map.status_code = -2;
				ret_map.status_message = "Chunk body is too big.";
				return ret_map;
			} else {
				while(chunk_length > sizeof(sbuff)) {
					rs.read((char *)sbuff, sizeof(sbuff));
					chunk_length -= sizeof(sbuff);
				};
				
				rs.read((char *)sbuff, chunk_length);
				
				continue;
			}
		}
		
		rs.read((char *)chunk_hash, 32);
		
		hash.reset();
		hash.update(chunk_length, sbuff, 0);
		chunk_digest = hash.digest();
		
		// We check if the chunk is corrupted, if that's true, we throw an
		// exception.
		for(uint8_t i = 0; i < 32; i++) {
			if(chunk_digest[i] != chunk_hash[i]) {
				ret_map.status_code = -3;
				ret_map.status_message = "The chunk's checksum doesn't match.";
				return ret_map;
			};
		};
		
		if(std::strcmp(chunk_name, "MHDR") == 0) {
			mhdr_ready = true;
			
			rs.read((char *)sbuff, 6);
			
			ret_map.width = read_u16be(sbuff, 0);
			ret_map.height = read_u16be(sbuff, 2);
			map_bg_tile = read_u16be(sbuff, 4);
			
			continue;
		}
		
		// The first chunk should be a "MHDR" chunk. If not, we throw an
		// exception.
		if(mhdr_ready != true) {
			ret_map.status_code = -4;
			ret_map.status_message = "First chunk was not a \"MHDR\" one.";
			return ret_map;
		}
		
		if(std::strcmp(chunk_name, "MROW") == 0) {
			// The first byte is the major version.
			uint8_t ver = sbuff[0];
			
			if(ver != 0) {
				ret_map.status_code = -12;
				ret_map.status_message =	"The decoder can't read a major "
											"version MROW chunk.";
				
				return ret_map;
			}
			
			// Then it comes this chunk flags.
			uint32_t flags = read_u32be(sbuff, 1);
			
			chunk_length -= 5;
			
			// This chunk, with no flags, is 16-bit tiles serie.
			// We check the byte boundaries.
			if(flags == 0x00000000) {
				if((chunk_length & 0xfffffffe) == chunk_length) {
					ret_map.status_code = -5;
					ret_map.status_message = "Invalid image's byte boundary.";
					return ret_map;
				}
				
				// We check if the amount of tiles is greater than the map's
				// width.
				if((chunk_length >> 1) > ret_map.width) {
					ret_map.status_code = -6;
					ret_map.status_message = "The row is too big.";
					return ret_map;
				}
				
				/* TODO
				// What the hell is this?
				for(uint16_t i = 0, x; i < ret_map.width; i++) {
					*tiles++ =	(i > (chunk_length >> 1))
								? (*tiles++ = 0x0000)
								: (*tiles++ = read_u16be(sbuff, i << 1));
				}; */
				
				continue;
			}
			
			ret_map.status_code = -6;
			ret_map.status_message =	"The specified flags for the image "
										"are not supported."; // TODO
			
			return ret_map;
		}
		
		if(std::strcmp(chunk_name, "MEND") == 0) {
			break;
		}
		
		// TODO : Todo lo a continuación
		if(std::strcmp(chunk_name, "MTEX") == 0) {
			continue;
		}
		
		if(std::strcmp(chunk_name, "MBLK") == 0) {
			continue;
		}
		
		if(std::strcmp(chunk_name, "MIMG") == 0) {
			// The first byte is the major version.
			uint8_t ver = sbuff[0];
			
			if(ver != 0) {
				ret_map.status_code = -11;
				ret_map.status_message =	"The decoder can't read a major "
											"version MIMG chunk.";
				
				return ret_map;
			}
			
			// Then it comes this chunk flags.
			uint32_t flags = read_u32be(sbuff, 1);
			
			// Then the image width and height, in pixels.
			map_tile_t tile;
			tile.width = read_u16be(sbuff, 5);
			tile.height = read_u16be(sbuff, 7);
			
			// Then the tile ID.
			tile.id = read_u16be(sbuff, 9);
			tile.is_set = true;
			
			chunk_length -= 11;
			
			// We alloc enough memory to store the image.
			tile.data = new uint8_t[(tile.width * tile.height) << 2];
			
			// Raw Big-Endian R8G8B8A8 image.
			if(flags == 0x00000000) {
				// If the chunk data length is not equal than it should be, we
				// throw an exception.
				if(chunk_length != ((img_width * img_height) << 2)) {
					delete tile.data;
					
					ret_map.status_code = -7;
					ret_map.status_message =	"The chunk data length for ";
												"the image is odd."
					return ret_map;
				}
				
				// We start decoding the image.
				for(uint32_t i = 0; i < chunk_length; i++) {
					tile.data[i] = i;
				};
			}
			
			// QOI image.
			if(flags & 0x00000001) {
				// TODO
			}
			
			map_tiles_index_t[tile.id] = tile;
		}
		
		// If it's a "Future expansion" chunk, we just throw an exception.
		if((chunk_name[3] & 0b00100000) == 0x00000000) {
			ret_map.status_code = -8;
			ret_map.status_message =	"The decoder can't read a major "
										"expansion chunk.";
			
			return ret_map;
		}
		
		// Critical or ancillary.
		if(chunk_name[0] & 0b00100000) {
			ret_map.status_code = -9;
			ret_map.status_message = "Chunk name 'XXXX' not recognized."; // TODO
			return ret_map;
		} else {
			// For now, we'll discard it.
			continue;
		}
		
		ret_map.status_code = -10;
		ret_map.status_message = "Unhandled chunk."; //TODO
		
		return ret_map;
	};
	
	// We free the shared buffer.
	delete sbuff;
};

#endif
