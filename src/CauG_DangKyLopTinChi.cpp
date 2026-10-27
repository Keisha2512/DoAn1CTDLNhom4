/*
    CAU G - DANG KY LOP TIN CHI
    Module nghiep vu, khong chua main() va khong chua giao dien menu.

    Luu y khi ghep vao repo:
    - File nay dang include ../include/KhaiBaoDoAn.h theo cau truc thu muc
      include/ va src/ trong repo cua nhom.
    - Cac ham timSV(), timLopTinChi(), chuoiRong() duoc khai bao trong header
      va se duoc dinh nghia boi cac module tuong ung cua nhom.
*/

#include "../include/KhaiBaoDoAn.h"
#include <iostream>
#include <iomanip>
#include <cstring>

using std::cout;
using std::left;
using std::right;
using std::setw;

// Dem so dang ky con hieu luc (khong bi huy) trong mot lop tin chi.
int demSVDangKyHopLe(const LopTinChi& ltc) {
    int dem = 0;
    const NodeDK* p = ltc.dsDK;
    while (p != nullptr) {
        if (!p->data.huyDangKy) ++dem;
        p = p->next;
    }
    return dem;
}

int soChoTrong(const LopTinChi& ltc) {
    int conLai = ltc.soSVMax - demSVDangKyHopLe(ltc);
    return (conLai > 0) ? conLai : 0;
}

// Lop da thi neu co it nhat mot dang ky dang hieu luc da co diem hop le.
bool lopTinChiDaThi(const LopTinChi& ltc) {
    const NodeDK* p = ltc.dsDK;
    while (p != nullptr) {
        if (!p->data.huyDangKy && diemHopLe(p->data.diem)) return true;
        p = p->next;
    }
    return false;
}

bool lopTinChiChoPhepDangKy(const LopTinChi& ltc) {
    if (ltc.huyLop) return false;
    if (lopTinChiDaThi(ltc)) return false;
    if (ltc.soSVMax <= 0) return false;
    if (demSVDangKyHopLe(ltc) >= ltc.soSVMax) return false;
    return true;
}

// Tim dang ky theo maSV. Neu da co ban ghi bi huy thi ham van tra ve ban ghi do.
NodeDK* timDangKy(NodeDK* head, const char* maSV) {
    if (maSV == nullptr) return nullptr;
    NodeDK* p = head;
    while (p != nullptr) {
        if (std::strcmp(p->data.maSV, maSV) == 0) return p;
        p = p->next;
    }
    return nullptr;
}

const NodeDK* timDangKy(const NodeDK* head, const char* maSV) {
    if (maSV == nullptr) return nullptr;
    const NodeDK* p = head;
    while (p != nullptr) {
        if (std::strcmp(p->data.maSV, maSV) == 0) return p;
        p = p->next;
    }
    return nullptr;
}

// Kiem tra SV dang co dang ky chua huy o mon hoc nay trong cung NK/HK.
bool sinhVienDaDangKyMon(const DSLopTinChi& dsLTC, const char* maSV,
                         const char* nienKhoa, int hocKy, const char* maMH) {
    if (maSV == nullptr || nienKhoa == nullptr || maMH == nullptr) return false;

    for (int i = 0; i < dsLTC.soLuong; ++i) {
        const LopTinChi* ltc = dsLTC.ds[i];
        if (ltc == nullptr || ltc->huyLop) continue;
        if (std::strcmp(ltc->nienKhoa, nienKhoa) != 0 || ltc->hocKy != hocKy)
            continue;
        if (std::strcmp(ltc->maMH, maMH) != 0) continue;

        const NodeDK* dk = timDangKy(ltc->dsDK, maSV);
        if (dk != nullptr && !dk->data.huyDangKy) return true;
    }
    return false;
}

