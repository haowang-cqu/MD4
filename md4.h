#ifndef HASH_MD4_H
#define HASH_MD4_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef uint32_t u32;
typedef uint64_t u64;

#define MD4_DIGEST_SIZE        16
#define MD4_BLOCK_WORDS        16
#define MD4_HASH_WORDS         4

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

#define F(x, y, z) (((x) & (y)) | ((~(x)) & (z)))
#define G(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))

#define ROUND1(a, b, c, d, k, s) ((a) = rol((a) + F((b),(c),(d)) + (k), (s)))
#define ROUND2(a, b, c, d, k, s) ((a) = rol((a) + G((b),(c),(d)) + (k) + (u32)0x5A827999, (s)))
#define ROUND3(a, b, c, d, k, s) ((a) = rol((a) + H((b),(c),(d)) + (k) + (u32)0x6ED9EBA1, (s)))

struct md4_ctx {
    u32 hash[MD4_HASH_WORDS];
    u32 block[MD4_BLOCK_WORDS];
    u64 byte_count;
};

void md4_init(struct md4_ctx *ctx);

void md4_update(struct md4_ctx *ctx, const void *data, unsigned long len);

void md4_final(struct md4_ctx *ctx, unsigned char *hash);

void md4(const unsigned char *data, unsigned long len, unsigned char *hash);

#endif //HASH_MD4_H