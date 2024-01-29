#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toy.h"

// Fills a buffer with small random values
static void toy_fill_small_random_values(short *buf, int n) {
    for (int k = 0; k < n; ++k) {
        short val = rand() & 0xF;
        val = (val >> 1 & 1) - (val & 1); // small Binomial distribution using Hamming weight
        if (val < 0)
            val += TK_Q;
        buf[k] = val;
    }
}

// Helper function: polynomial multiplication in Z97[X]/(X^4+1)
static void toy_polynomial_mul_naive(short *dst, const short *a, const short *b, int add) {
    // Perform polynomial multiplication with optional addition
    dst[0] = ((dst[0] & -add) + a[0] * b[0] + NEG(a[3]) * b[1] + NEG(a[2]) * b[2] + NEG(a[1]) * b[3]) % TK_Q;
    dst[1] = ((dst[1] & -add) + a[1] * b[0] + a[0] * b[1] + NEG(a[3]) * b[2] + NEG(a[2]) * b[3]) % TK_Q;
    dst[2] = ((dst[2] & -add) + a[2] * b[0] + a[1] * b[1] + a[0] * b[2] + NEG(a[3]) * b[3]) % TK_Q;
    dst[3] = ((dst[3] & -add) + a[3] * b[0] + a[2] * b[1] + a[1] * b[2] + a[0] * b[3]) % TK_Q;
}

// Multiplies a matrix by a vector
static void toy_mulmv(short *dst, const short *mat, const short *vec) {
    memset(dst, 0, TK_K * TK_N * sizeof(short));
    for (int kv = 0, idx = 0; kv < TK_K * TK_N; kv += TK_N) {
        for (int k = 0; k < TK_K * TK_N; k += TK_N, idx += TK_N) {
            toy_polynomial_mul_naive(dst + kv, mat + idx, vec + k, 1);
        }
    }
}

// Multiplies the transpose of a matrix by a vector
static void toy_multiply_transpose_by_vector(short *dst, const short *mat, const short *vec) {
    memset(dst, 0, TK_K * TK_N * sizeof(short));
    for (int kv = 0; kv < TK_K * TK_N; kv += TK_N) {
        for (int k = 0; k < TK_K * TK_N; k += TK_N) {
            toy_polynomial_mul_naive(dst + kv, mat + TK_K * k + kv, vec + k, 1);
        }
    }
}

// Computes the dot product of two vectors
static void toy_dot_product(short *dst, const short *v1, const short *v2) {
    memset(dst, 0, TK_N * sizeof(short));
    for (int k = 0; k < TK_K * TK_N; k += TK_N) {
        toy_polynomial_mul_naive(dst, v1 + k, v2 + k, 1);
    }
}

// Adds two vectors
static void toy_add(short *dst, const short *v1, const short *v2, int count, int v2_neg) {
    for (int k = 0; k < count; ++k) {
        short val = v2[k];
        if (v2_neg)
            val = NEG(val);
        dst[k] = (v1[k] + val) % TK_Q;
    }
}

// Key generation function
void toy_gen(short *A, short *t, short *s) {
    short e[TK_K * TK_N];
    for (int k = 0; k < TK_K * TK_K * TK_N; ++k)
        A[k] = rand() % TK_Q;
    toy_fill_small_random_values(s, TK_K * TK_N);
    toy_fill_small_random_values(e, TK_K * TK_N);

    toy_mulmv(t, A, s); // t = A.s + e
    toy_add(t, t, e, TK_K * TK_N, 0);
}

// Encryption function
void toy_enc(const short *A, const short *t, int plain, short *u, short *v) {
    short r[TK_K * TK_N], el[TK_K * TK_N], e2[TK_N];
    toy_fill_small_random_values(r, TK_K * TK_N);
    toy_fill_small_random_values(el, TK_K * TK_N);
    toy_fill_small_random_values(e2, TK_N);

    toy_multiply_transpose_by_vector(u, A, r); // u = A^T . r + el
    toy_add(u, u, el, TK_K * TK_N, 0);

    toy_dot_product(v, t, r); // v = t^T . r + e2 + plain * q/2
    toy_add(v, v, e2, TK_N, 0);

    for (int k = 0; k < TK_N; ++k)
        v[k] = (v[k] + ((TK_Q >> 1) & -(plain >> k & 1))) % TK_Q;
}

// Decryption function
int toy_dec(const short *s, const short *u, const short *v) {
    short p[TK_N], plain;
    toy_dot_product(p, s, u);
    toy_add(p, v, p, TK_N, 1);

    plain = 0;

    for (int k = 0; k < TK_N; ++k) {
        int val = p[k];

        if (val > TK_Q / 2)
            val -= TK_Q;

        int bit = abs(val) > TK_Q / 4;
        plain |= bit << k;
    }

    return plain;
}

// Main function
int main() {
    // Define parameters
    short A[TK_K * TK_K * TK_N];
    short t[TK_K * TK_N];
    short s[TK_K * TK_N];
    short u[TK_K * TK_N];
    short v[TK_N];

    // Key generation
    toy_gen(A, t, s);

    // Encryption
    int plaintext = 0; // Example plaintext (4 bits)
    //print plain text 
    printf("Original plaintext: %d\n", plaintext);
    toy_enc(A, t, plaintext, u, v);

    // Print the ciphertext
    printf("Cipher Text: ");
    for (int i = 0; i < TK_N; ++i) {
        printf("%d ", v[i]);
    }
    printf("\n");
    
    // Decryption
    int decrypted = toy_dec
(s, u, v);

    // Display results
    printf("Decrypted plaintext: %d\n", decrypted);

    return 0;
}
