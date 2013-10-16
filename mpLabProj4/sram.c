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

extern byte *SRAMAddressPtrs;

void writeSRAM(byte selector, short data) {
    byte tempAddress;
    // Select address block for type of measurement
    tempAddress = SRAMAddressPtrs[selector] + (selector * SRAM_STORED_VALUES);
    
    // Set address pins from stored value
    PIN_A0 = tempAddress & 1;
    PIN_A1 = tempAddress >> 1 & 1;
    PIN_A2 = tempAddress >> 2 & 1;
    PIN_A3 = tempAddress >> 3 & 1;
    
    // Set data pins on SRAM from a stored value in PIC memory
    PIN_D0 = data & 1;
    PIN_D1 = data >> 1 & 1;
    PIN_D2 = data >> 2 & 1;
    PIN_D3 = data >> 3 & 1;
    PIN_D4 = data >> 4 & 1;
    PIN_D5 = data >> 5 & 1;
    PIN_D6 = data >> 6 & 1;
    PIN_D7 = data >> 7 & 1;
    
    // Complete write cycle on hardware SRAM
    delay();
    PIN_OE = 1;
    PIN_WE = 0;
    PIN_HZ = 0;
    delay();
    PIN_OE = 1;
    PIN_WE = 1;
    PIN_HZ = 0;
    delay();
    PIN_OE = 1;
    PIN_WE = 1;
    PIN_HZ = 1;
    
    // Increment SRAM pointer for measurement block
    SRAMAddressPtrs[selector] = (SRAMAddressPtrs[selector] + 1) % 16 + (selector * SRAM_STORED_VALUES);
}

short readSRAM(byte selector, byte history) {
    short data;
    
    // Reads previous value
    history = (SRAMAddressPtrs[selector] + SRAM_STORED_VALUES - history) % SRAM_STORED_VALUES + (selector * SRAM_STORED_VALUES);
    
    // Set address pins from stored value
    PIN_A0 = history & 1;
    PIN_A1 = history >> 1 & 1;
    PIN_A2 = history >> 2 & 1;
    PIN_A3 = history >> 3 & 1;
    
    // Read data pins from SRAM and store value in PIC memory
    delay();
    data = PIN_D7;
    data <<= 1;
    data |= PIN_D6;
    data <<= 1;
    data |= PIN_D5;
    data <<= 1;
    data |= PIN_D4;
    data <<= 1;
    data |= PIN_D3;
    data <<= 1;
    data |= PIN_D2;
    data <<= 1;
    data |= PIN_D1;
    data <<= 1;
    data |= PIN_D0;
    
    return data;
}