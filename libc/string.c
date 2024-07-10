int strlen(const char* str)
{
    int len = 0;
    while(str[len])
        len++;
    return len;
}

void strreverse(char* str)
{
    int len = strlen(str);
    for(int i = 0; i < len / 2; i++){
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}