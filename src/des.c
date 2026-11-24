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

// hoan vi bit theo bang cho truoc
uint64_t hoan_vi(uint64_t in, int bang[], int n_in, int n_out) {
    uint64_t out = 0;
    for (int i = 0; i < n_out; i++) {
        uint64_t bit = (in >> (n_in - bang[i])) & 1ULL;
        out = (out << 1) | bit;
    }
    return out;
}

// in vi tri bit tong quat b0..b63 qua IP, E(R0) va b0..b31 qua P
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

int main() {
    in_bang_tong_quat();

    uint64_t pt = 0x0123456789ABCDEFULL;
    uint64_t sau_ip = hoan_vi(pt, IP, 64, 64);
    printf("Test IP: %016llX\n", (unsigned long long)sau_ip);
    return 0;
}