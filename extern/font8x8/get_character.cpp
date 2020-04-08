#include "font8x8.h"

////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////

struct character_range
{
	unsigned start, end;
	const uint64_t *map;
};

static const struct character_range character_maps[] = {
	{0x0000, 0x007F, font8x8_basic},
	{0x0080, 0x009F, font8x8_control},
	{0x00A0, 0x00FF, font8x8_ext_latin},
	{0x0390, 0x03C9, font8x8_greek},
	{0x2500, 0x257F, font8x8_box},
	{0x2580, 0x259F, font8x8_block},
	{0x3040, 0x309F, font8x8_hiragana}
};

static const unsigned int character_maps_count = sizeof(character_maps) / sizeof(character_maps[0]);

////////////////////////////////////////////////////////////////////////////////

/*
** Returns the bitmap for any supported unicode character
*/
uint64_t font8x8_get_character(int c)
{
	unsigned u = (unsigned)c;

	for (size_t i = 0; i < character_maps_count; ++i) {
		const struct character_range *range = &character_maps[i];
		if (u >= range->start && u <= range->end)
			return range->map[u - range->start];
	}
	return 0;
}