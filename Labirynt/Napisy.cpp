#include "Napisy.h"

Napisy::Napisy()
{
}

Napisy::~Napisy()
{
}

void Napisy::KlawMenu(INT frame[19], INT czerwony[18][18])
{
	for (INT fram = 0; fram < 16; fram++)
	{
		for (INT framik = 0; framik < 18; framik++)
		{
			if (frame[fram] == framik)
				czerwony[fram][framik] = 1;

			else if (frame[fram] != framik)
				czerwony[fram][framik] = 0;
		}
	}

	for (INT framik = 0; framik < 6; framik++)
	{
		if (frame[17] == framik)
			czerwony[16][framik] = 1;

		else if (frame[17] != framik)
			czerwony[16][framik] = 0;
	}

	for (INT framik = 0; framik < 18; framik++)
	{
		if (frame[18] == framik)
			czerwony[17][framik] = 1;

		else if (frame[18] != framik)
			czerwony[17][framik] = 0;
	}
}
