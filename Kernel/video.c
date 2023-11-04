#include <video.h>
#include <font.h>

#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

static int pointer_x = 3;
static int pointer_y = 3;
struct vbe_mode_info_structure
{
	uint16_t attributes;  // deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;	  // deprecated
	uint8_t window_b;	  // deprecated
	uint16_t granularity; // deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr; // deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;		   // number of bytes per horizontal line
	uint16_t width;		   // width in pixels
	uint16_t height;	   // height in pixels
	uint8_t w_char;		   // unused...
	uint8_t y_char;		   // ...
	uint8_t planes;
	uint8_t bpp;   // bits per pixel in this mode
	uint8_t banks; // deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size; // deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
	uint32_t framebuffer; // physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size; // size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__((packed));
typedef struct vbe_mode_info_structure *VBEInfoPtr;
VBEInfoPtr VBE_mode_info = (VBEInfoPtr)0x0000000000005C00;

int getWidth(){
	return VBE_mode_info->width;
}

void putPixel(uint32_t hexColor, uint32_t x, uint32_t y)
{
	uint8_t *screen = (uint8_t *)((uint64_t)(VBE_mode_info->framebuffer));
	uint32_t offset = VBE_mode_info->pitch * y + x * 3;
	screen[offset] = hexColor & 0xFF;
	screen[offset + 1] = (hexColor >> 8) & 0xFF;
	screen[offset + 2] = (hexColor >> 16) & 0xFF;
}

void copyPixel(uint32_t new_x, uint32_t new_y, uint32_t old_x, uint32_t old_y){
	uint8_t *screen = (uint8_t *)((uint64_t)(VBE_mode_info->framebuffer));
	uint32_t offset_old = VBE_mode_info->pitch * old_y + old_x * 3;
	uint32_t offset_new = VBE_mode_info->pitch * new_y + new_x * 3;
	screen[offset_new] = screen[offset_old];
	screen[offset_new + 1] = screen[offset_old + 1];
	screen[offset_new + 2] = screen[offset_old + 2];
}



void putLetter(int caracter, uint32_t x, uint32_t y, int color)
{
	unsigned char *bitMap = charMap(caracter);
	for (int j = 0; j < CHAR_HEIGHT; j++)
	{
		for (int i = 0; i < CHAR_WIDTH; i++)
		{
			uint8_t bit = ((bitMap[j]) >> i) & 0x01;
			if (bit == 1)
			{
				//Call putPixel when bit is 1
				putPixel(color, x + 7 - i, j + y); //For instance, put red pixel in row 0
			}
		}
	}
}
void putArray(char *array, uint32_t x, uint32_t y, int color)
{
	int i = 0;
	int aux_y = y;
	int aux_x = x;
	while (array[i] != '\0')
	{
		if (aux_x + CHAR_HEIGHT > VBE_mode_info->width)
		{
			aux_x = 0;
			aux_y = aux_y + CHAR_HEIGHT;
		}
		putLetter(array[i], aux_x, aux_y, color);
		i++;
		aux_x = aux_x + CHAR_WIDTH;
	}
}

void moveUpScreen()
{
    //Iterate from second line to last
    for (int y = 0; y < VBE_mode_info->height - CHAR_HEIGHT; y++)
    {
        //Copy each pixel in the following line
        for (int x = 0; x < VBE_mode_info->width ; x++)
        {
            copyPixel(x, y , x, y + CHAR_HEIGHT);
        }
    }

	//Iterate from second line to last
    for (int y = VBE_mode_info->height - CHAR_HEIGHT; y < VBE_mode_info->height; y++)
    {
        //Copy each pixel from following line to the current one
        for (int x = 0; x < VBE_mode_info->width ; x++)
        {
            putPixel(BLACK, x, y);
        }
    }

	pointer_y=pointer_y-CHAR_HEIGHT;
	pointer_x=3;
}

void clearScreen(){
	for (int y = 0; y < VBE_mode_info->height; y++)
    {
        //Copy each pixel from following line to current line
        for (int x = 0; x < VBE_mode_info->width ; x++)
        {
            putPixel(BLACK, x, y);
        }
    }
	pointer_x=3;
	pointer_y=3;
}


void intToString(int number, char str[])
{
	if (number == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return;
	}
	int i = 0;
	while (number > 0)
	{
		int digit = number % 10;
		str[i++] = digit + '0';
		number /= 10;
	}
	str[i] = '\0';
	// Reverse the string
	int start = 0;
	int end = i - 1;
	while (start < end)
	{
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}
void putDec(int number, uint32_t x, uint32_t y, int color)
{
	char strNumber[20];
	intToString(number, strNumber);
	putArray(strNumber, x, y, color);
}

void putBackspace()
{
	if (pointer_x == 0 && pointer_y == 0)
		return;
	pointer_x = pointer_x - CHAR_WIDTH;
	for (int j = 0; j < CHAR_HEIGHT; j++)
	{
		for (int i = 0; i < CHAR_WIDTH; i++)
		{
			putPixel(0x000000, pointer_x + 7 - i, j + pointer_y);
		}
	}
	if (pointer_x < 0 && pointer_y > 0) {
		pointer_x = VBE_mode_info->width - CHAR_WIDTH;
		pointer_y = (pointer_y >= CHAR_HEIGHT) ? pointer_y - CHAR_HEIGHT : 0;
	}
}

void putLetterNext(int caracter, int color)
{
	if (caracter == '\b')
	{
		putBackspace();
		return;
	}
	if (pointer_x + CHAR_WIDTH > VBE_mode_info->width)
	{
		pointer_x = 3;
		pointer_y = pointer_y + CHAR_HEIGHT;
	}
	if (pointer_y + CHAR_HEIGHT > VBE_mode_info->height)
    {
        moveUpScreen();
    }
	unsigned char *bitMap = charMap(caracter);
	for (int j = 0; j < CHAR_HEIGHT; j++)
	{
		for (int i = 0; i < CHAR_WIDTH; i++)
		{
			uint8_t bit = ((bitMap[j]) >> i) & 0x01;
			if (bit == 1)
			{
				//Call putPixel when bit is 1
				putPixel(color, pointer_x + 7 - i, j + pointer_y);
			}
		}
	}
	pointer_x = pointer_x + CHAR_WIDTH;
}

void putArrayNext(char *array, int color)
{
	int i = 0;
	while (array[i] != '\0')
	{
        if (array[i] != '\n') {
            putLetterNext(array[i], color);
        } else {
            putLine();
        }
		i++;
	}
}
void putDecNext(int number, int color)
{
	char strNumber[20];
	intToString(number, strNumber);
	putArrayNext(strNumber, color);
}
void putLine()
{
	if (pointer_y + CHAR_HEIGHT > VBE_mode_info->height)
    {
        moveUpScreen();
    }
	pointer_y = pointer_y + CHAR_HEIGHT;
	pointer_x = 3;
}

void printError(char * string){
	int i = 0;
	while(string[i] != '\0'){
		putLetterNext(string[i], RED);
		i++;
	}
	putLetterNext('\n', WHITE);
}


// Converts an unsigned integer number to a character string representation in the specified base.
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base){
    char *p = buffer;
    char *p1, *p2;
    uint32_t digits = 0;

    //Calculate characters for each digit
    do{
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    }
    while (value /= base);

    // Terminate string in buffer.
    *p = 0;

    //Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }

    return digits;
}

void fillHexa(int num, char* buf){ 
    for(int i = 15; i >= 0; i--){
        if(i >= num){
            buf[i] = buf[i-num];
        }else{
            buf[i] = '0';
        }
    }
    buf[16] = '\0';
}

void printHexa(uint64_t value){
    char buf[20];
    int digits = uintToBase(value, buf, 16);
    fillHexa(16-digits, buf);
	int i = 0;
    while(buf[i] != 0){
		putLetterNext(buf[i], WHITE);
		i++;
	}
}


