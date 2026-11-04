#include "../include/KhaiBaoDoAn.h"
#include <iostream>

using namespace std;

static void saoChepChuoi(
    char* dich,
    const char* nguon,
    int kichThuoc
) {
    int i = 0;

    while (i < kichThuoc - 1 && nguon[i] != '\0') {
        dich[i] = nguon[i];
        i++;
    }

    dich[i] = '\0';
}

void inDSSVTheoMaSV(const Lop& lop) {
    cout << "Danh sach sinh vien lop: "
         << lop.maLop << '\n';

    cout << "Ma SV\tHo va ten\t\tPhai\tSo DT\n";

    const NodeSV* p = lop.dsSV;

    if (p == nullptr) {
        cout << "Lop chua co sinh vien!\n";
        return;
    }

    char maTruoc[MAX_MASV] = "";
    bool lanDau = true;

    while (true) {
        const NodeSV* nhoNhat = nullptr;
        p = lop.dsSV;

        while (p != nullptr) {
            bool duocChon = lanDau ||
                soSanhChuoi(p->data.maSV, maTruoc) > 0;

            if (duocChon) {
                if (nhoNhat == nullptr ||
                    soSanhChuoi(
                        p->data.maSV,
                        nhoNhat->data.maSV
                    ) < 0) {
                    nhoNhat = p;
                }
            }

            p = p->next;
        }

        if (nhoNhat == nullptr) break;

        cout << nhoNhat->data.maSV << "\t"
             << nhoNhat->data.ho << " "
             << nhoNhat->data.ten << "\t\t"
             << nhoNhat->data.phai << "\t"
             << nhoNhat->data.soDT << '\n';

        saoChepChuoi(maTruoc, nhoNhat->data.maSV, MAX_MASV);
        lanDau = false;
    }
}

void inDSSVTheoMaSV(
    const DSLop& dsLop,
    const char* maLop
) {
    const Lop* lop = timLop(dsLop, maLop);

    if (lop == nullptr) {
        cout << "Khong tim thay lop!\n";
        return;
    }

    inDSSVTheoMaSV(*lop);
}