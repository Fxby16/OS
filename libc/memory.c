void* memcpy(void* dest, const void* src, int count)
{
    for(int i = 0; i < count; i++){
        *((char*) dest + i) = *((const char*) src + i);
    }
}

void* memset(void* dest, char val, int count)
{
    for(int i = 0; i < count; i++){
        *((char*) dest + i) = val;
    }
}