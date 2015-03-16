#include <string.h>
#include "c8_util.h"


void cpu_print_regs(CPU const * const cpu)
{
	printf("op 0x%04x     PC 0x%04x     I 0x%04x\nSP 0x%02x       delay 0x%02x    sound 0x%02x\n\n", cpu->opcode, cpu->PC, cpu->I, cpu->SP, cpu->delay_timer, cpu->sound_timer);
	printf("#     V        Stack    Keys\n");
	for (int i = 0; i < 16; i++)
	{
		printf("%1x     %04x     %04x     %04x\n", i, cpu->V[i], cpu->stack[i], cpu->keys[i]);
	}
}

void cpu_print_screen(CPU const * const cpu)
{
	printf("    0         1         2         3         4         5         6   \n");
	printf("    0123456789012345678901234567890123456789012345678901234567890123\n\n");
	for (int i = 0; i < SCR_H; i++)
	{
		printf("%02d  ", i);
		for (int j = 0; j < SCR_W; j++)
		{
			putchar(cpu->screen[i * SCR_W + j] ? '@' : ' ');
		}
		printf("  %02d\n", i);
	}
	printf("\n");
	printf("    0         1         2         3         4         5         6   \n");
	printf("    0123456789012345678901234567890123456789012345678901234567890123\n\n");
}


void cpu_print_mem(CPU const * const cpu, uint16_t first, uint16_t last)
{
	if ((first < 0) || (last > 4096) || first > last)
	{
		printf("Failure: Memory Selection out of range.");
		exit(1);
	}

	uint8_t const * const mem = cpu->mem;
	//print heading row header
	printf("------"); //10 spaces
	for (uint16_t i = 0; i < 0xF; ++i)
		printf("%X--", i);
	printf("F-");
	//print starting address
	printf("\n0x%02X:", first>>4);
	//print padding starting XXXX's
	for (uint16_t i = 0; i < (first & 0x000F); ++i)
	{
		printf("   ");
	}
	//print first row memory after padding
	for (uint16_t i = first & 0x000F; i <= 0xF; ++i)
	{
		printf(" %02X", mem[(first & 0xFFF0) + i]); //check validity later on
	}
	//print rows first + 1 to last - 1 //again check validity later
	for (uint16_t i = (first & 0xFFF0) + 0x10; i < (last & 0xFFF0); i += 0x10)
	{
		printf("\n0x%02X:", i>>4);
		for (int j = 0; j <= 0xF; ++j)
		{
			printf(" %02X", mem[i + j]);
		}
		if ((i & 0x00F0) == 0x00F0) //print heading row every 8 lines /////////////////////heres a glitch. too tired.
		{
			printf("\n------");
			for (uint16_t i = 0; i < 0xF; ++i)
				printf("%X--", i);
			printf("F-");
		}
	//for (uint16_t i = 0; i <= 0xF; ++i) printf("%X --- ", i);
	}
	//print last valid row //definitely are much easier ways to index through
	printf("\n0x%02X:", last>>4);
	for (uint16_t i = 0; i <= (last & 0x000F); ++i)
	{
		printf(" %02X", mem[(last & 0xFFF0) + i]);
	}
	//print closing padding XXXX's
	for (uint16_t i = last + 1; i <= (last | 0x000F); ++i)
	{
		printf("   ");
	}
	printf("\n\n");
}

void cpu_print_everything(CPU const * const cpu, uint16_t first, uint16_t last)
{
	cpu_print_regs(cpu);
	cpu_print_screen(cpu);
	cpu_print_mem(cpu, first, last);
}

void cpu_print_current_instruction(CPU const * const cpu)
{
	char assembly[MAX_ASSEMBLY_LEN];
	assemble(cpu->opcode, assembly);
	printf("PC=0x%03X, SP=%X, Ins=0x%04X %s\n", cpu->PC, cpu->SP, cpu->opcode, assembly);
}

