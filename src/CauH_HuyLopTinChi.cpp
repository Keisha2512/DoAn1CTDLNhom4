/*
    CAU H - TU DONG HUY CAC LOP TIN CHI KHONG DU SI SO TOI THIEU
    Module nghiep vu, khong chua main() va khong chua giao dien menu.

    Quy trinh dung:
    1. Phia main/UI nhap nien khoa va hoc ky.
    2. Hien thi danh sach lop sap bi huy de nguoi dung xem.
    3. Hoi xac nhan nguoi dung.
    4. Chi khi nguoi dung dong y moi goi huyCacLopKhongDuSiSo().

    Ham duoi day chi thay doi du lieu, khong tu hoi xac nhan.
*/

#include "../include/KhaiBaoDoAn.h"
#include <cstring>

int huyCacLopKhongDuSiSo(DSLopTinChi& dsLTC,
                          const char* nienKhoa, int hocKy) {
    if (nienKhoa == nullptr || nienKhoa[0] == '\0' || hocKy <= 0)
        return 0;

    int soLopDaHuy = 0;
    for (int i = 0; i < dsLTC.soLuong; ++i) {
        LopTinChi* ltc = dsLTC.ds[i];
        if (ltc == nullptr || ltc->huyLop) continue;
        if (std::strcmp(ltc->nienKhoa, nienKhoa) != 0 || ltc->hocKy != hocKy)
            continue;

        int soSVThucDangKy = 0;
        NodeDK* p = ltc->dsDK;
        while (p != nullptr) {
            if (!p->data.huyDangKy) ++soSVThucDangKy;
            p = p->next;
        }

        // Theo de bai: huy lop neu so SV dang ky < soSVMin.
        if (soSVThucDangKy < ltc->soSVMin) {
            ltc->huyLop = true;
            ++soLopDaHuy;
        }
    }
    return soLopDaHuy;
}
