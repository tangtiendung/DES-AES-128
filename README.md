# Bài tập Lý thuyết mật mã: DES & AES-128

- **Sinh viên:** Tăng Tiến Dũng
- **MSSV:** 20233350
- **Ngôn ngữ:** C

## Cấu trúc thư mục

- `src/des.c`: Cài đặt mã hóa DES 16 vòng (gồm in bảng tổng quát IP, E, P và in trạng thái từng vòng).
- `src/aes128.c`: Cài đặt mã hóa AES-128 10 vòng (S-box tính trực tiếp theo công thức $S(x) = A \cdot x^{-1} \oplus 0x63$ trên $GF(2^8)$).
- `results/des_output.txt`: Kết quả chạy kiểm định thuật toán DES.
- `results/aes_output.txt`: Kết quả chạy kiểm định thuật toán AES-128.

## Cách biên dịch và chạy

```bash
gcc src/des.c -o des.exe
./des.exe > results/des_output.txt

gcc src/aes128.c -o aes128.exe
./aes128.exe > results/aes_output.txt