static void(*assembly_table[16])(const uint16_t opcode, char * const assembly);
static void i_0xxx(const uint16_t opcode, char * const assembly);
static void i_8xxx(const uint16_t opcode, char * const assembly);
static void i_Exxx(const uint16_t opcode, char * const assembly);
static void i_Fxxx(const uint16_t opcode, char * const assembly);
static void i_0nnn(const uint16_t opcode, char * const assembly);
static void i_00E0(const uint16_t opcode, char * const assembly);
static void i_00EE(const uint16_t opcode, char * const assembly);
static void i_1nnn(const uint16_t opcode, char * const assembly);
static void i_2nnn(const uint16_t opcode, char * const assembly);
static void i_3xkk(const uint16_t opcode, char * const assembly);
static void i_4xkk(const uint16_t opcode, char * const assembly);
static void i_5xy0(const uint16_t opcode, char * const assembly);
static void i_6xkk(const uint16_t opcode, char * const assembly);
static void i_7xkk(const uint16_t opcode, char * const assembly);
static void i_8xy0(const uint16_t opcode, char * const assembly);
static void i_8xy1(const uint16_t opcode, char * const assembly);
static void i_8xy2(const uint16_t opcode, char * const assembly);
static void i_8xy3(const uint16_t opcode, char * const assembly);
static void i_8xy4(const uint16_t opcode, char * const assembly);
static void i_8xy5(const uint16_t opcode, char * const assembly);
static void i_8xy6(const uint16_t opcode, char * const assembly);
static void i_8xy7(const uint16_t opcode, char * const assembly);
static void i_8xyE(const uint16_t opcode, char * const assembly);
static void i_9xy0(const uint16_t opcode, char * const assembly);
static void i_Annn(const uint16_t opcode, char * const assembly);
static void i_Bnnn(const uint16_t opcode, char * const assembly);
static void i_Cxkk(const uint16_t opcode, char * const assembly);
static void i_Dxyn(const uint16_t opcode, char * const assembly);
static void i_Ex9E(const uint16_t opcode, char * const assembly);
static void i_ExA1(const uint16_t opcode, char * const assembly);
static void i_Fx07(const uint16_t opcode, char * const assembly);
static void i_Fx0A(const uint16_t opcode, char * const assembly);
static void i_Fx15(const uint16_t opcode, char * const assembly);
static void i_Fx18(const uint16_t opcode, char * const assembly);
static void i_Fx1E(const uint16_t opcode, char * const assembly);
static void i_Fx29(const uint16_t opcode, char * const assembly);
static void i_Fx33(const uint16_t opcode, char * const assembly);
static void i_Fx55(const uint16_t opcode, char * const assembly);
static void i_Fx65(const uint16_t opcode, char * const assembly);

void assemble(const uint16_t opcode, char * const assembly)
{
	assembly_table[(opcode & 0xF000) >> 12](opcode, assembly);
}

void generateAssemblyFile(char const * const binfile)
{
	// open ROM
	FILE * binfp;
	if ((binfp = fopen(binfile, "rb")) == NULL)
	{
		fprintf(stderr, "Opening ROM  \"%s\" failed\n", binfile);
		fflush(stderr);
		exit(1);
	}

	// open assembly file
	char asmfile[512];
	strcpy(asmfile, binfile);
	strcat(asmfile, ".c8asm");
	FILE * asmfp;
	if ((asmfp = fopen(asmfile, "wb")) == NULL)
	{
		fprintf(stderr, "Generating  \"%s\" failed\n", asmfile);
		fflush(stderr);
		exit(1);
	}

	uint8_t ROM[MEM_BYTES - PROGRAM_START_ADDRESS];
	int numread = fread(ROM, 1, MEM_BYTES - PROGRAM_START_ADDRESS, binfp);
	printf("\nBytes read from ROM file: %d\n", numread);
	fflush(stdout);

	char assembly[MAX_ASSEMBLY_LEN];
	for (int i = 0; i < numread; i += 2)
	{
		uint16_t opcode = ((((uint16_t) ROM[i]) << 8) | ROM[i+1]);
		assemble(opcode, assembly);
		fprintf(asmfp, "%4d:   0x%03X:  0x%04X      %s\n", i / 2, PROGRAM_START_ADDRESS + i, opcode, assembly);
	}

	fclose(binfp);
	fclose(asmfp);
}

// array of function pointer where the functions correspond to the
// instruction to be executed given the high order opcode nibble
static void(*assembly_table[16])(const uint16_t opcode, char * const assembly) =
{
	i_0xxx,		// 0
	i_1nnn, 	// 1
	i_2nnn, 	// 2
	i_3xkk, 	// 3
	i_4xkk,		// 4
	i_5xy0, 	// 5
	i_6xkk, 	// 6
	i_7xkk, 	// 7 
	i_8xxx, 	// 8
	i_9xy0, 	// 9
	i_Annn, 	// A
	i_Bnnn, 	// B
	i_Cxkk, 	// C
	i_Dxyn, 	// D
	i_Exxx, 	// E
	i_Fxxx 		// F
};

// handles instructions with high-order nibble 8
void i_8xxx(const uint16_t opcode, char * const assembly)
{
	switch (opcode & 0x000F)
	{
	case 0:
		i_8xy0(opcode, assembly);
		break;
	case 1:
		i_8xy1(opcode, assembly);
		break;
	case 2:
		i_8xy1(opcode, assembly);
		break;
	case 3:
		i_8xy3(opcode, assembly);
		break;
	case 4:
		i_8xy4(opcode, assembly);
		break;
	case 5:
		i_8xy5(opcode, assembly);
		break;
	case 6:
		i_8xy6(opcode, assembly);
		break;
	case 7:
		i_8xy7(opcode, assembly);
		break;
	case 0xE:
		i_8xyE(opcode, assembly);
		break;
	}
}

// handles instructions with high-order nibble 0
void i_0xxx(const uint16_t opcode, char * const assembly)
{
	if (opcode == 0x00E0)
	{
		i_00E0(opcode, assembly);
	}
	else if (opcode == 0x00EE)
	{
		i_00EE(opcode, assembly);
	}
	else
	{
		i_0nnn(opcode, assembly);
	}
}

