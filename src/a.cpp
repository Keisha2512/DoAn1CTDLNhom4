#include "../include/KhaiBaoDoAn.h"
#include <iostream>
#include <iomanip>

using namespace std;

LopTinChi* taoLopTinChi(const LopTinChi& duLieu) {
    LopTinChi* p = new LopTinChi;
    *p = duLieu;
    return p;
}

LopTinChi* timLopTinChi(DSLopTinChi& dsLTC, int maLTC) {
    for (int i = 0; i < dsLTC.soLuong; i++) {
        if (dsLTC.ds[i] != nullptr &&
            dsLTC.ds[i]->maLTC == maLTC) {
            return dsLTC.ds[i];
        }
    }

    return nullptr;
}

const LopTinChi* timLopTinChi(
    const DSLopTinChi& dsLTC, int maLTC
) {
    for (int i = 0; i < dsLTC.soLuong; i++) {
        if (dsLTC.ds[i] != nullptr &&
            dsLTC.ds[i]->maLTC == maLTC) {
            return dsLTC.ds[i];
        }
    }

    return nullptr;
}

LopTinChi* timLopTinChi(
    DSLopTinChi& dsLTC,
    const char* nienKhoa,
    int hocKy,
    const char* maMH,
    int nhom
) {
    for (int i = 0; i < dsLTC.soLuong; i++) {
        LopTinChi* p = dsLTC.ds[i];

        if (p != nullptr &&
            soSanhChuoi(p->nienKhoa, nienKhoa) == 0 &&
            p->hocKy == hocKy &&
            soSanhChuoi(p->maMH, maMH) == 0 &&
            p->nhom == nhom) {
            return p;
        }
    }

    return nullptr;
}

const LopTinChi* timLopTinChi(
    const DSLopTinChi& dsLTC,
    const char* nienKhoa,
    int hocKy,
    const char* maMH,
    int nhom
) {
    for (int i = 0; i < dsLTC.soLuong; i++) {
        const LopTinChi* p = dsLTC.ds[i];

        if (p != nullptr &&
            soSanhChuoi(p->nienKhoa, nienKhoa) == 0 &&
            p->hocKy == hocKy &&
            soSanhChuoi(p->maMH, maMH) == 0 &&
            p->nhom == nhom) {
            return p;
        }
    }

    return nullptr;
}

int demSVDangKyHopLe(const LopTinChi& ltc) {
    int dem = 0;
    NodeDK* p = ltc.dsDK;

    while (p != nullptr) {
        if (!p->data.huyDangKy) dem++;
        p = p->next;
    }

    return dem;
}

int soChoTrong(const LopTinChi& ltc) {
    int choTrong = ltc.soSVMax - demSVDangKyHopLe(ltc);

    if (choTrong < 0) return 0;
    return choTrong;
}

bool lopTinChiDaThi(const LopTinChi& ltc) {
    NodeDK* p = ltc.dsDK;

    while (p != nullptr) {
        if (p->data.diem != CHUA_CO_DIEM &&
            diemHopLe(p->data.diem)) {
            return true;
        }

        p = p->next;
    }

    return false;
}

bool lopTinChiChoPhepDangKy(const LopTinChi& ltc) {
    if (ltc.huyLop) return false;
    if (lopTinChiDaThi(ltc)) return false;
    if (soChoTrong(ltc) <= 0) return false;

    return true;
}

bool themLopTinChi(
    DSLopTinChi& dsLTC,
    const NodeMonHoc* dsMonHoc,
    LopTinChi duLieuMoi,
    int& maLTCTiepTheo
) {
    if (dsLTC.soLuong >= MAX_LOP_TIN_CHI) return false;
    if (!thongTinLopTinChiHopLe(duLieuMoi)) return false;
    if (timMonHoc(dsMonHoc, duLieuMoi.maMH) == nullptr) return false;

    if (timLopTinChi(
        dsLTC,
        duLieuMoi.nienKhoa,
        duLieuMoi.hocKy,
        duLieuMoi.maMH,
        duLieuMoi.nhom
    ) != nullptr) {
        return false;
    }

    duLieuMoi.maLTC = maLTCTiepTheo;
    duLieuMoi.huyLop = false;
    duLieuMoi.dsDK = nullptr;

    LopTinChi* p = taoLopTinChi(duLieuMoi);

    dsLTC.ds[dsLTC.soLuong] = p;
    dsLTC.soLuong++;

    maLTCTiepTheo++;

    return true;
}

