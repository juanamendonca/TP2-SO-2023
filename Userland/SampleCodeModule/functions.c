#include <stdint.h>
#include <functions.h>
#include <user_syscalls.h>
#include <stdarg.h>

int strcmp(char * x, char * y){
    int i=0;
    while(x[i] && y[i]){
        if(x[i]>y[i]){
            return 1;
        }
        else if(x[i]<y[i]){
            return -1;
        }
        i++;
    }
    return x[i]-y[i];
}

void strcpy(char * dest, const char * src){
    int i=0,j=0;
    while (src[j] != 0){
        dest[i++] = src[j++];
    }
    dest[i] = 0;
}


void scanf(char * buffer, int bufferSize){
    char letter[2];
	letter[1]='\0';
    int i=0;
	while(letter[0] != '\n' && i<bufferSize-1){
		sys_read(letter);
		if ((letter[0] >= '0' && letter[0]<= '9') || (letter[0] >= 'A' && letter[0]<= 'Z') || letter[0]==' '){
			sys_write(letter, WHITE);
			buffer[i++]=letter[0];
		}
        if(letter[0]=='\b' && i>0){
            sys_write(letter, WHITE);
            i-=1;
        }
	}
	buffer[i]='\0';
    enter();
}

char getKey(){
    char key[2];
	key[1]='\0';
    sys_read(key);
    return key[0];
}

char getChar(){
    char key[2];
	key[1]='\0';
    sys_read(key);
    while(key[0]==-1){
        sys_read(key);
    }
    return key[0];
}

void deleteExtraSpaces(char * string){
    deleteInitialFinalSpace(string);
    int j=0;
    for(int i=0; string[i]!='\0'; i++){
        if(string[i]==' '){
            while(string[i+j]==' '){
                j++;
            }
            if(j>1){
                int k=1;
                for(; string[j]!='\0'; j++, k++){
                    string[i+k]=string[i+j];
                }
                string[i+k]='\0';
            }
            j=0;
        }
    }
}

void deleteInitialFinalSpace(char * string){
    int i=0;
    while(string[i]==' '){
        i++;
    }
    int j=0;
    if(i!=0){
        while(string[i-1]!='\0'){
            string[j]=string[i];
            i++;
            j++;
        }
    }

    int length;
    for(length=0; string[length]!='\0'; length++){}
    while(string[length-1]==' '){
        string[length-1]=string[length];
        length--;
    }
}

 void intToString(int number, char *str)
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




//printing
void print(char * string){
	sys_write(string, WHITE);

}


//strings
int strlen(char * string){
    int i = 0;
    while(string[i++] != 0);
    return i-1;
}

void printBase(uint64_t value, uint32_t base){
    char buf[20];
    uintToBase(value, buf, base);
    print(buf);
}

void printHexa(uint64_t value){
    char buf[20];
    int digits = uintToBase(value, buf, 16);
    fillHexa(16-digits, buf);
    print(buf);
}

//from naiveConsole
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

void fillHexa(int num, char* buf){ //to fill with 0 the hexa
    for(int i = 15; i >= 0; i--){
        if(i >= num){
            buf[i] = buf[i-num];
        }else{
            buf[i] = '0';
        }
    }
    buf[16] = '\0';
}


void enter(){
    sys_new_line();
}

int atoi2(char *str)
{
	// Initialize result
	int res = 0;

	// Iterate through all characters
	// of input string and update result
	// take ASCII character of corosponding digit and
	// subtract the code from '0' to get numerical
	// value and multiply res by 10 to shuffle
	// digits left to update running total
	int flag = (str[0] == '-' ? -1 : 1);
	if (flag == -1)
	{
		str++;
	}
	for (int i = 0; str[i] != '\0'; ++i)
		res = res * 10 + str[i] - '0';

	// return result.
	return res * flag;
}

void printInt(int num)
{
    char buffer[40];
    uintToBase(num, buffer, 10);
    print(buffer);
}