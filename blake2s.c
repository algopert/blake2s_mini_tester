#include "blake2s.h"

W iv[8] =
    {0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
     0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19};

W v_idx[8] =
    {0xC840, 0xD951, 0xEA62, 0xFB73,
     0xFA50, 0xCB61, 0xD872, 0xE943};

Q sigma[12] =
    {0xfedcba9876543210, 0x357b20c16df984ae,
     0x491763eadf250c8b, 0x8f04a562ebcd1397,
     0xd386cb1efa427509, 0x91ef57d438b0a6c2,
     0xb8293670a4def15c, 0xa2684f05931ce7bd,
     0x5a417d2c803b9ef6, 0x0dc3e9bf5167482a,
     0xfedcba9876543210, 0x357b20c16df984ae};

void blake2s_compress(blake2_ctx *ctx, W last)
{
  W i, j, a, b, c, d, r, t, s[16];
  Q z, *p = sigma;

  F(8)
  s[i] = ctx->s[i],
  s[i + 8] = iv[i];

  z = ctx->len;
  s[12] ^= (z & 0xFFFFFFFF);
  s[13] ^= (z >> 32);
  s[14] ^= -last;

  for (i = 0; i < ROUNDS;)
  {
    z = *p++;
    do
    {
      d = v_idx[i++ % 8];
      a = (d & 15);
      b = (d >> 4 & 15);
      c = (d >> 8 & 15);
      d >>= 12;
      r = ROTATION;
      for (j = 0; j < 4; j++)
      {
        if (!(j & 1))
        {
          s[a] += ctx->x.w[z & 15];
          z >>= 4;
        }
        s[a] += s[b];
        s[d] = R(s[d] ^ s[a], (r & 255));
        X(a, c), X(b, d);
        r >>= 8;
      }
    } while (z != 0);
  }
  F(8)
  ctx->s[i] ^= s[i] ^ s[i + 8];
}

int blake2s_init(blake2_ctx *c, W outlen, const void *key, W keylen)
{
  W i;

  if (outlen == 0 || outlen > OUTLEN || keylen > KEYLEN)
    return -1;

  // set the initial values
  F(8)
  c->s[i] = iv[i];

  c->s[0] ^= 0x01010000 ^ (keylen << 8) ^ outlen;
  c->len = 0;
  c->idx = 0;
  c->outlen = outlen;

  // zero initialize buffer
  for (i = keylen; i < BLOCKLEN; i++)
    c->x.b[i] = 0;

  // add key if provided
  if (keylen > 0)
  {
    blake2s_update(c, key, keylen);
    c->idx = BLOCKLEN;
  }
  return 0;
}

void blake2s_update(blake2_ctx *c, const void *in, W len)
{
  B *p = (B *)in;
  W i;

  F(len)
  {
    // buffer filled?
    if (c->idx == BLOCKLEN)
    {
      // update length
      c->len += BLOCKLEN;
      blake2s_compress(c, 0);
      // reset buffer index
      c->idx = 0;
    }
    // add byte to buffer, increase index
    c->x.b[c->idx] = p[i];
    c->idx++;
  }
}

void blake2s_final(void *out, blake2_ctx *c)
{
  W i;

  c->len += c->idx;
  // zero remainder of buffer
  while (c->idx < BLOCKLEN)
    c->x.b[c->idx++] = 0;
  // permute
  blake2s_compress(c, 1);
  // return hash
  F(c->outlen)
  ((B *)out)[i] = ((B *)c->s)[i];
}

int blake2s(void *out, Q outlen, const void *key, Q keylen, const void *in, Q inlen)
{
  blake2_ctx ctx;

  if (blake2s_init(&ctx, outlen, key, keylen))
    return -1;
  blake2s_update(&ctx, in, inlen);
  blake2s_final(out, &ctx);

  return 0;
}