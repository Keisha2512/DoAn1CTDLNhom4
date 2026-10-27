/*
    CAU I - NHAP / HIEU CHINH DIEM CHO LOP TIN CHI
    Module nghiep vu, khong chua main() va khong chua giao dien menu.

    Nguoi goi can tim lop theo nien khoa, hoc ky, maMH, nhom roi goi
    capNhatDiem(). Ham capNhatDiem sua truc tiep node DangKy, do do cac ham
    in bang diem/tinh diem sau do se doc duoc diem moi nhat.
*/

#include "../include/KhaiBaoDoAn.h"
#include <iostream>
#include <iomanip>
#include <cstring>

using std::cout;
using std::left;
using std::right;
using std::setw;

bool capNhatDiem(LopTinChi& ltc, const char* maSV, float diem) {
    if (maSV == nullptr || maSV[0] == '\0' || !diemHopLe(diem)) return false;
    if (ltc.huyLop) return false;

    NodeDK* p = ltc.dsDK;
    while (p != nullptr) {
        if (std::strcmp(p->data.maSV, maSV) == 0) {
            // Khong nhap/sua diem cho dang ky da bi huy.
            if (p->data.huyDangKy) return false;
            p->data.diem = diem;
            return true;
        }
        p = p->next;
    }
    return false;
}

bool capNhatDiem(DSLopTinChi& dsLTC, const char* nienKhoa, int hocKy,
                 const char* maMH, int nhom, const char* maSV, float diem) {
    if (nienKhoa == nullptr || maMH == nullptr || maSV == nullptr ||
        nienKhoa[0] == '\0' || maMH[0] == '\0' || maSV[0] == '\0' ||
        hocKy <= 0 || nhom <= 0 || !diemHopLe(diem)) {
        return false;
    }

    LopTinChi* ltc = timLopTinChi(dsLTC, nienKhoa, hocKy, maMH, nhom);
    if (ltc == nullptr) return false;
    return capNhatDiem(*ltc, maSV, diem);
}

void inBangNhapDiem(const DSLop& dsLop, const LopTinChi& ltc) {
    cout << "\nNHAP / HIEU CHINH DIEM\n";
    cout << "Ma LTC: " << ltc.maLTC
         << " | Ma MH: " << ltc.maMH
         << " | Nien khoa: " << ltc.nienKhoa
         << " | Hoc ky: " << ltc.hocKy
         << " | Nhom: " << ltc.nhom << "\n\n";

    cout << left << setw(5) << "STT"
         << setw(18) << "MASV"
         << setw(32) << "HO"
         << setw(18) << "TEN"
         << right << setw(10) << "DIEM" << '\n';
    cout << "---------------------------------------------------------------------------------\n";

    int stt = 0;
    const NodeDK* dk = ltc.dsDK;
    while (dk != nullptr) {
        if (!dk->data.huyDangKy) {
            const Lop* lopChuaSV = nullptr;
            const NodeSV* svNode = timSV(dsLop, dk->data.maSV, &lopChuaSV);
            if (svNode != nullptr) {
                ++stt;
                cout << left << setw(5) << stt
                     << setw(18) << svNode->data.maSV
                     << setw(32) << svNode->data.ho
                     << setw(18) << svNode->data.ten;
                if (diemHopLe(dk->data.diem))
                    cout << right << setw(10) << std::fixed << std::setprecision(2)
                         << dk->data.diem << '\n';
                else
                    cout << right << setw(10) << "Chua co" << '\n';
            }
        }
        dk = dk->next;
    }

    if (stt == 0) cout << "Lop tin chi chua co sinh vien dang ky hop le.\n";
    cout << "\nGhi chu: Ham nay chi in danh sach. Nhap/sua diem bang capNhatDiem().\n";
}
