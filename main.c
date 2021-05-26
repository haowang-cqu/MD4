#include <stdio.h>
#include "md4.h"


int main() {
    char m1[] = "The quick brown fox jumps over the lazy dog";
    char m2[] = "The quick brown fox jumps over the lazy cog";
    unsigned char hash1[MD4_DIGEST_SIZE];
    unsigned char hash2[MD4_DIGEST_SIZE];
    md4((unsigned char *) m1, strlen(m1), hash1);
    md4((unsigned char *) m2, strlen(m2), hash2);
    printf("MD4(\"%s\")\n", m1);
    for (int i = 0; i < MD4_DIGEST_SIZE; i++) {
        printf("%02x", hash1[i]);
    }
    printf("\n");
    printf("MD4(\"%s\")\n", m2);
    for (int i = 0; i < MD4_DIGEST_SIZE; i++) {
        printf("%02x", hash2[i]);
    }
    printf("\n");
    return 0;
}