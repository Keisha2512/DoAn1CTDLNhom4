/*
    CAU L - IN BANG DIEM TONG KET CAC MON CUA CAC SINH VIEN THUOC 1 LOP
    Module nghiep vu, khong chua main() va khong chua giao dien menu.

    Bang diem tong ket chi hien thi diem thi cao nhat tung mon cua sinh vien.
    Cac cot mon hoc duoc duyet LNR tren cay BST dsMonHoc de dam bao thu tu
    tang dan theo MAMH.
*/

#include "../include/KhaiBaoDoAn.h"
#include <iostream>
#include <iomanip>
#include <cstring>

using std::cout;
using std::left;
using std::right;
using std::setw;

// Ham phu: dem tong so mon hoc tren cay BST de ke duong phan cach bang
static int demSoMonHoc(const NodeMonHoc* root) {
    if (root == nullptr) return 0;
    return 1 + demSoMonHoc(root->left) + demSoMonHoc(root->right);
}

// Ham phu: duyet LNR de in tieu de cac cot Ma MH tang dan theo MAMH
static void inTieuDeMonHocLNR(const NodeMonHoc* root) {
    if (root == nullptr) return;
    inTieuDeMonHocLNR(root->left);
    cout << right << setw(12) << root->data.maMH;
    inTieuDeMonHocLNR(root->right);
}

// Ham phu: duyet LNR de in diem cao nhat cua sinh vien ung voi tung mon hoc
static void inDiemTungMonCuaSVLNR(const NodeMonHoc* root,
                                  const DSLopTinChi& dsLTC,
                                  const char* maSV) {
    if (root == nullptr) return;

    inDiemTungMonCuaSVLNR(root->left, dsLTC, maSV);

    float diemMax = diemCaoNhatMonCuaSV(dsLTC, maSV, root->data.maMH);
    if (diemHopLe(diemMax)) {
        cout << right << setw(12) << std::fixed << std::setprecision(2)
             << diemMax;
    } else {
        cout << right << setw(12) << "Chua co";
    }

    inDiemTungMonCuaSVLNR(root->right, dsLTC, maSV);
}

// Ham phu: ghep HO va TEN thanh 1 chuoi "Ho Ten" de in dung 1 cot theo mau cau l
static void ghepHoTen(const char* ho, const char* ten,
                      char* hoTen, std::size_t maxLen) {
    if (maxLen == 0) return;
    hoTen[0] = '\0';

    if (ho != nullptr && ho[0] != '\0') {
        std::strncat(hoTen, ho, maxLen - 1);
    }
    if (ten != nullptr && ten[0] != '\0') {
        if (hoTen[0] != '\0' && std::strlen(hoTen) + 1 < maxLen) {
            std::strcat(hoTen, " ");
        }
        std::strncat(hoTen, ten, maxLen - std::strlen(hoTen) - 1);
    }
}

void inBangDiemTongKet(const Lop& lop, const DSLopTinChi& dsLTC,
                       const NodeMonHoc* dsMonHoc) {
    cout << "\n                    BANG DIEM TONG KET\n";
    cout << "                    Lop : " << lop.maLop << "\n\n";

    cout << left << setw(5) << "STT"
         << setw(18) << "Ma SV"
         << setw(35) << "Ho Ten";
    inTieuDeMonHocLNR(dsMonHoc);
    cout << '\n';

    int soMon = demSoMonHoc(dsMonHoc);
    int doRongBang = 5 + 18 + 35 + soMon * 12;
    for (int i = 0; i < doRongBang; ++i) {
        cout << '-';
    }
    cout << '\n';

    int stt = 0;
    const NodeSV* sv = lop.dsSV;
    while (sv != nullptr) {
        ++stt;
        char hoTen[MAX_HO + MAX_TEN + 2];
        ghepHoTen(sv->data.ho, sv->data.ten, hoTen, sizeof(hoTen));

        cout << left << setw(5) << stt
             << setw(18) << sv->data.maSV
             << setw(35) << hoTen;

        inDiemTungMonCuaSVLNR(dsMonHoc, dsLTC, sv->data.maSV);
        cout << '\n';

        sv = sv->next;
    }

    if (stt == 0) {
        cout << "Lop chua co sinh vien.\n";
    }
}

void inBangDiemTongKet(const DSLop& dsLop, const DSLopTinChi& dsLTC,
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

    inBangDiemTongKet(*lop, dsLTC, dsMonHoc);
}