// handles instructions with high-order nibble E
void i_Exxx(const uint16_t opcode, char * const assembly)
{
	if ((opcode & 0xF0FF) == 0xE09E)
	{
		i_Ex9E(opcode, assembly);
	}
	else
	{
		i_ExA1(opcode, assembly);
	}
}

// handles instructions with high-order nibble F
void i_Fxxx(const uint16_t opcode, char * const assembly)
{
	switch (opcode & 0x00FF)
	{
	case 0x07:
		i_Fx07(opcode, assembly);
		break;
	case 0x0A:
		i_Fx0A(opcode, assembly);
		break;
	case 0x15:
		i_Fx15(opcode, assembly);
		break;
	case 0x18:
		i_Fx18(opcode, assembly);
		break;
	case 0x1E:
		i_Fx1E(opcode, assembly);
		break;
	case 0x29:
		i_Fx29(opcode, assembly);
		break;
	case 0x33:
		i_Fx33(opcode, assembly);
		break;
	case 0x55:
		i_Fx55(opcode, assembly);
		break;
	case 0x65:
		i_Fx65(opcode, assembly);
		break;
	}
}

#ifdef _MSC_VER
#define writeString sprintf_s
#else
#define writeString snprintf
#endif

static uint16_t op_nnn(const uint16_t opcode)
{
	return opcode & 0x0FFF;
}

static uint8_t op_x(const uint16_t opcode)
{
	return (opcode & 0x0F00) >> 8;
}

static uint8_t op_y(const uint16_t opcode)
{
	return (opcode & 0x00F0) >> 4;
}

static uint8_t op_kk(const uint16_t opcode)
{
	return opcode & 0x00FF;
}

void i_0nnn(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s %0X", "SYS", op_nnn(opcode));
}

void i_00E0(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s", "CLS");
}

void i_00EE(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s", "RET");
}

void i_1nnn(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s 0x%03X", "JP", op_nnn(opcode));
}

void i_2nnn(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s 0x%03X", "CALL",  op_nnn(opcode));
}

void i_3xkk(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, 0x%02X", "SE", op_x(opcode), op_kk(opcode));
}

void i_4xkk(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, 0x%02X", "SNE", op_x(opcode), op_kk(opcode));
}

void i_5xy0(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, V%X", "SE", op_x(opcode), op_y(opcode));
}

void i_6xkk(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, 0x%02X", "LD", op_x(opcode), op_kk(opcode));
}

void i_7xkk(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, 0x%02X", "ADD", op_x(opcode), op_kk(opcode));
}

void i_8xy0(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, V%X", "LD", op_x(opcode), op_y(opcode));
}

void i_8xy1(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, V%X", "OR", op_x(opcode), op_y(opcode));
}

void i_8xy2(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, V%X", "AND", op_x(opcode), op_y(opcode));
}

void i_8xy3(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, V%X", "XOR", op_x(opcode), op_y(opcode));
}

void i_8xy4(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, V%X", "ADD", op_x(opcode), op_y(opcode));
}

void i_8xy5(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, V%X", "SUB", op_x(opcode), op_y(opcode));
}

void i_8xy6(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X", "SHR", op_x(opcode));
}

void i_8xy7(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, V%X", "SUBN", op_x(opcode), op_y(opcode));
}

void i_8xyE(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X", "AND", op_x(opcode));
}

void i_9xy0(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, V%X", "SNE", op_x(opcode), op_y(opcode));
}

void i_Annn(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s I, 0x%03X", "LD", op_nnn(opcode));
}

void i_Bnnn(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, 0x%03X", "JP", op_x(opcode), op_nnn(opcode));
}

void i_Cxkk(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, 0x%02X", "RND", op_x(opcode), op_kk(opcode));
}

void i_Dxyn(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, V%X, %X", "DRW", op_x(opcode), op_y(opcode), opcode & 0x000F);
}

void i_Ex9E(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X", "SKP", op_x(opcode));
}

void i_ExA1(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X", "SKNP", op_x(opcode));
}

void i_Fx07(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, DT", "LD",  op_x(opcode));
}

void i_Fx0A(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, K", "LD", op_x(opcode));
}

void i_Fx15(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s DT, V%X", "SKNP", op_x(opcode));
}

void i_Fx18(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s ST, V%X", "LD", op_x(opcode));
}

void i_Fx1E(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s I, V%X", "ADD", op_x(opcode));
}

void i_Fx29(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s F, V%X", "LD", op_x(opcode));
}

void i_Fx33(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s B, V%X", "LD", op_x(opcode));
}

void i_Fx55(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s [I], V%X", "LD", op_x(opcode));
}

void i_Fx65(const uint16_t opcode, char * const assembly)
{
	writeString(assembly, MAX_ASSEMBLY_LEN, "%-5s V%X, [I]", "LD", op_x(opcode));
}