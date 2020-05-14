ENTRY(reset_handler)

MEMORY
{
    FLASH(rx)   : ORIGIN = 0x08000000,LENGTH = 128k
    SRAM(rwx)   : ORIGIN = 0x20000000,LENGTH = 40k 
}
SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))
        *(.text)
        *(.text.*)
        *(.rodata .rodata.*)
        _endtext = .;
    }>FLASH
    .data : AT(_endtext)
    {
        _startdata = .;
        *(.data)
        *(.data.*)
        _enddata = .;
    }>SRAM
    .bss :
    {
        _startbss = .;
        *(.bss)
        _endbss = .;
    }>SRAM
    _estack = ORIGIN(SRAM)+LENGTH(SRAM);
}