#include <naiveConsole.h>
#include <video.h>

#define BLACK 0x000000

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25 ;

void ncPrint(const char * string,int colorNum, int backColor)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		// ncPrintChar(string[i],colorNum,backColor);
		putLetterNext(string[i], BLACK);
}

void ncPrintChar(char character,int colorNum, int backColor)
{
	*currentVideo = character;
	currentVideo ++;
	*currentVideo = backColor*16 +colorNum;
	currentVideo ++;
}

void ncNewline()
{
	do
	{
		//ncPrintChar(' ',BLACK,BLACK);
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
}

void ncClear()
{
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}



