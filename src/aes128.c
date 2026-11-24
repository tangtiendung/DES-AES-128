#include <stdio.h>
#include <stdint.h>

uint8_t RCON[11] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

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

// mo rong khoa 16 byte thanh 44 word (11 khoa vong k0..k10)
void mo_rong_khoa(uint8_t key[16], uint8_t w[44][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            w[i][j] = key[4 * i + j];
        }
    }

    for (int i = 4; i < 44; i++) {
        uint8_t temp[4];
        for (int j = 0; j < 4; j++) {
            temp[j] = w[i - 1][j];
        }
        // ham g: dich vong trai 1 byte -> S-box -> XOR RCON
        if (i % 4 == 0) {
            uint8_t t0 = temp[0];
            temp[0] = tinh_sbox(temp[1]) ^ RCON[i / 4];
            temp[1] = tinh_sbox(temp[2]);
            temp[2] = tinh_sbox(temp[3]);
            temp[3] = tinh_sbox(t0);
        }
        for (int j = 0; j < 4; j++) {
            w[i][j] = w[i - 4][j] ^ temp[j];
        }
    }
}

void sub_bytes(uint8_t state[4][4]) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            state[r][c] = tinh_sbox(state[r][c]);
        }
    }
}

void shift_rows(uint8_t state[4][4]) {
    uint8_t tmp[4];
    for (int r = 1; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            tmp[c] = state[r][(c + r) % 4];
        }
        for (int c = 0; c < 4; c++) {
            state[r][c] = tmp[c];
        }
    }
}

void mix_columns(uint8_t state[4][4]) {
    for (int c = 0; c < 4; c++) {
        uint8_t s0 = state[0][c];
        uint8_t s1 = state[1][c];
        uint8_t s2 = state[2][c];
        uint8_t s3 = state[3][c];

        state[0][c] = nhan_gf(2, s0) ^ nhan_gf(3, s1) ^ s2 ^ s3;
        state[1][c] = s0 ^ nhan_gf(2, s1) ^ nhan_gf(3, s2) ^ s3;
        state[2][c] = s0 ^ s1 ^ nhan_gf(2, s2) ^ nhan_gf(3, s3);
        state[3][c] = nhan_gf(3, s0) ^ s1 ^ s2 ^ nhan_gf(2, s3);
    }
}

void add_round_key(uint8_t state[4][4], uint8_t w[44][4], int vong) {
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            state[r][c] ^= w[4 * vong + c][r];
        }
    }
}

void in_state(const char *ten, uint8_t state[4][4]) {
    printf("  %s:\n", ten);
    for (int r = 0; r < 4; r++) {
        printf("    %02X %02X %02X %02X\n",
               state[r][0], state[r][1], state[r][2], state[r][3]);
    }
}

int main() {
    uint8_t pt[16]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x11, 0x11};
    uint8_t key[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t w[44][4];
    uint8_t state[4][4];

    mo_rong_khoa(key, w);

    printf("Khoa vong 1 (k1): ");
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) printf("%02X", w[4 + c][r]);
        printf(" ");
    }
    printf("\n\n");

    // nap plaintext vao ma tran 4x4 theo cot
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) state[r][c] = pt[4 * c + r];
    }

    add_round_key(state, w, 0);
    sub_bytes(state);
    in_state("Sau SubBytes (S-box)", state);
    shift_rows(state);
    in_state("Sau ShiftRows", state);
    mix_columns(state);
    in_state("Sau MixColumns", state);
    add_round_key(state, w, 1);
    in_state("Sau AddRoundKey (dau ra vong 1)", state);

    return 0;
}