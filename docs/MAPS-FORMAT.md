# OpenWars map format

**Note:** the purpose of this document is not to provide the most accurate, or
up-to-date information possible, but to provide satisfactory documentation for
those who want to create OpenWars map editors.

The structure of OpenWars map files is inspired by that of PNG. As such, it'll
be based on "chunks" that provide certain information.

The structure of a chunk is as follows:

- Four ASCII letters that will determine the type of chunk.
- A 32-bit unsigned integer that will determine the length of the chunk data.
- The chunk data itself...
- An 8-bit unsigned integer that will determine the "integrity checksum"
  algorithm to use.
- The 32-byte checksum itself, which includes the type of chunk, chunk length,
  chunk data, and checksum type.

The four ASCII letters are case-sensitive, and these will determine the type
of chunk:

* The first letter of the "chunk type" indicates whether the chunk is critical
  (uppercase) or ancillary (lowercase).
  If the decoder doesn't support a certain **critical** chunk, it may stop the
  decoding process entirely, or warn the user about the unsupported chunk.
* The second letter of the "chunk type" indicates whether the chunk is "public"
  (uppercase) or not (lowercase).
  "Public" means that the chunk is either a chunk specified in this document,
  ("standardized") or accepted by most of the decoders ("de facto standard").
  "Private" means that the chunk is only use by a set of individuals, it is
  under heavy development, or it is not indended to be a public one at all.
* The third letter indicates whether the chunk is "safe to copy" by a decoder
  that does not supports it (uppercase), or not (lowercase).
* The fourth letter is reserved for future expansion, and must be uppercase.

## Chunks list

### Chunks list: "MHDR"

This chunk contains the map's width and height, in tiles, as base-1 Big-Endian
unsigned 16-bit integers.

This chunk MUST be the first one in the file.

### Chunks list: "MEND"

This chunk marks the end of the file, and tells the decoder that no further
chunks may be processed, even if the file still goes.

### Chunks list: "MTEX"

This chunk constains the "external" (not included in the map itself) tiles
textures to be loaded.

These consists of a 16-bit unsigned integer (Big-Endian), indicating the tile
ID, followed by a null-terminated string that contains the file path.

The file path must be started either by a '`/`' which will signal that the file
is stored in the user's game directory (e.g.: `~/.config/openwars`), or by a
'`.`', which will signal that the file is stored in the same directory as the
map.

### Chunks list: "MTIN"

This chunk provides information about the tile texture to be loaded.

The structure of this chunk is as follows:

- The tile ID, as a 16-bit unsigned integer (stored as Big-Endian).
- The texture's width in pixels, as a 16-bit unsigned integer (Big-Endian).
- The texture's height in pixels, as a 16-bit unsigned integer (Big-Endian).
- The format in which the texture is stored, as case-sensitive, three ASCII
  letters.
- The texture pixel data.

Texture format e.g.: "PNG", "QOI", "GIF".

### Chunks list: "MROW"

This chunk provides a serie of tiles to be placed on the map.
It consists of the X and Y offset (stored as Big-Endian 16-bit unsigned
integers) at which to start placing the tiles, and the tiles themselves
(also stored as Big-Endian 16-bit unsigned integers).
