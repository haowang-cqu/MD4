#include "md4.h"

static void md4_transform(u32 *hash, u32 const *in) {
    u32 a, b, c, d;
    a = hash[0];
    b = hash[1];
    c = hash[2];
    d = hash[3];
    // Round 1
    ROUND1(a, b, c, d, in[0], 3);
    ROUND1(d, a, b, c, in[1], 7);
    ROUND1(c, d, a, b, in[2], 11);
    ROUND1(b, c, d, a, in[3], 19);
    ROUND1(a, b, c, d, in[4], 3);
    ROUND1(d, a, b, c, in[5], 7);
    ROUND1(c, d, a, b, in[6], 11);
    ROUND1(b, c, d, a, in[7], 19);
    ROUND1(a, b, c, d, in[8], 3);
    ROUND1(d, a, b, c, in[9], 7);
    ROUND1(c, d, a, b, in[10], 11);
    ROUND1(b, c, d, a, in[11], 19);
    ROUND1(a, b, c, d, in[12], 3);
    ROUND1(d, a, b, c, in[13], 7);
    ROUND1(c, d, a, b, in[14], 11);
    ROUND1(b, c, d, a, in[15], 19);
    // Round 2
    ROUND2(a, b, c, d, in[0], 3);
    ROUND2(d, a, b, c, in[4], 5);
    ROUND2(c, d, a, b, in[8], 9);
    ROUND2(b, c, d, a, in[12], 13);
    ROUND2(a, b, c, d, in[1], 3);
    ROUND2(d, a, b, c, in[5], 5);
    ROUND2(c, d, a, b, in[9], 9);
    ROUND2(b, c, d, a, in[13], 13);
    ROUND2(a, b, c, d, in[2], 3);
    ROUND2(d, a, b, c, in[6], 5);
    ROUND2(c, d, a, b, in[10], 9);
    ROUND2(b, c, d, a, in[14], 13);
    ROUND2(a, b, c, d, in[3], 3);
    ROUND2(d, a, b, c, in[7], 5);
    ROUND2(c, d, a, b, in[11], 9);
    ROUND2(b, c, d, a, in[15], 13);
    // Round 3
    ROUND3(a, b, c, d, in[0], 3);
    ROUND3(d, a, b, c, in[8], 9);
    ROUND3(c, d, a, b, in[4], 11);
    ROUND3(b, c, d, a, in[12], 15);
    ROUND3(a, b, c, d, in[2], 3);
    ROUND3(d, a, b, c, in[10], 9);
    ROUND3(c, d, a, b, in[6], 11);
    ROUND3(b, c, d, a, in[14], 15);
    ROUND3(a, b, c, d, in[1], 3);
    ROUND3(d, a, b, c, in[9], 9);
    ROUND3(c, d, a, b, in[5], 11);
    ROUND3(b, c, d, a, in[13], 15);
    ROUND3(a, b, c, d, in[3], 3);
    ROUND3(d, a, b, c, in[11], 9);
    ROUND3(c, d, a, b, in[7], 11);
    ROUND3(b, c, d, a, in[15], 15);
    hash[0] += a;
    hash[1] += b;
    hash[2] += c;
    hash[3] += d;
}

/**
 * @brief 初始化md4_ctx，赋予状态寄存器初值
 */
void md4_init(struct md4_ctx *ctx) {
    ctx->hash[0] = 0x67452301;
    ctx->hash[1] = 0xefcdab89;
    ctx->hash[2] = 0x98badcfe;
    ctx->hash[3] = 0x10325476;
    ctx->byte_count = 0;
}

/**
 * @brief 更新MD4状态寄存器
 * @param ctx md4_ctx
 * @param data 计算hash值的部分数据
 * @param len data的字节数
 */
void md4_update(struct md4_ctx *ctx, const void *data, unsigned long len) {
    unsigned char * byte_data = (unsigned char *)data;
    unsigned char * byte_block = (unsigned char *)ctx->block;
    // block中空余的字节数
    const u32 avail = sizeof(ctx->block) - (ctx->byte_count & 0x3f);
    ctx->byte_count += len;
    // data不足以装满一个block则存入后直接返回
    if (avail > len) {
        memcpy(byte_block + (sizeof(ctx->block) - avail), byte_data, len);
        return;
    }
    // 装满一个block后更新状态寄存器
    memcpy(byte_block + (sizeof(ctx->block) - avail), byte_data, avail);
    // TODO
    md4_transform(ctx->hash, ctx->block);
    byte_data += avail;
    len -= avail;
    while (len >= sizeof(ctx->block)) {
        memcpy(ctx->block, byte_data, sizeof(ctx->block));
        // TODO
        md4_transform(ctx->hash, ctx->block);
        byte_data += sizeof(ctx->block);
        len -= sizeof(ctx->block);
    }
    // 剩余的不足一个block的部分暂存在block等下次更新
    memcpy(ctx->block, byte_data, len);
}

/**
 * @brief 完成MD4 hash值的计算
 * @param ctx md4_ctx
 * @param hash 最终得到的MD4
 */
void md4_final(struct md4_ctx *ctx, unsigned char *hash) {
    // block中剩余的字节数
    const unsigned int offset = ctx->byte_count & 0x3f;
    char *p = (char *) ctx->block + offset;
    int padding = 56 - ((int)offset + 1);
    *p++ = (char)0x80;
    if (padding < 0) {
        memset(p, 0, padding + sizeof(u64));
        // TODO
        md4_transform(ctx->hash, ctx->block);
        p = (char *) ctx->block;
        padding = 56;
    }
    memset(p, 0, padding);
    ctx->block[14] = ctx->byte_count << 3;
    ctx->block[15] = ctx->byte_count >> 29;
    md4_transform(ctx->hash, ctx->block);
    memcpy(hash, ctx->hash, sizeof(ctx->hash));
    memset(ctx, 0, sizeof(struct md4_ctx));
}

/**
 * @brief 计算给定byte数组的MD4 hash值
 * @param data byte数组
 * @param len byte数组的长度
 * @param hash MD4
 */
void md4(const unsigned char *data, unsigned long len, unsigned char *hash) {
    struct md4_ctx ctx = {0};
    md4_init(&ctx);
    md4_update(&ctx, data, len);
    md4_final(&ctx, hash);
}
