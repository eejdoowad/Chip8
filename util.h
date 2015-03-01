#include <stdio.h>

void c8_printMem_compact(uint8 const * const mem, uint16 first, uint16 last)
{
	//print heading row header
	printf("------"); //10 spaces
	for (uint16 i = 0; i < 0xF; ++i)
		printf("%X--", i);
	printf("F-");
	//print starting address
	printf("\n0x%02X:", first>>4);
	//print padding starting XXXX's
	for (uint16 i = 0; i < (first & 0x000F); ++i)
	{
		printf(" XX");
	}
	//print first row memory after padding
	for (uint16 i = first & 0x000F; i <= 0xF; ++i)
	{
		printf(" %02X", mem[(first & 0xFFF0) + i]); //check validity later on
	}
	//print rows first + 1 to last - 1 //again check validity later
	for (uint16 i = (first & 0xFFF0) + 0x10; i < (last & 0xFFF0); i += 0x10)
	{
		printf("\n0x%02X:", i>>4);
		for (int j = 0; j <= 0xF; ++j)
		{
			printf(" %02X", mem[i + j]);
		}
		if ((i & 0x00F0) == 0x00F0) //print heading row every 8 lines /////////////////////heres a glitch. too tired.
		{
			printf("\n------");
			for (uint16 i = 0; i < 0xF; ++i)
				printf("%X--", i);
			printf("F-");
		}
	//for (uint16 i = 0; i <= 0xF; ++i) printf("%X --- ", i);
	}
	//print last valid row //definitely are much easier ways to index through
	printf("\n0x%02X:", last>>4);
	for (uint16 i = 0; i <= (last & 0x000F); ++i)
	{
		printf(" %02X", mem[(last & 0xFFF0) + i]);
	}
	//print closing padding XXXX's
	for (uint16 i = last + 1; i <= (last | 0x000F); ++i)
	{
		printf(" XX");
	}
	printf("\n\n");
}