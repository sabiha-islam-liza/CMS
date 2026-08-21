#ifndef MD5_H
#define MD5_H

#include <stdint.h>

typedef struct {
    uint32_t state[4];
    uint32_t count[2];
    unsigned char buffer[64];
} MD5_CTX_INLINE;

//Liza (252-35-485)
// Core MD5 Functions
void MD5Init_Inline(MD5_CTX_INLINE *context);
void MD5Update_Inline(MD5_CTX_INLINE *context, const unsigned char *input, unsigned int inputLen);
void MD5Final_Inline(unsigned char digest[16], MD5_CTX_INLINE *context);

// Helper function to convert input string to 32-character hex hash string
void get_user_md5(const char *input, char *output_hash);

#endif // MD5_H

