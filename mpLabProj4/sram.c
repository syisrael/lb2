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
extern unsigned short SRAMWriteValue, SRAMReadValue;
extern byte *SRAMAddressPtrs;
extern byte SRAMSelector;
byte index;

void writeSRAM();
void readSRAM();

void writeSRAM() {
    byte tempAddress;
    if (sSRAMWrite) {
        // Select address block for type of measurement
        tempAddress = SRAMAddressPtrs[SRAMSelector] + (SRAMSelector * SRAM_STORED_VALUES);
        
        // Set address pins from stored value
        PIN_A0 = tempAddress & 1;
        PIN_A1 = tempAddress >> 1 & 1;
        PIN_A2 = tempAddress >> 2 & 1;
        PIN_A3 = tempAddress >> 3 & 1;
        
        // Set data pins on SRAM from a stored value in PIC memory
        PIN_D0 = SRAMWriteValue & 1;
        PIN_D1 = SRAMWriteValue >> 1 & 1;
        PIN_D2 = SRAMWriteValue >> 2 & 1;
        PIN_D3 = SRAMWriteValue >> 3 & 1;
        PIN_D4 = SRAMWriteValue >> 4 & 1;
        PIN_D5 = SRAMWriteValue >> 5 & 1;
        PIN_D6 = SRAMWriteValue >> 6 & 1;
        PIN_D7 = SRAMWriteValue >> 7 & 1;
        
        // Complete write cycle on hardware SRAM
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
        
        // Increment SRAM pointer for measurement block
        SRAMAddressPtrs[SRAMSelector] = (SRAMAddressPtrs[SRAMSelector] + 1) % 16 + (SRAMSelector * SRAM_STORED_VALUES);
        
        // Reset control flag
        sSRAMWrite = false;
    }
}

void readSRAM() {
    byte tempAddress;
    if (sSRAMRead) {
        // Reads previous value
        tempAddress = (SRAMAddressPtrs[SRAMSelector] + SRAM_STORED_VALUES - 1) % SRAM_STORED_VALUES + (SRAMSelector * SRAM_STORED_VALUES);
        
        // Set address pins from stored value
        PIN_A0 = tempAddress & 1;
        PIN_A1 = tempAddress >> 1 & 1;
        PIN_A2 = tempAddress >> 2 & 1;
        PIN_A3 = tempAddress >> 3 & 1;

        
        // Read data pins from SRAM and store value in PIC memory
        // sleep
        SRAMReadValue = PIN_D7;
        SRAMReadValue <<= 1;
        SRAMReadValue |= PIN_D6;
        SRAMReadValue <<= 1;
        SRAMReadValue |= PIN_D5;
        SRAMReadValue <<= 1;
        SRAMReadValue |= PIN_D4;
        SRAMReadValue <<= 1;
        SRAMReadValue |= PIN_D3;
        SRAMReadValue <<= 1;
        SRAMReadValue |= PIN_D2;
        SRAMReadValue <<= 1;
        SRAMReadValue |= PIN_D1;
        SRAMReadValue <<= 1;
        SRAMReadValue |= PIN_D0;
        
        sSRAMRead = false;
    }
}