#include "../include/KhaiBaoDoAn.h"
#include <iostream>
#include <iomanip>

using namespace std;

static const NodeSV* timSinhVienTrongDSLop(
    const DSLop& dsLop,
    const char* maSV
) {
    for (int i = 0; i < dsLop.soLuong; i++) {
        const NodeSV* p = dsLop.ds[i].dsSV;

        while (p != nullptr) {
            if (soSanhChuoi(p->data.maSV, maSV) == 0) {
                return p;
            }

            p = p->next;
        }
    }

    return nullptr;
}

void inDSDangKyLopTinChi(
    const DSLop& dsLop,
    const LopTinChi& ltc
) {
    cout << left
         << setw(18) << "Ma sinh vien"
         << setw(35) << "Ho ten"
         << setw(15) << "Trang thai"
         << '\n';

    const NodeDK* p = ltc.dsDK;

    while (p != nullptr) {
        const NodeSV* sv = timSinhVienTrongDSLop(
            dsLop, p->data.maSV
        );

        if (sv != nullptr) {
            cout << left
                 << setw(18) << sv->data.maSV;

            cout << sv->data.ho << " " << sv->data.ten;

            if (p->data.huyDangKy) {
                cout << setw(15) << "Da huy";
            } else {
                cout << setw(15) << "Chua huy";
            }

            cout << '\n';
        }

        p = p->next;
    }
}

void inDSDangKyLopTinChi(
    const DSLop& dsLop,
    const DSLopTinChi& dsLTC,
    const char* nienKhoa,
    int hocKy,
    const char* maMH,
    int nhom
) {
    const LopTinChi* ltc = nullptr;

    for (int i = 0; i < dsLTC.soLuong; i++) {
        const LopTinChi* p = dsLTC.ds[i];

        if (p == nullptr) continue;

        if (soSanhChuoi(p->nienKhoa, nienKhoa) == 0 &&
            p->hocKy == hocKy &&
            soSanhChuoi(p->maMH, maMH) == 0 &&
            p->nhom == nhom) {
            ltc = p;
            break;
        }
    }

    if (ltc == nullptr) {
        cout << "Khong tim thay lop tin chi phu hop!\n";
        return;
    }

    inDSDangKyLopTinChi(dsLop, *ltc);
}