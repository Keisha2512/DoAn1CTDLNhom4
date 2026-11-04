#include "../include/KhaiBaoDoAn.h"
#include <iostream>
#include <iomanip>

using namespace std;

static void timMonHocTiepTheo(
    const NodeMonHoc* root,
    const char* tenTruoc,
    const char* maTruoc,
    bool lanDau,
    const NodeMonHoc*& ketQua
) {
    if (root == nullptr) return;

    const char* ten = root->data.tenMH;
    const char* ma = root->data.maMH;

    bool lonHon = lanDau;

    if (!lanDau) {
        int kq = soSanhChuoi(ten, tenTruoc);

        if (kq > 0) {
            lonHon = true;
        } else if (kq == 0 &&
                   soSanhChuoi(ma, maTruoc) > 0) {
            lonHon = true;
        }
    }

    if (lonHon) {
        if (ketQua == nullptr) {
            ketQua = root;
        } else {
            int kqTen = soSanhChuoi(
                ten, ketQua->data.tenMH
            );

            if (kqTen < 0 ||
                (kqTen == 0 &&
                 soSanhChuoi(ma, ketQua->data.maMH) < 0)) {
                ketQua = root;
            }
        }
    }

    timMonHocTiepTheo(
        root->left,
        tenTruoc,
        maTruoc,
        lanDau,
        ketQua
    );

    timMonHocTiepTheo(
        root->right,
        tenTruoc,
        maTruoc,
        lanDau,
        ketQua
    );
}

void inDSMonHocTangTheoTen(const NodeMonHoc* root) {
    if (root == nullptr) {
        cout << "Danh sach mon hoc rong!\n";
        return;
    }

    cout << left
         << setw(15) << "Ma mon hoc"
         << setw(35) << "Ten mon hoc"
         << setw(12) << "TC ly thuyet"
         << setw(12) << "TC thuc hanh"
         << '\n';

    char tenTruoc[MAX_TENMH] = "";
    char maTruoc[MAX_MAMH] = "";

    bool lanDau = true;

    while (true) {
        const NodeMonHoc* ketQua = nullptr;

        timMonHocTiepTheo(
            root,
            tenTruoc,
            maTruoc,
            lanDau,
            ketQua
        );

        if (ketQua == nullptr) break;

        cout << left
             << setw(15) << ketQua->data.maMH
             << setw(35) << ketQua->data.tenMH
             << setw(12) << ketQua->data.soTCLyThuyet
             << setw(12) << ketQua->data.soTCThucHanh
             << '\n';

        int i = 0;

        while (ketQua->data.tenMH[i] != '\0') {
            tenTruoc[i] = ketQua->data.tenMH[i];
            i++;
        }
        tenTruoc[i] = '\0';

        i = 0;

        while (ketQua->data.maMH[i] != '\0') {
            maTruoc[i] = ketQua->data.maMH[i];
            i++;
        }
        maTruoc[i] = '\0';

        lanDau = false;
    }
}

void inMonHocTheoMa(
    const NodeMonHoc* root
) {
    if (root == nullptr) return;

    inMonHocTheoMa(root->left);

    cout << "Ma mon hoc: "
         << root->data.maMH << '\n';

    cout << "Ten mon hoc: "
         << root->data.tenMH << '\n';

    cout << "Tin chi ly thuyet: "
         << root->data.soTCLyThuyet << '\n';

    cout << "Tin chi thuc hanh: "
         << root->data.soTCThucHanh << '\n';

    cout << "-----------------------------\n";

    inMonHocTheoMa(root->right);
}