bool xoaLopTinChi(DSLopTinChi& dsLTC, int maLTC) {
    int viTri = -1;

    for (int i = 0; i < dsLTC.soLuong; i++) {
        if (dsLTC.ds[i] != nullptr &&
            dsLTC.ds[i]->maLTC == maLTC) {
            viTri = i;
            break;
        }
    }

    if (viTri == -1) return false;

    LopTinChi* p = dsLTC.ds[viTri];

    if (p->dsDK != nullptr) return false;

    delete p;

    for (int i = viTri; i < dsLTC.soLuong - 1; i++) {
        dsLTC.ds[i] = dsLTC.ds[i + 1];
    }

    dsLTC.soLuong--;
    dsLTC.ds[dsLTC.soLuong] = nullptr;

    return true;
}

bool suaLopTinChi(
    DSLopTinChi& dsLTC,
    const NodeMonHoc* dsMonHoc,
    int maLTC,
    const LopTinChi& duLieuMoi
) {
    LopTinChi* p = timLopTinChi(dsLTC, maLTC);

    if (p == nullptr) return false;
    if (!thongTinLopTinChiHopLe(duLieuMoi)) return false;
    if (timMonHoc(dsMonHoc, duLieuMoi.maMH) == nullptr) return false;

    const LopTinChi* trung = timLopTinChi(
        dsLTC,
        duLieuMoi.nienKhoa,
        duLieuMoi.hocKy,
        duLieuMoi.maMH,
        duLieuMoi.nhom
    );

    if (trung != nullptr && trung->maLTC != maLTC) {
        return false;
    }

    if (p->dsDK != nullptr) {
        if (soSanhChuoi(p->maMH, duLieuMoi.maMH) != 0 ||
            soSanhChuoi(p->nienKhoa, duLieuMoi.nienKhoa) != 0 ||
            p->hocKy != duLieuMoi.hocKy ||
            p->nhom != duLieuMoi.nhom) {
            return false;
        }

        if (duLieuMoi.soSVMax < demSVDangKyHopLe(*p)) {
            return false;
        }
    }

    p->soSVMin = duLieuMoi.soSVMin;
    p->soSVMax = duLieuMoi.soSVMax;
    p->huyLop = duLieuMoi.huyLop;

    if (p->dsDK == nullptr) {
        p->hocKy = duLieuMoi.hocKy;
        p->nhom = duLieuMoi.nhom;

        for (int i = 0; duLieuMoi.nienKhoa[i] != '\0'; i++) {
            p->nienKhoa[i] = duLieuMoi.nienKhoa[i];
            p->nienKhoa[i + 1] = '\0';
        }

        for (int i = 0; duLieuMoi.maMH[i] != '\0'; i++) {
            p->maMH[i] = duLieuMoi.maMH[i];
            p->maMH[i + 1] = '\0';
        }
    }

    return true;
}

void inDSLopTinChi(
    const DSLopTinChi& dsLTC,
    const NodeMonHoc* dsMonHoc
) {
    cout << left
         << setw(10) << "Ma LTC"
         << setw(12) << "Ma MH"
         << setw(30) << "Ten mon hoc"
         << setw(12) << "Nien khoa"
         << setw(8) << "Hoc ky"
         << setw(8) << "Nhom"
         << setw(10) << "Si so"
         << setw(12) << "Trang thai"
         << '\n';

    for (int i = 0; i < dsLTC.soLuong; i++) {
        const LopTinChi* p = dsLTC.ds[i];

        if (p == nullptr) continue;

        const NodeMonHoc* mh = timMonHoc(dsMonHoc, p->maMH);

        cout << left
             << setw(10) << p->maLTC
             << setw(12) << p->maMH
             << setw(30) << (mh == nullptr ? "Khong tim thay" : mh->data.tenMH)
             << setw(12) << p->nienKhoa
             << setw(8) << p->hocKy
             << setw(8) << p->nhom
             << setw(10) << demSVDangKyHopLe(*p)
             << setw(12) << (p->huyLop ? "Da huy" : "Dang mo")
             << '\n';
    }
}