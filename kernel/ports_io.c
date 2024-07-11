// write byte to a port
void port_byte_out(unsigned short port, unsigned char data) {
    asm volatile("outb %0, %1" : : "a"(data), "Nd"(port)); // put data in al, port in dx
}

// read byte from a port
unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    asm volatile("inb %1, %0"
                 : "=a"(result)     // result in al
                 : "Nd"(port));     // port in dx
    return result;
}