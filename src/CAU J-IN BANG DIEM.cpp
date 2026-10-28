/*
    CAU J - IN BANG DIEM MON HOC CUA 1 LOP TIN CHI
    Module nghiep vu, khong chua main() va khong chua giao dien menu.

    Nguoi goi truyen vao nien khoa, hoc ky, maMH, nhom (hoac truyen truc tiep
    doi tuong LopTinChi da tim duoc) de xuat bang diem mon hoc ra man hinh.
*/

#include "../include/KhaiBaoDoAn.h"
#include <iostream>
#include <iomanip>
#include <cstring>

using std::cout;
using std::left;
using std::right;
using std::setw;

void inBangDiemMonHoc(const DSLop& dsLop, const NodeMonHoc* dsMonHoc,
                      const LopTinChi& ltc) {
    if (ltc.huyLop) {
        cout << "\nLop tin chi (Ma LTC: " << ltc.maLTC << ") da bi huy.\n";
        return;
    }

    // Tra cuu ten mon hoc tu cay nhi phan tim kiem dsMonHoc theo maMH
    const NodeMonHoc* mhNode = timMonHoc(dsMonHoc, ltc.maMH);
    const char* tenMH = (mhNode != nullptr) ? mhNode->data.tenMH : ltc.maMH;

    // In tieu de bang diem theo dung mau yeu cau cua cau j
    cout << "\n               BANG DIEM MON HOC " << tenMH << '\n';
    cout << "               Nien khoa : " << ltc.nienKhoa
         << "   Hoc ky : " << ltc.hocKy
         << "   Nhom : " << ltc.nhom << "\n\n";

    cout << left << setw(5) << "STT"
         << setw(18) << "MASV"
         << setw(32) << "HO"
         << setw(18) << "TEN"
         << right << setw(10) << "DIEM" << '\n';
    cout << "---------------------------------------------------------------------------------\n";

    int stt = 0;
    const NodeDK* dk = ltc.dsDK;
    while (dk != nullptr) {
        // Chi in cac sinh vien co dang ky hop le (chua huy dang ky)
        if (!dk->data.huyDangKy) {
            const Lop* lopChuaSV = nullptr;
            const NodeSV* svNode = timSV(dsLop, dk->data.maSV, &lopChuaSV);
            if (svNode != nullptr) {
                ++stt;
                cout << left << setw(5) << stt
                     << setw(18) << svNode->data.maSV
                     << setw(32) << svNode->data.ho
                     << setw(18) << svNode->data.ten;

                if (diemHopLe(dk->data.diem)) {
                    cout << right << setw(10) << std::fixed << std::setprecision(2)
                         << dk->data.diem << '\n';
                } else {
                    cout << right << setw(10) << "Chua co" << '\n';
                }
            }
        }
        dk = dk->next;
    }

    if (stt == 0) {
        cout << "Lop tin chi chua co sinh vien dang ky hop le.\n";
    }
}

void inBangDiemMonHoc(const DSLop& dsLop, const DSLopTinChi& dsLTC,
                      const NodeMonHoc* dsMonHoc, const char* nienKhoa,
                      int hocKy, const char* maMH, int nhom) {
    if (nienKhoa == nullptr || maMH == nullptr ||
        nienKhoa[0] == '\0' || maMH[0] == '\0' ||
        hocKy <= 0 || nhom <= 0) {
        cout << "\nThong tin tra cuu lop tin chi khong hop le.\n";
        return;
    }

    const LopTinChi* ltc = timLopTinChi(dsLTC, nienKhoa, hocKy, maMH, nhom);
    if (ltc == nullptr) {
        cout << "\nKhong tim thay lop tin chi phu hop (Nien khoa: " << nienKhoa
             << ", Hoc ky: " << hocKy
             << ", Ma MH: " << maMH
             << ", Nhom: " << nhom << ").\n";
        return;
    }

    inBangDiemMonHoc(dsLop, dsMonHoc, *ltc);
}