// In cac lop da mo theo nien khoa/hoc ky de phia main/UI cho SV chon maLTC.
// Day la ket qua nghiep vu dang ky; khong xu ly phim hay ve khung giao dien.
void inLopTinChiMoTheoHocKy(const NodeMonHoc* dsMonHoc,
                            const DSLopTinChi& dsLTC,
                            const char* nienKhoa, int hocKy) {
    if (nienKhoa == nullptr || hocKy <= 0) {
        cout << "Nien khoa hoac hoc ky khong hop le.\n";
        return;
    }

    cout << "\nDANH SACH LOP TIN CHI MO - " << nienKhoa
         << " - Hoc ky " << hocKy << "\n";
    cout << left << setw(8) << "MALTC"
         << setw(13) << "MAMH"
         << setw(36) << "TEN MON HOC"
         << right << setw(7) << "NHOM"
         << setw(12) << "DA DANG KY"
         << setw(12) << "CON CHO" << '\n';
    cout << "-----------------------------------------------------------------------------------------------\n";

    int soDong = 0;
    for (int i = 0; i < dsLTC.soLuong; ++i) {
        const LopTinChi* ltc = dsLTC.ds[i];
        if (ltc == nullptr || ltc->huyLop) continue;
        if (std::strcmp(ltc->nienKhoa, nienKhoa) != 0 || ltc->hocKy != hocKy)
            continue;

        const NodeMonHoc* mh = timMonHoc(dsMonHoc, ltc->maMH);
        const char* tenMH = (mh != nullptr) ? mh->data.tenMH : "[Khong tim thay mon hoc]";
        cout << left << setw(8) << ltc->maLTC
             << setw(13) << ltc->maMH
             << setw(36) << tenMH
             << right << setw(7) << ltc->nhom
             << setw(12) << demSVDangKyHopLe(*ltc)
             << setw(12) << soChoTrong(*ltc) << '\n';
        ++soDong;
    }

    if (soDong == 0) cout << "Khong co lop tin chi nao duoc mo trong hoc ky nay.\n";
}

// Dang ky mot SV vao lop tin chi da chon.
// Cac dieu kien duoc kiem tra truoc khi sua danh sach dang ky.
bool dangKyLopTinChi(const DSLop& dsLop, DSLopTinChi& dsLTC,
                    const char* maSV, int maLTC) {
    if (maSV == nullptr || maSV[0] == '\0') return false;

    const NodeSV* sv = timSV(dsLop, maSV, nullptr);
    if (sv == nullptr) return false;

    LopTinChi* ltc = timLopTinChi(dsLTC, maLTC);
    if (ltc == nullptr || !lopTinChiChoPhepDangKy(*ltc)) return false;

    NodeDK* dkDaCo = timDangKy(ltc->dsDK, maSV);
    if (dkDaCo != nullptr) {
        // Dang ky dang hieu luc thi khong duoc dang ky lap.
        if (!dkDaCo->data.huyDangKy) return false;
        // Neu ban ghi cu da co diem, khong kich hoat lai lich su dang ky.
        if (diemHopLe(dkDaCo->data.diem)) return false;
        // Cho phep dang ky lai neu ban ghi cu da huy va chua tung co diem.
        dkDaCo->data.huyDangKy = false;
        dkDaCo->data.diem = CHUA_CO_DIEM;
        return true;
    }

    if (sinhVienDaDangKyMon(dsLTC, maSV, ltc->nienKhoa, ltc->hocKy, ltc->maMH))
        return false;

    DangKy duLieu{};
    std::strncpy(duLieu.maSV, maSV, MAX_MASV - 1);
    duLieu.maSV[MAX_MASV - 1] = '\0';
    duLieu.diem = CHUA_CO_DIEM;
    duLieu.huyDangKy = false;

    NodeDK* nodeMoi = taoNodeDK(duLieu);
    if (nodeMoi == nullptr) return false;

    // Chen cuoi danh sach de giu thu tu dang ky.
    if (ltc->dsDK == nullptr) {
        ltc->dsDK = nodeMoi;
    } else {
        NodeDK* cuoi = ltc->dsDK;
        while (cuoi->next != nullptr) cuoi = cuoi->next;
        cuoi->next = nodeMoi;
    }
    return true;
}

// Huy dang ky cua mot SV. Khong xoa node de giu lich su.
bool huyDangKyLopTinChi(DSLopTinChi& dsLTC, const char* maSV, int maLTC) {
    if (maSV == nullptr || maSV[0] == '\0') return false;
    LopTinChi* ltc = timLopTinChi(dsLTC, maLTC);
    if (ltc == nullptr || ltc->huyLop || lopTinChiDaThi(*ltc)) return false;

    NodeDK* dk = timDangKy(ltc->dsDK, maSV);
    if (dk == nullptr || dk->data.huyDangKy) return false;
    if (diemHopLe(dk->data.diem)) return false;

    dk->data.huyDangKy = true;
    return true;
}
