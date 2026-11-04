#include "../include/KhaiBaoDoAn.h"
#include <iostream>

using namespace std;

NodeSV* taoNodeSV(const SinhVien& sinhVien) {
    NodeSV* p = new NodeSV;
    p->data = sinhVien;
    p->next = nullptr;
    return p;
}

NodeSV* timSVTrongLop(NodeSV* head, const char* maSV) {
    NodeSV* p = head;

    while (p != nullptr) {
        if (soSanhChuoi(p->data.maSV, maSV) == 0) {
            return p;
        }

        p = p->next;
    }

    return nullptr;
}

const NodeSV* timSVTrongLop(
    const NodeSV* head,
    const char* maSV
) {
    const NodeSV* p = head;

    while (p != nullptr) {
        if (soSanhChuoi(p->data.maSV, maSV) == 0) {
            return p;
        }

        p = p->next;
    }

    return nullptr;
}

NodeSV* timSV(
    DSLop& dsLop,
    const char* maSV,
    Lop** lopChuaSV
) {
    for (int i = 0; i < dsLop.soLuong; i++) {
        NodeSV* p = timSVTrongLop(dsLop.ds[i].dsSV, maSV);

        if (p != nullptr) {
            if (lopChuaSV != nullptr) {
                *lopChuaSV = &dsLop.ds[i];
            }

            return p;
        }
    }

    if (lopChuaSV != nullptr) *lopChuaSV = nullptr;
    return nullptr;
}

const NodeSV* timSV(
    const DSLop& dsLop,
    const char* maSV,
    const Lop** lopChuaSV
) {
    for (int i = 0; i < dsLop.soLuong; i++) {
        const NodeSV* p = timSVTrongLop(dsLop.ds[i].dsSV, maSV);

        if (p != nullptr) {
            if (lopChuaSV != nullptr) {
                *lopChuaSV = &dsLop.ds[i];
            }

            return p;
        }
    }

    if (lopChuaSV != nullptr) *lopChuaSV = nullptr;
    return nullptr;
}

void chenSVTheoTen(NodeSV*& head, const SinhVien& sinhVien) {
    NodeSV* moi = taoNodeSV(sinhVien);

    if (head == nullptr) {
        head = moi;
        return;
    }

    if (soSanhChuoi(sinhVien.ten, head->data.ten) < 0 ||
        (soSanhChuoi(sinhVien.ten, head->data.ten) == 0 &&
         soSanhChuoi(sinhVien.ho, head->data.ho) < 0) ||
        (soSanhChuoi(sinhVien.ten, head->data.ten) == 0 &&
         soSanhChuoi(sinhVien.ho, head->data.ho) == 0 &&
         soSanhChuoi(sinhVien.maSV, head->data.maSV) < 0)) {
        moi->next = head;
        head = moi;
        return;
    }

    NodeSV* p = head;

    while (p->next != nullptr) {
        int kqTen = soSanhChuoi(sinhVien.ten, p->next->data.ten);
        int kqHo = soSanhChuoi(sinhVien.ho, p->next->data.ho);
        int kqMa = soSanhChuoi(sinhVien.maSV, p->next->data.maSV);

        if (kqTen < 0 ||
            (kqTen == 0 && kqHo < 0) ||
            (kqTen == 0 && kqHo == 0 && kqMa < 0)) {
            break;
        }

        p = p->next;
    }

    moi->next = p->next;
    p->next = moi;
}

bool themSV(Lop& lop, const SinhVien& sinhVien) {
    if (!thongTinSinhVienHopLe(sinhVien)) return false;

    if (timSVTrongLop(lop.dsSV, sinhVien.maSV) != nullptr) {
        return false;
    }

    chenSVTheoTen(lop.dsSV, sinhVien);
    return true;
}

bool xoaSV(Lop& lop, const char* maSV) {
    NodeSV* p = lop.dsSV;
    NodeSV* truoc = nullptr;

    while (p != nullptr &&
           soSanhChuoi(p->data.maSV, maSV) != 0) {
        truoc = p;
        p = p->next;
    }

    if (p == nullptr) return false;

    if (truoc == nullptr) {
        lop.dsSV = p->next;
    } else {
        truoc->next = p->next;
    }

    delete p;
    return true;
}

bool suaSV(
    Lop& lop,
    const char* maSV,
    const SinhVien& sinhVienMoi
) {
    if (!thongTinSinhVienHopLe(sinhVienMoi)) return false;

    NodeSV* cu = timSVTrongLop(lop.dsSV, maSV);

    if (cu == nullptr) return false;

    NodeSV* trung = timSVTrongLop(
        lop.dsSV, sinhVienMoi.maSV
    );

    if (trung != nullptr && trung != cu) return false;

    xoaSV(lop, maSV);
    chenSVTheoTen(lop.dsSV, sinhVienMoi);

    return true;
}

bool themSVVaoLop(
    DSLop& dsLop,
    const char* maLop,
    const SinhVien& sinhVien
) {
    Lop* lop = timLop(dsLop, maLop);

    if (lop == nullptr) return false;
    if (timSV(dsLop, sinhVien.maSV) != nullptr) return false;

    return themSV(*lop, sinhVien);
}

bool xoaSVKhoiLop(
    DSLop& dsLop,
    const char* maLop,
    const char* maSV
) {
    Lop* lop = timLop(dsLop, maLop);

    if (lop == nullptr) return false;
    if (timSVTrongLop(lop->dsSV, maSV) == nullptr) return false;

    return xoaSV(*lop, maSV);
}

bool suaSVTrongLop(
    DSLop& dsLop,
    const char* maLop,
    const char* maSV,
    const SinhVien& sinhVienMoi
) {
    Lop* lop = timLop(dsLop, maLop);

    if (lop == nullptr) return false;
    if (!thongTinSinhVienHopLe(sinhVienMoi)) return false;

    NodeSV* cu = timSVTrongLop(lop->dsSV, maSV);

    if (cu == nullptr) return false;

    NodeSV* trung = timSV(dsLop, sinhVienMoi.maSV);

    if (trung != nullptr && trung != cu) return false;

    if (soSanhChuoi(maSV, sinhVienMoi.maSV) != 0 &&
        sinhVienCoDangKyChuaHuy(
            *(DSLopTinChi*)nullptr, maSV
        )) {
        return false;
    }

    return suaSV(*lop, maSV, sinhVienMoi);
}