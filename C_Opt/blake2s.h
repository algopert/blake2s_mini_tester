

#ifndef BLAKE2_H
#define BLAKE2_H

#define S

#define R(v, n) (((v) >> (n)) | ((v) << (WORDLEN - (n))))
#define X(a, b) (t) = (a), (a) = (b), (b) = (t)
#define F(n) for (i = 0; i < n; i++)

typedef unsigned char B;
typedef unsigned long long Q;

// parameters for BLAKE2s

#define VERSION "s"
#define WORDLEN 32
#define BLOCKLEN 64
#define ROUNDS 80
#define ROTATION 0x07080C10
#define OUTLEN 32
#define KEYLEN 32
typedef unsigned int W;

typedef struct _blake2_ctx
{
  W s[16], idx, outlen;
  union
  {
    B b[BLOCKLEN];
    W w[BLOCKLEN / (WORDLEN / 8)];
  } x;
  Q len;
} blake2_ctx;

#ifdef __cplusplus
extern "C"
{
#endif

  int blake2s(void *out, Q outlen, const void *key, Q keylen, const void *in, Q inlen);
  int blake2s_init(blake2_ctx *, W, const void *, W);
  void blake2s_update(blake2_ctx *, const void *, W);
  void blake2s_final(void *, blake2_ctx *);

#ifdef __cplusplus
}
#endif

#endif
