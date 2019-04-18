/*
** EPITECH PROJECT, 2019
** G13
** File description:
** pbm2lpbm.cpp
*/

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>

// convert a .pbm raw file to our custom .lpbm format

int main(int /*argc*/, char **/*argv*/)
{
	const int LEN = 256;

	char s[LEN] = {0};
	std::cin.getline(s, LEN);
	if (strncmp(s, "P4", 2) != 0) {
		std::cerr << "Input file is not .pbm (P4)" << std::endl;
		return EXIT_FAILURE;
	}

	std::cin.getline(s, LEN);
	while (s[0] == '#' || s[0] == ' ')
		std::cin.getline(s, LEN);

	unsigned int w = 0, h = 0;
	if (std::sscanf(s, "%d %d", &w, &h) != 2) {
		std::cerr << "Height and width not found" << std::endl;
		return EXIT_FAILURE;
	}
	if (w != 160 || h != 43) {
		std::cerr << "Incorrect width/height values" << std::endl;
		std::cerr << "	Mandated: 160x43" << std::endl;
		std::cerr << "	Found: " << w << "x" << h << std::endl;
		return EXIT_FAILURE;
	}

	std::cin >> std::noskipws;

	int i = 0, row = -1;
	unsigned char buf[160 * 48] = {0};

	unsigned char c = 0;
	while (std::cin >> c)
	{
		if (i % 20 == 0)
			row++;

		if (row == 8)
			row = 0;

		buf[7 + (i % 20) * 8 + i / 160 *160] |= ((c >> 0) & 0x01) << row;
		buf[6 + (i % 20) * 8 + i / 160 *160] |= ((c >> 1) & 0x01) << row;
		buf[5 + (i % 20) * 8 + i / 160 *160] |= ((c >> 2) & 0x01) << row;
		buf[4 + (i % 20) * 8 + i / 160 *160] |= ((c >> 3) & 0x01) << row;
		buf[3 + (i % 20) * 8 + i / 160 *160] |= ((c >> 4) & 0x01) << row;
		buf[2 + (i % 20) * 8 + i / 160 *160] |= ((c >> 5) & 0x01) << row;
		buf[1 + (i % 20) * 8 + i / 160 *160] |= ((c >> 6) & 0x01) << row;
		buf[0 + (i % 20) * 8 + i / 160 *160] |= ((c >> 7) & 0x01) << row;

		i++;
	}

	const size_t expected = 160 * 43 / 8;
	if (i != expected)
		std::cerr << "Wrong number of bytes, expected " << expected << ", got " << i << std::endl;

	for (size_t i = 0; i < 160 * 48 / 8; i++)
		std::cout << std::hex << (char)buf[i];

	return EXIT_SUCCESS;
}