#include <stdio.h>
#include <stdint.h>

// nhan 2 byte trong truong GF(2^8) voi da thuc m(x) = x^8 + x^4 + x^3 + x + 1 (0x1B)
uint8_t nhan_gf(uint8_t a, uint8_t b) {
    uint8_t kq = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) {
            kq ^= a;
        }
        uint8_t bit_cao = a & 0x80;
        a <<= 1;
        if (bit_cao) {
            a ^= 0x1B;
        }
        b >>= 1;
    }
    return kq;
}

// tim nghich dao u = x^(-1) trong GF(2^8) bang x^254, quy uoc 0^(-1) = 0
uint8_t nghich_dao_gf(uint8_t x) {
    if (x == 0) return 0;
    uint8_t kq = 1;
    uint8_t co_so = x;
    int mu = 254;
    while (mu > 0) {
        if (mu & 1) {
            kq = nhan_gf(kq, co_so);
        }
        co_so = nhan_gf(co_so, co_so);
        mu >>= 1;
    }
    return kq;
}

// tinh S-box theo cong thuc S(x) = A * u XOR c (c = 0x63)
uint8_t tinh_sbox(uint8_t x) {
    int A[8][8] = {
        {1, 0, 0, 0, 1, 1, 1, 1},
        {1, 1, 0, 0, 0, 1, 1, 1},
        {1, 1, 1, 0, 0, 0, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 1, 1, 1, 1, 1}
    };
    uint8_t u = nghich_dao_gf(x);
    uint8_t c = 0x63;
    uint8_t y = 0;

    for (int i = 0; i < 8; i++) {
        int bit = (c >> i) & 1;
        for (int j = 0; j < 8; j++) {
            if (A[i][j] == 1) {
                bit ^= (u >> j) & 1;
            }
        }
        y |= (bit << i);
    }
    return y;
}

int main() {
    printf("Kiem tra phep tinh GF(2^8) va S-box:\n");
    printf("0x02^(-1) = 0x%02X\n", nghich_dao_gf(0x02));
    printf("0x11^(-1) = 0x%02X\n", nghich_dao_gf(0x11));
    printf("S(0x00)   = 0x%02X\n", tinh_sbox(0x00));
    printf("S(0x11)   = 0x%02X\n", tinh_sbox(0x11));
    return 0;
}