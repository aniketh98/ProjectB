//Authors: Aniketh & Ahad Siddiqui 
void printString(char*);
void readString(char*);
void readSector(char*,int);
void handleInterrupt21(int, int, int, int);

void main() {
    char line [80];
    char buffer[512];

    makeInterrupt21();
    interrupt(0x21,1,line,0,0);
    printString("\nYour String is: ");
    interrupt(0x21,0,line,0,0);
    interrupt(0x21,2,buffer,30,0);
    interrupt(0x21,0,buffer,0,0);
    while(1);
}

void printString(char* str) {
    while (*str != 0x0) {
        char al = *str;
        char ah = 0x0E;
        int ax = ah * 256 + al;

        interrupt(0x10, ax, 0, 0, 0);

        str++;
    }
}
void readString(char* str) {
    int index = 0;
    while (1) {
        char key = interrupt(0x16, 0, 0, 0, 0);
        if (key == 0xd) {
            interrupt(0x10, 0xE * 256 + 0xa, 0, 0, 0);
            interrupt(0x10, 0xE * 256, 0, 0, 0);
            str[index] = 0xa;
            str[index + 1] = 0x0;
            return;
        } else if (key == 0x8) {
            if (index > 0) {
                interrupt(0x10, 0xE * 256 + 0x8, 0, 0, 0);
                interrupt(0x10, 0xE * 256 + ' ', 0, 0, 0);
                interrupt(0x10, 0xE * 256 + 0x8, 0, 0, 0);
                index--;
            }
        } else {
            interrupt(0x10, 0xE * 256 + key, 0, 0, 0);
            str[index] = key;
            index++;
        }
    }
}   

void readSector(char* chars, int sector) {
    int AH = 2;
    int AL = 1;
    int AX = AH*256+AL;
    int BX = chars;
    int CH = 0;
    int CL = sector+1;
    int CX = CH*256+CL;
    int DH = 0;
    int DL = 0x80;
    int DX = DH*256+DL;

    interrupt(0x13, AX, BX, CX, DX);
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
    if( ax == 0) {
        printString(bx);
    } else if(ax == 1) {
        readString(bx);
    } else if(ax == 2) {
        readSector(bx, cx);
    } else {
        printString("AX must be one of 1,2,3");
    }
} 
