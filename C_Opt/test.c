

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#include "blake2s.h"



uint8_t hexchar2int(char c)
{
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    return c - '0';
}

uint16_t read_bytes(const char *str, uint8_t *target)
{
    uint16_t len = strlen(str) / 2;
    for (size_t i = 0; i < len; i++)
    {
        uint8_t c1 = hexchar2int(str[2 * i]);
        uint8_t c2 = hexchar2int(str[2 * i + 1]);
        target[i] = (c1 << 4) | c2;
    }
    return len;
}

int main(int argc, char **argv)
{
    uint8_t in[1024], md[32], key[32];
    size_t outlen, inlen;
    outlen = 32;

    char str[2000];

    freopen("input.txt", "r", stdin);
    scanf("%s", str);

    printf("input Value =  %s \n\n", str);

    inlen = read_bytes(str, in);

    freopen("output.txt","w", stdout);

    blake2s(md, outlen, NULL, 0, in, inlen);

    printf("Hash Vaule :\n");
    for (int i = 0; i < outlen; i++)
        printf("%02x ", md[i]);

    fclose(stdout);

    return 0;
}
