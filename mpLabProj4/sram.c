#define     PIN_WE      <change me>
#define     PIN_OE      <change me>
#define     PIN_HZ      <change me>

#define     PIN_A0      <change me>
#define     PIN_A1      <change me>
#define     PIN_A2      <change me>
#define     PIN_A3      <change me>

#define     PIN_D0      <change me>
#define     PIN_D1      <change me>
#define     PIN_D2      <change me>
#define     PIN_D3      <change me>
#define     PIN_D4      <change me>
#define     PIN_D5      <change me>
#define     PIN_D6      <change me>
#define     PIN_D7      <change me>

extern bool sSRAMWrite, sSRAMRead;
extern byte SRAMAddressValue;
extern short SRAMWriteValue, SRAMReadValue;

void writeSRAM();
void readSRAM();

void writeSRAM() {
    if (sSRAMWrite) {
        PIN_A0 = SRAMAddressValue & 1;
        PIN_A1 = SRAMAddressValue >> 1 & 1;
        PIN_A2 = SRAMAddressValue >> 2 & 1;
        PIN_A3 = SRAMAddressValue >> 3 & 1;
        
        PIN_D0 = SRAMWriteValue & 1;
        PIN_D1 = SRAMWriteValue >> 1 & 1;
        PIN_D2 = SRAMWriteValue >> 2 & 1;
        PIN_D3 = SRAMWriteValue >> 3 & 1;
        PIN_D4 = SRAMWriteValue >> 4 & 1;
        PIN_D5 = SRAMWriteValue >> 5 & 1;
        PIN_D6 = SRAMWriteValue >> 6 & 1;
        PIN_D7 = SRAMWriteValue >> 7 & 1;
        
        // sleep
        PIN_OE = 1;
        PIN_WE = 0;
        PIN_HZ = 0;
        // sleep
        PIN_OE = 1;
        PIN_WE = 1;
        PIN_HZ = 0;
        // sleep
        PIN_OE = 1;
        PIN_WE = 1;
        PIN_HZ = 1;
        
        sSRAMWrite = false;
    }
}

void readSRAM() {
    if (sSRAMRead) {
        short buffer;
        
        PIN_A0 = SRAMAddressValue & 1;
        PIN_A1 = SRAMAddressValue >> 1 & 1;
        PIN_A2 = SRAMAddressValue >> 2 & 1;
        PIN_A3 = SRAMAddressValue >> 3 & 1;

        // sleep
        
        SRAMReadValue = 0;
        buffer = PIN_D0;
        SRAMReadValue |= buffer;
        buffer = PIN_D1;
        SRAMReadValue |= buffer << 1;
        buffer = PIN_D2;
        SRAMReadValue |= buffer << 2;
        buffer = PIN_D3;
        SRAMReadValue |= buffer << 3;
        buffer = PIN_D4;
        SRAMReadValue |= buffer << 4;
        buffer = PIN_D5;
        SRAMReadValue |= buffer << 5;
        buffer = PIN_D6;
        SRAMReadValue |= buffer << 6;
        buffer = PIN_D7;
        SRAMReadValue |= buffer << 7;
        
        sSRAMRead = false;
    }
}