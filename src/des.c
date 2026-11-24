#include <stdio.h>
#include <stdint.h>

int IP[64] = {
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

int IP_inv[64] = {
    40,  8, 48, 16, 56, 24, 64, 32,
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25
};

int E[48] = {
    32,  1,  2,  3,  4,  5,
     4,  5,  6,  7,  8,  9,
     8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

int P[32] = {
    16,  7, 20, 21, 29, 12, 28, 17,
     1, 15, 23, 26,  5, 18, 31, 10,
     2,  8, 24, 14, 32, 27,  3,  9,
    19, 13, 30,  6, 22, 11,  4, 25
};

int PC1[56] = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

int PC2[48] = {
    14, 17, 11, 24,  1,  5,
     3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

int SHIFT[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

int S[8][4][16] = {
    {
        {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
        { 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
        { 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
        {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}
    },
    {
        {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
        { 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
        { 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
        {13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9}
    },
    {
        {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
        {13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
        {13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
        { 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12}
    },
    {
        { 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
        {13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
        {10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
        { 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14}
    },
    {
        { 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
        {14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
        { 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
        {11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3}
    },
    {
        {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
        {10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
        { 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
        { 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13}
    },
    {
        { 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
        {13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
        { 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
        { 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12}
    },
    {
        {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
        { 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
        { 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
        { 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}
    }
};

uint64_t hoan_vi(uint64_t in, int bang[], int n_in, int n_out) {
    uint64_t out = 0;
    for (int i = 0; i < n_out; i++) {
        uint64_t bit = (in >> (n_in - bang[i])) & 1ULL;
        out = (out << 1) | bit;
    }
    return out;
}

uint32_t dich_trai_28(uint32_t x, int s) {
    return ((x << s) | (x >> (28 - s))) & 0x0FFFFFFF;
}

void in_bang_tong_quat() {
    printf("Bang IP (4 hang dau la L0, 4 hang sau la R0):\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            printf("b%-2d ", IP[i * 8 + j] - 1);
        }
        printf("\n");
    }

    printf("\nBang mo rong E(R0):\n");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 6; j++) {
            int vt = E[i * 6 + j] - 1;
            printf("b%-2d ", IP[32 + vt] - 1);
        }
        printf("\n");
    }

    printf("\nBang hoan vi P(b0..b31):\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            printf("b%-2d ", P[i * 8 + j] - 1);
        }
        printf("\n");
    }
    printf("\n");
}

void sinh_khoa(uint64_t key, uint64_t K[16]) {
    uint64_t k56 = hoan_vi(key, PC1, 64, 56);
    uint32_t C = (k56 >> 28) & 0x0FFFFFFF;
    uint32_t D = k56 & 0x0FFFFFFF;

    printf("Sinh khoa con:\n");
    printf("C0 = %07X, D0 = %07X\n", C, D);
    for (int i = 0; i < 16; i++) {
        C = dich_trai_28(C, SHIFT[i]);
        D = dich_trai_28(D, SHIFT[i]);
        uint64_t cd = ((uint64_t)C << 28) | D;
        K[i] = hoan_vi(cd, PC2, 56, 48);
        printf("Vong %2d: C = %07X | D = %07X | K%-2d = %012llX\n",
               i + 1, C, D, i + 1, (unsigned long long)K[i]);
    }
    printf("\n");
}

// ham f(R, Ki): mo rong E -> XOR Ki -> 8 S-box -> hoan vi P
uint32_t ham_f(uint32_t R, uint64_t Ki) {
    uint64_t ep = hoan_vi(R, E, 32, 48);
    uint64_t x = ep ^ Ki;

    uint32_t s_out = 0;
    for (int i = 0; i < 8; i++) {
        int b = (x >> (42 - 6 * i)) & 0x3F;
        int hang = ((b & 0x20) >> 4) | (b & 0x01);
        int cot = (b >> 1) & 0x0F;
        s_out = (s_out << 4) | S[i][hang][cot];
    }

    uint32_t p_out = (uint32_t)hoan_vi(s_out, P, 32, 32);

    printf("  E(R)      = %012llX\n", (unsigned long long)ep);
    printf("  E XOR K   = %012llX\n", (unsigned long long)x);
    printf("  Sau S-box = %08X\n", s_out);
    printf("  f = P(S)  = %08X\n", p_out);
    return p_out;
}

uint64_t ma_hoa_des(uint64_t pt, uint64_t key) {
    uint64_t K[16];
    printf("Plaintext : %016llX\n", (unsigned long long)pt);
    printf("Key       : %016llX\n\n", (unsigned long long)key);

    sinh_khoa(key, K);

    uint64_t ip = hoan_vi(pt, IP, 64, 64);
    uint32_t L = ip >> 32;
    uint32_t R = ip & 0xFFFFFFFF;
    printf("Sau IP: %016llX (L0 = %08X, R0 = %08X)\n\n",
           (unsigned long long)ip, L, R);

    for (int i = 0; i < 16; i++) {
        printf("Vong %d:\n", i + 1);
        uint32_t f = ham_f(R, K[i]);
        uint32_t L_moi = R;
        uint32_t R_moi = L ^ f;
        L = L_moi;
        R = R_moi;
        printf("  L%d = %08X, R%d = %08X\n\n", i + 1, L, i + 1, R);
    }

    // doi cho R16 va L16 truoc khi qua IP^-1
    uint64_t ghep = ((uint64_t)R << 32) | L;
    uint64_t ct = hoan_vi(ghep, IP_inv, 64, 64);

    printf("Sau doi cho (R16||L16): %016llX\n", (unsigned long long)ghep);
    printf("Ciphertext            : %016llX\n", (unsigned long long)ct);
    return ct;
}

int main() {
    in_bang_tong_quat();

    uint64_t pt  = 0x0123456789ABCDEFULL;
    uint64_t key = 0x133457799BBCDFF1ULL;
    ma_hoa_des(pt, key);
    return 0;
}