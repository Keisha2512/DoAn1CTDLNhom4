#include "../include/KhaiBaoDoAn.h"
#include <iostream>

using namespace std;

bool chuoiRong(const char* text) {
    if (text == nullptr) return true;

    int i = 0;
    while (text[i] != '\0') {
        if (text[i] != ' ' && text[i] != '\t') return false;
        i++;
    }
    return true;
}

int soSanhChuoi(const char* a, const char* b) {
    if (a == nullptr) a = "";
    if (b == nullptr) b = "";
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
        i++;
    }

    if (a[i] == '\0' && b[i] == '\0') return 0;
    if (a[i] == '\0') return -1;
    return 1;
}

static char doiChuThuong(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

int soSanhChuoiKhongPhanBietHoaThuong(
    const char* a, const char* b
) {
    if (a == nullptr) a = "";
    if (b == nullptr) b = "";
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0') {
        char x = doiChuThuong(a[i]);
        char y = doiChuThuong(b[i]);

        if (x < y) return -1;
        if (x > y) return 1;
        i++;
    }

    if (a[i] == '\0' && b[i] == '\0') return 0;
    if (a[i] == '\0') return -1;
    return 1;
}

bool chuoiChuaTuKhoa(const char* chuoi, const char* tuKhoa) {
    if (chuoi == nullptr || tuKhoa == nullptr) return false;
    if (tuKhoa[0] == '\0') return true;

    int i = 0;

    while (chuoi[i] != '\0') {
        int j = 0;

        while (chuoi[i + j] != '\0' &&
               tuKhoa[j] != '\0' &&
               doiChuThuong(chuoi[i + j]) == doiChuThuong(tuKhoa[j])) {
            j++;
        }

        if (tuKhoa[j] == '\0') return true;
        i++;
    }

    return false;
}

bool diemHopLe(float diem) {
    return diem >= 0.0f && diem <= 10.0f;
}

bool thongTinMonHocHopLe(const MonHoc& monHoc) {
    if (chuoiRong(monHoc.maMH)) return false;
    if (chuoiRong(monHoc.tenMH)) return false;
    if (monHoc.soTCLyThuyet < 0) return false;
    if (monHoc.soTCThucHanh < 0) return false;

    return true;
}

bool thongTinSinhVienHopLe(const SinhVien& sinhVien) {
    if (chuoiRong(sinhVien.maSV)) return false;
    if (chuoiRong(sinhVien.ho)) return false;
    if (chuoiRong(sinhVien.ten)) return false;
    if (chuoiRong(sinhVien.phai)) return false;
    if (chuoiRong(sinhVien.soDT)) return false;

    return true;
}

bool thongTinLopTinChiHopLe(const LopTinChi& ltc) {
    if (chuoiRong(ltc.maMH)) return false;
    if (chuoiRong(ltc.nienKhoa)) return false;
    if (ltc.hocKy <= 0) return false;
    if (ltc.nhom <= 0) return false;
    if (ltc.soSVMin < 0) return false;
    if (ltc.soSVMax <= 0) return false;
    if (ltc.soSVMin > ltc.soSVMax) return false;

    return true;
}