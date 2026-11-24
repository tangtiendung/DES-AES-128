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

uint64_t hoan_vi(uint64_t in, int bang[], int n_in, int n_out) {
    uint64_t out = 0;
    for (int i = 0; i < n_out; i++) {
        uint64_t bit = (in >> (n_in - bang[i])) & 1ULL;
        out = (out << 1) | bit;
    }
    return out;
}

// dich vong trai 28 bit
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

// sinh 16 khoa con K1..K16
void sinh_khoa(uint64_t key, uint64_t K[16]) {
    uint64_t k56 = hoan_vi(key, PC1, 64, 56);
    uint32_t C = (k56 >> 28) & 0x0FFFFFFF;
    uint32_t D = k56 & 0x0FFFFFFF;

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

int main() {
    in_bang_tong_quat();

    uint64_t key = 0x133457799BBCDFF1ULL;
    uint64_t K[16];
    sinh_khoa(key, K);
    return 0;
}