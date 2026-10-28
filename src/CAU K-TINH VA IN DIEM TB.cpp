/*
    CAU K - IN DIEM TRUNG BINH KET THUC KHOA HOC CUA 1 LOP THEO SO TIN CHI
    Module nghiep vu, khong chua main() va khong chua giao dien menu.

    Diem trung binh khoa hoc cua sinh vien duoc tinh co trong so tin chi
    (soTCLyThuyet + soTCThucHanh), trong do moi mon hoc chi lay diem cao nhat
    trong cac lop tin chi da co diem hop le.
*/

#include "../include/KhaiBaoDoAn.h"
#include <iostream>
#include <iomanip>
#include <cstring>

using std::cout;
using std::left;
using std::right;
using std::setw;

// Ham phu: tim diem cao nhat cua sinh vien o 1 mon hoc theo pham vi (hoc ky hoac toan khoa)
static float timDiemCaoNhatMonTheoPhamVi(const DSLopTinChi& dsLTC,
                                         const char* maSV,
                                         const char* maMH,
                                         PhamViTinhDiem phamVi,
                                         const char* nienKhoa,
                                         int hocKy) {
    if (maSV == nullptr || maMH == nullptr || maSV[0] == '\0' || maMH[0] == '\0') {
        return CHUA_CO_DIEM;
    }

    if (phamVi == PhamViTinhDiem::THEO_HOC_KY) {
        if (nienKhoa == nullptr || nienKhoa[0] == '\0' || hocKy <= 0) {
            return CHUA_CO_DIEM;
        }
    }

    float diemMax = CHUA_CO_DIEM;

    for (int i = 0; i < dsLTC.soLuong; ++i) {
        const LopTinChi* ltc = dsLTC.ds[i];
        if (ltc == nullptr || ltc->huyLop) continue;
        if (std::strcmp(ltc->maMH, maMH) != 0) continue;

        if (phamVi == PhamViTinhDiem::THEO_HOC_KY) {
            if (ltc->hocKy != hocKy || std::strcmp(ltc->nienKhoa, nienKhoa) != 0) {
                continue;
            }
        }

        const NodeDK* dk = ltc->dsDK;
        while (dk != nullptr) {
            if (!dk->data.huyDangKy && std::strcmp(dk->data.maSV, maSV) == 0) {
                if (diemHopLe(dk->data.diem)) {
                    if (!diemHopLe(diemMax) || dk->data.diem > diemMax) {
                        diemMax = dk->data.diem;
                    }
                }
            }
            dk = dk->next;
        }
    }

    return diemMax;
}

// Ham phu: duyet cay BST mon hoc de tich luy (diem cao nhat * so tin chi) cua tung mon
static void duyetCayTinhDTB(const NodeMonHoc* root,
                            const DSLopTinChi& dsLTC,
                            const char* maSV,
                            PhamViTinhDiem phamVi,
                            const char* nienKhoa,
                            int hocKy,
                            double& tongDiemNhanTC,
                            int& tongTinChi) {
    if (root == nullptr) return;

    duyetCayTinhDTB(root->left, dsLTC, maSV, phamVi, nienKhoa, hocKy,
                    tongDiemNhanTC, tongTinChi);

    int soTC = root->data.soTCLyThuyet + root->data.soTCThucHanh;
    if (soTC > 0) {
        float diemMon = (phamVi == PhamViTinhDiem::TOAN_KHOA)
                            ? diemCaoNhatMonCuaSV(dsLTC, maSV, root->data.maMH)
                            : timDiemCaoNhatMonTheoPhamVi(dsLTC, maSV, root->data.maMH,
                                                          phamVi, nienKhoa, hocKy);
        if (diemHopLe(diemMon)) {
            tongDiemNhanTC += static_cast<double>(diemMon) * soTC;
            tongTinChi += soTC;
        }
    }

    duyetCayTinhDTB(root->right, dsLTC, maSV, phamVi, nienKhoa, hocKy,
                    tongDiemNhanTC, tongTinChi);
}

float diemCaoNhatMonCuaSV(const DSLopTinChi& dsLTC,
                          const char* maSV, const char* maMH) {
    return timDiemCaoNhatMonTheoPhamVi(dsLTC, maSV, maMH,
                                       PhamViTinhDiem::TOAN_KHOA, nullptr, 0);
}

float tinhDTBTheoTinChi(const DSLopTinChi& dsLTC,
                        const NodeMonHoc* dsMonHoc,
                        const char* maSV,
                        PhamViTinhDiem phamVi,
                        const char* nienKhoa,
                        int hocKy) {
    if (dsMonHoc == nullptr || maSV == nullptr || maSV[0] == '\0') {
        return CHUA_CO_DIEM;
    }

    if (phamVi == PhamViTinhDiem::THEO_HOC_KY &&
        (nienKhoa == nullptr || nienKhoa[0] == '\0' || hocKy <= 0)) {
        return CHUA_CO_DIEM;
    }

    double tongDiemNhanTC = 0.0;
    int tongTinChi = 0;

    duyetCayTinhDTB(dsMonHoc, dsLTC, maSV, phamVi, nienKhoa, hocKy,
                    tongDiemNhanTC, tongTinChi);

    if (tongTinChi <= 0) return CHUA_CO_DIEM;
    return static_cast<float>(tongDiemNhanTC / tongTinChi);
}

void inBangDTBKetThucKhoa(const Lop& lop, const DSLopTinChi& dsLTC,
                          const NodeMonHoc* dsMonHoc) {
    cout << "\n          BANG THONG KE DIEM TRUNG BINH KHOA HOC\n";
    cout << "          Lop     : " << lop.maLop << "\n\n";

    cout << left << setw(5) << "STT"
         << setw(18) << "MASV"
         << setw(32) << "HO"
         << setw(18) << "TEN"
         << right << setw(10) << "Diem TB" << '\n';
    cout << "---------------------------------------------------------------------------------\n";

    int stt = 0;
    const NodeSV* sv = lop.dsSV;
    while (sv != nullptr) {
        ++stt;
        float dtb = tinhDTBTheoTinChi(dsLTC, dsMonHoc, sv->data.maSV,
                                      PhamViTinhDiem::TOAN_KHOA);

        cout << left << setw(5) << stt
             << setw(18) << sv->data.maSV
             << setw(32) << sv->data.ho
             << setw(18) << sv->data.ten;

        if (diemHopLe(dtb)) {
            cout << right << setw(10) << std::fixed << std::setprecision(2)
                 << dtb << '\n';
        } else {
            cout << right << setw(10) << "Chua co" << '\n';
        }

        sv = sv->next;
    }

    if (stt == 0) {
        cout << "Lop chua co sinh vien.\n";
    }
}

void inBangDTBKetThucKhoa(const DSLop& dsLop, const DSLopTinChi& dsLTC,
                          const NodeMonHoc* dsMonHoc, const char* maLop) {
    if (maLop == nullptr || maLop[0] == '\0') {
        cout << "\nMa lop khong hop le.\n";
        return;
    }

    const Lop* lop = timLop(dsLop, maLop);
    if (lop == nullptr) {
        cout << "\nKhong tim thay lop co ma: " << maLop << '\n';
        return;
    }

    inBangDTBKetThucKhoa(*lop, dsLTC, dsMonHoc);
}
