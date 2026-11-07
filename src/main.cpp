#include "../include/KhaiBaoDoAn.h"

#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <cstring>

using namespace std;

static const char* TEN_FILE_DU_LIEU = "du_lieu.dat";

static string nhapChuoi(const string& nhan) {
    string s;
    cout << nhan;
    getline(cin, s);
    return s;
}

static int nhapSoNguyen(const string& nhan, int minValue, int maxValue) {
    while (true) {
        string s = nhapChuoi(nhan);
        try {
            size_t pos = 0;
            long long v = stoll(s, &pos);
            if (pos == s.size() && v >= minValue && v <= maxValue)
                return static_cast<int>(v);
        } catch (...) {}
        cout << "Gia tri khong hop le. Vui long nhap lai.\n";
    }
}

static float nhapDiem() {
    while (true) {
        string s = nhapChuoi("Nhap diem (0-10): ");
        try {
            size_t pos = 0;
            float v = stof(s, &pos);
            if (pos == s.size() && diemHopLe(v)) return v;
        } catch (...) {}
        cout << "Diem phai nam trong [0, 10].\n";
    }
}

static void luuSauThayDoi(const HeThong& ht) {
    if (!luuDuLieuKhiThoatChucNang(TEN_FILE_DU_LIEU, ht))
        cout << "Canh bao: chua luu duoc du lieu vao " << TEN_FILE_DU_LIEU << ".\n";
}

static void nhapMonHoc(HeThong& ht) {
    MonHoc mh{};
    string ma = nhapChuoi("Ma mon hoc: ");
    string ten = nhapChuoi("Ten mon hoc: ");
    if (ma.size() >= MAX_MAMH || ten.size() >= MAX_TENMH) {
        cout << "Ma/ten mon hoc vuot qua do dai cho phep.\n"; return;
    }
    copy(ma.begin(), ma.end(), mh.maMH);
    copy(ten.begin(), ten.end(), mh.tenMH);
    mh.soTCLyThuyet = nhapSoNguyen("So tin chi ly thuyet: ", 0, 100);
    mh.soTCThucHanh = nhapSoNguyen("So tin chi thuc hanh: ", 0, 100);
    if (themMonHoc(ht.dsMonHoc, mh)) {
        cout << "Them mon hoc thanh cong.\n"; luuSauThayDoi(ht);
    } else cout << "Them that bai (du lieu khong hop le hoac trung ma).\n";
}

static void suaMonHocMenu(HeThong& ht) {
    string ma = nhapChuoi("Nhap ma mon hoc can sua: ");
    NodeMonHoc* cu = timMonHoc(ht.dsMonHoc, ma.c_str());
    if (!cu) { cout << "Khong tim thay mon hoc.\n"; return; }
    MonHoc moi{};
    string maMoi = nhapChuoi("Ma mon hoc moi: ");
    string tenMoi = nhapChuoi("Ten mon hoc moi: ");
    if (maMoi.size() >= MAX_MAMH || tenMoi.size() >= MAX_TENMH) {
        cout << "Ma/ten vuot qua do dai cho phep.\n"; return;
    }
    copy(maMoi.begin(), maMoi.end(), moi.maMH);
    copy(tenMoi.begin(), tenMoi.end(), moi.tenMH);
    moi.soTCLyThuyet = nhapSoNguyen("So TC ly thuyet: ", 0, 100);
    moi.soTCThucHanh = nhapSoNguyen("So TC thuc hanh: ", 0, 100);
    if (soSanhChuoi(ma.c_str(), moi.maMH) != 0 &&
        monHocDaCoLopTinChi(ht.dsLopTinChi, ma.c_str())) {
        cout << "Khong the doi ma mon hoc vi da co lop tin chi su dung ma nay.\n";
        return;
    }
    if (suaMonHoc(ht.dsMonHoc, ma.c_str(), moi)) {
        cout << "Cap nhat mon hoc thanh cong.\n"; luuSauThayDoi(ht);
    } else cout << "Cap nhat that bai.\n";
}

static void xoaMonHocMenu(HeThong& ht) {
    string ma = nhapChuoi("Nhap ma mon hoc can xoa: ");
    if (monHocDaCoLopTinChi(ht.dsLopTinChi, ma.c_str())) {
        cout << "Khong the xoa mon hoc da duoc tao lop tin chi.\n"; return;
    }
    if (xoaMonHoc(ht.dsMonHoc, ma.c_str())) {
        cout << "Da xoa mon hoc.\n"; luuSauThayDoi(ht);
    } else cout << "Khong tim thay mon hoc.\n";
}

static void menuMonHoc(HeThong& ht) {
    while (true) {
        cout << "\n===== A/F - QUAN LY MON HOC =====\n"
             << "1. Them mon hoc\n2. Sua mon hoc\n3. Xoa mon hoc\n"
             << "4. In danh sach tang dan theo ten\n5. In danh sach theo ma\n0. Quay lai\n";
        int c = nhapSoNguyen("Chon: ", 0, 5);
        if (c == 0) return;
        if (c == 1) nhapMonHoc(ht);
        if (c == 2) suaMonHocMenu(ht);
        if (c == 3) xoaMonHocMenu(ht);
        if (c == 4) inDSMonHocTangTheoTen(ht.dsMonHoc);
        if (c == 5) inMonHocTheoMa(ht.dsMonHoc);
    }
}

static void menuLopVaSinhVien(HeThong& ht) {
    while (true) {
        cout << "\n===== QUAN LY LOP / SINH VIEN =====\n"
             << "1. Them lop\n2. Sua thong tin lop\n3. Xoa lop rong\n"
             << "4. Them sinh vien vao lop\n5. Sua sinh vien\n6. Xoa sinh vien\n"
             << "7. In sinh vien theo ten\n8. In sinh vien theo ma SV\n0. Quay lai\n";
        int c = nhapSoNguyen("Chon: ", 0, 8);
        if (c == 0) return;
        if (c == 1) {
            Lop l{}; string ma = nhapChuoi("Ma lop: "), ten = nhapChuoi("Ten lop: ");
            if (ma.size() >= MAX_MALOP || ten.size() >= MAX_TENLOP) { cout << "Du lieu qua dai.\n"; continue; }
            copy(ma.begin(), ma.end(), l.maLop); copy(ten.begin(), ten.end(), l.tenLop);
            l.dsSV = nullptr;
            if (themLop(ht.dsLop, l)) { cout << "Them lop thanh cong.\n"; luuSauThayDoi(ht); }
            else cout << "Them lop that bai.\n";
        } else if (c == 2) {
            string ma = nhapChuoi("Ma lop can sua: ");
            Lop* old = timLop(ht.dsLop, ma.c_str());
            if (!old) { cout << "Khong tim thay lop.\n"; continue; }
            Lop moi{}; string maMoi = nhapChuoi("Ma lop moi: "), tenMoi = nhapChuoi("Ten lop moi: ");
            if (maMoi.size() >= MAX_MALOP || tenMoi.size() >= MAX_TENLOP) { cout << "Du lieu qua dai.\n"; continue; }
            copy(maMoi.begin(), maMoi.end(), moi.maLop); copy(tenMoi.begin(), tenMoi.end(), moi.tenLop);
            if (suaLop(ht.dsLop, ma.c_str(), moi)) { cout << "Da sua lop.\n"; luuSauThayDoi(ht); }
            else cout << "Sua lop that bai.\n";
        } else if (c == 3) {
            string ma = nhapChuoi("Ma lop can xoa: ");
            if (xoaLop(ht.dsLop, ma.c_str())) { cout << "Da xoa lop.\n"; luuSauThayDoi(ht); }
            else cout << "Khong the xoa lop (khong ton tai hoac con sinh vien).\n";
        } else if (c == 4) {
            string maLop = nhapChuoi("Ma lop: ");
            Lop* l = timLop(ht.dsLop, maLop.c_str());
            if (!l) { cout << "Khong tim thay lop.\n"; continue; }
            SinhVien sv{}; string ma = nhapChuoi("Ma SV: "), ho = nhapChuoi("Ho: ");
            string ten = nhapChuoi("Ten: "), phai = nhapChuoi("Phai: "), sdt = nhapChuoi("So dien thoai: ");
            if (ma.size() >= MAX_MASV || ho.size() >= MAX_HO || ten.size() >= MAX_TEN ||
                phai.size() >= MAX_PHAI || sdt.size() >= MAX_SODT) { cout << "Du lieu qua dai.\n"; continue; }
            copy(ma.begin(), ma.end(), sv.maSV); copy(ho.begin(), ho.end(), sv.ho);
            copy(ten.begin(), ten.end(), sv.ten); copy(phai.begin(), phai.end(), sv.phai);
            copy(sdt.begin(), sdt.end(), sv.soDT);
            if (themSVVaoLop(ht.dsLop, maLop.c_str(), sv)) { cout << "Da them sinh vien.\n"; luuSauThayDoi(ht); }
            else cout << "Them sinh vien that bai (ma trung/du lieu sai).\n";
        } else if (c == 5) {
            string maLop = nhapChuoi("Ma lop: "), maSV = nhapChuoi("Ma SV can sua: ");
            SinhVien sv{}; string maMoi = nhapChuoi("Ma SV moi (giu nguyen neu khong doi): ");
            string ho = nhapChuoi("Ho moi: "), ten = nhapChuoi("Ten moi: ");
            string phai = nhapChuoi("Phai moi: "), sdt = nhapChuoi("So dien thoai moi: ");
            if (maMoi.size() >= MAX_MASV || ho.size() >= MAX_HO || ten.size() >= MAX_TEN ||
                phai.size() >= MAX_PHAI || sdt.size() >= MAX_SODT) { cout << "Du lieu qua dai.\n"; continue; }
            copy(maMoi.begin(), maMoi.end(), sv.maSV); copy(ho.begin(), ho.end(), sv.ho);
            copy(ten.begin(), ten.end(), sv.ten); copy(phai.begin(), phai.end(), sv.phai);
            copy(sdt.begin(), sdt.end(), sv.soDT);
            if (suaSVTrongLop(ht.dsLop, ht.dsLopTinChi, maLop.c_str(), maSV.c_str(), sv)) {
                cout << "Da cap nhat sinh vien.\n"; luuSauThayDoi(ht);
            } else cout << "Sua sinh vien that bai (kiem tra ma trung/lich su dang ky).\n";
        } else if (c == 6) {
            string maLop = nhapChuoi("Ma lop: "), maSV = nhapChuoi("Ma SV can xoa: ");
            if (xoaSVKhoiLop(ht.dsLop, ht.dsLopTinChi, maLop.c_str(), maSV.c_str())) {
                cout << "Da xoa sinh vien.\n"; luuSauThayDoi(ht);
            } else cout << "Khong the xoa (khong ton tai hoac con dang ky chua huy).\n";
        } else if (c == 7 || c == 8) {
            string maLop = nhapChuoi("Ma lop: "); const Lop* l = timLop(ht.dsLop, maLop.c_str());
            if (!l) cout << "Khong tim thay lop.\n";
            else if (c == 7) inDSSVTheoTen(*l); else inDSSVTheoMaSV(*l);
        }
    }
}

static void menuLopTinChi(HeThong& ht) {
    while (true) {
        cout << "\n===== A - QUAN LY LOP TIN CHI =====\n"
             << "1. Them lop tin chi\n2. Sua lop tin chi\n3. Xoa lop tin chi rong\n"
             << "4. In danh sach lop tin chi\n5. Tim theo ma LTC\n"
             << "6. Tim theo ma mon hoc\n7. Tim theo ten mon hoc\n0. Quay lai\n";
        int c = nhapSoNguyen("Chon: ", 0, 7);
        if (c == 0) return;
        if (c == 1) {
            LopTinChi l{}; string maMH = nhapChuoi("Ma mon hoc: ");
            string nk = nhapChuoi("Nien khoa (VD 2025-2026): ");
            if (maMH.size() >= MAX_MAMH || nk.size() >= MAX_NIEN_KHOA) { cout << "Du lieu qua dai.\n"; continue; }
            copy(maMH.begin(), maMH.end(), l.maMH); copy(nk.begin(), nk.end(), l.nienKhoa);
            l.hocKy = nhapSoNguyen("Hoc ky: ", 1, 3);
            l.nhom = nhapSoNguyen("Nhom: ", 1, 10000);
            l.soSVMin = nhapSoNguyen("Si so toi thieu: ", 0, 10000);
            l.soSVMax = nhapSoNguyen("Si so toi da: ", 1, 10000);
            l.huyLop = false; l.dsDK = nullptr;
            if (themLopTinChi(ht.dsLopTinChi, ht.dsMonHoc, l, ht.maLTCTiepTheo)) {
                cout << "Da tao lop tin chi, ma LTC = " << ht.maLTCTiepTheo - 1 << ".\n"; luuSauThayDoi(ht);
            } else cout << "Tao lop tin chi that bai.\n";
        } else if (c == 2) {
            int id = nhapSoNguyen("Ma LTC can sua: ", 1, 1000000000);
            LopTinChi* old = timLopTinChi(ht.dsLopTinChi, id);
            if (!old) { cout << "Khong tim thay.\n"; continue; }
            LopTinChi moi = *old;
            string maMH = nhapChuoi("Ma MH moi: "), nk = nhapChuoi("Nien khoa moi: ");
            if (maMH.size() >= MAX_MAMH || nk.size() >= MAX_NIEN_KHOA) { cout << "Du lieu qua dai.\n"; continue; }
            std::strncpy(moi.maMH, maMH.c_str(), MAX_MAMH - 1); moi.maMH[MAX_MAMH - 1] = '\0';
            std::strncpy(moi.nienKhoa, nk.c_str(), MAX_NIEN_KHOA - 1); moi.nienKhoa[MAX_NIEN_KHOA - 1] = '\0';
            moi.hocKy = nhapSoNguyen("Hoc ky: ", 1, 3); moi.nhom = nhapSoNguyen("Nhom: ", 1, 10000);
            moi.soSVMin = nhapSoNguyen("Si so toi thieu: ", 0, 10000);
            moi.soSVMax = nhapSoNguyen("Si so toi da: ", 1, 10000);
            if (suaLopTinChi(ht.dsLopTinChi, ht.dsMonHoc, id, moi)) { cout << "Da sua lop tin chi.\n"; luuSauThayDoi(ht); }
            else cout << "Sua that bai (lop co dang ky co the khong duoc doi khoa).\n";
        } else if (c == 3) {
            int id = nhapSoNguyen("Ma LTC can xoa: ", 1, 1000000000);
            if (xoaLopTinChi(ht.dsLopTinChi, id)) { cout << "Da xoa lop tin chi rong.\n"; luuSauThayDoi(ht); }
            else cout << "Khong the xoa (khong ton tai hoac da co lich su dang ky).\n";
        } else if (c == 4) inDSLopTinChi(ht.dsLopTinChi, ht.dsMonHoc);
        else if (c >= 5 && c <= 7) {
            string key = nhapChuoi("Tu khoa: ");
            KieuTimKiemLTC k = c == 5 ? KieuTimKiemLTC::THEO_MALTC :
                              c == 6 ? KieuTimKiemLTC::THEO_MAMH : KieuTimKiemLTC::THEO_TENMH;
            timKiemLocLopTinChi(ht.dsLopTinChi, ht.dsMonHoc, k, key.c_str());
        }
    }
}

static void menuDangKy(HeThong& ht) {
    while (true) {
        cout << "\n===== B/G - DANG KY LOP TIN CHI =====\n"
             << "1. Xem lop tin chi mo theo hoc ky\n2. Dang ky\n"
             << "3. Huy dang ky\n4. In danh sach dang ky cua lop\n"
             << "0. Quay lai\n";
        int c = nhapSoNguyen("Chon: ", 0, 4);
        if (!c) return;
        if (c == 1) {
            string nk = nhapChuoi("Nien khoa: "); int hk = nhapSoNguyen("Hoc ky: ", 1, 3);
            inLopTinChiMoTheoHocKy(ht.dsMonHoc, ht.dsLopTinChi, nk.c_str(), hk);
        } else if (c == 2) {
            string maSV = nhapChuoi("Ma SV: "); int id = nhapSoNguyen("Ma LTC: ", 1, 1000000000);
            if (dangKyLopTinChi(ht.dsLop, ht.dsLopTinChi, maSV.c_str(), id)) {
                cout << "Dang ky thanh cong.\n"; luuSauThayDoi(ht);
            } else cout << "Dang ky that bai (kiem tra sinh vien, lop, si so, mon hoc).\n";
        } else if (c == 3) {
            string maSV = nhapChuoi("Ma SV: "); int id = nhapSoNguyen("Ma LTC: ", 1, 1000000000);
            if (huyDangKyLopTinChi(ht.dsLopTinChi, maSV.c_str(), id)) {
                cout << "Da huy dang ky.\n"; luuSauThayDoi(ht);
            } else cout << "Huy dang ky that bai.\n";
        } else if (c == 4) {
            int id = nhapSoNguyen("Ma LTC: ", 1, 1000000000);
            const LopTinChi* ltc = timLopTinChi(ht.dsLopTinChi, id);
            if (ltc) inDSDangKyLopTinChi(ht.dsLop, *ltc);
            else cout << "Khong tim thay lop tin chi.\n";
        }
    }
}

static void menuHuyLop(HeThong& ht) {
    string nk = nhapChuoi("Nien khoa: ");
    int hk = nhapSoNguyen("Hoc ky: ", 1, 3);
    int duKien = 0;
    for (int i = 0; i < ht.dsLopTinChi.soLuong; ++i) {
        const LopTinChi* p = ht.dsLopTinChi.ds[i];
        if (p && !p->huyLop && soSanhChuoi(p->nienKhoa, nk.c_str()) == 0 &&
            p->hocKy == hk && demSVDangKyHopLe(*p) < p->soSVMin) {
            cout << "LTC " << p->maLTC << " - " << p->maMH
                 << " - si so " << demSVDangKyHopLe(*p)
                 << "/" << p->soSVMin << '\n';
            ++duKien;
        }
    }
    if (!duKien) { cout << "Khong co lop nao can huy.\n"; return; }
    string xacNhan = nhapChuoi("Xac nhan huy cac lop tren? (Y/N): ");
    if (xacNhan == "Y" || xacNhan == "y") {
        int n = huyCacLopKhongDuSiSo(ht.dsLopTinChi, nk.c_str(), hk);
        cout << "Da huy " << n << " lop tin chi.\n"; luuSauThayDoi(ht);
    } else cout << "Da huy thao tac.\n";
}

static void menuNhapDiem(HeThong& ht) {
    int id = nhapSoNguyen("Ma LTC: ", 1, 1000000000);
    LopTinChi* ltc = timLopTinChi(ht.dsLopTinChi, id);
    if (!ltc) { cout << "Khong tim thay lop tin chi.\n"; return; }
    inBangNhapDiem(ht.dsLop, *ltc);
    while (true) {
        string maSV = nhapChuoi("Ma SV can nhap/sua diem (Enter de ket thuc): ");
        if (maSV.empty()) break;
        float diem = nhapDiem();
        if (capNhatDiem(*ltc, maSV.c_str(), diem)) {
            cout << "Da cap nhat diem.\n"; luuSauThayDoi(ht);
        } else cout << "Cap nhat diem that bai.\n";
    }
}

static void menuBaoCao(HeThong& ht) {
    while (true) {
        cout << "\n===== J/K/L - BAO CAO DIEM =====\n"
             << "1. J - Bang diem mon hoc cua mot LTC\n"
             << "2. K - Bang diem trung binh cua lop\n"
             << "3. L - Bang diem tong ket cua lop\n0. Quay lai\n";
        int c = nhapSoNguyen("Chon: ", 0, 3);
        if (!c) return;
        if (c == 1) {
            int id = nhapSoNguyen("Ma LTC: ", 1, 1000000000);
            const LopTinChi* ltc = timLopTinChi(ht.dsLopTinChi, id);
            if (ltc) inBangDiemMonHoc(ht.dsLop, ht.dsMonHoc, *ltc);
            else cout << "Khong tim thay lop tin chi.\n";
        } else {
            string maLop = nhapChuoi("Ma lop: ");
            if (c == 2) inBangDTBKetThucKhoa(ht.dsLop, ht.dsLopTinChi, ht.dsMonHoc, maLop.c_str());
            else inBangDiemTongKet(ht.dsLop, ht.dsLopTinChi, ht.dsMonHoc, maLop.c_str());
        }
    }
}

int main() {
    HeThong heThong{};
    khoiTaoHeThong(heThong);
    if (taiDuLieu(TEN_FILE_DU_LIEU, heThong))
        cout << "Da nap du lieu tu " << TEN_FILE_DU_LIEU << ".\n";
    else
        cout << "Chua co tep du lieu hop le. Bat dau voi he thong rong.\n";

    while (true) {
        cout << "\n===============================================\n"
             << "     HE THONG QUAN LY DIEM THEO TIN CHI\n"
             << "===============================================\n"
             << "1. A/F - Quan ly mon hoc\n"
             << "2. Quan ly lop va sinh vien (C/D)\n"
             << "3. A - Quan ly lop tin chi\n"
             << "4. B/G - Dang ky / huy dang ky lop tin chi\n"
             << "5. H - Huy lop khong du si so\n"
             << "6. I - Nhap / hieu chinh diem\n"
             << "7. J/K/L - Cac bang diem va diem trung binh\n"
             << "8. Luu du lieu\n"
             << "9. Kiem tra toan ven du lieu\n"
             << "0. Luu va thoat\n";
        int c = nhapSoNguyen("Chon chuc nang: ", 0, 9);
        if (c == 0) {
            luuSauThayDoi(heThong);
            break;
        }
        switch (c) {
            case 1: menuMonHoc(heThong); break;
            case 2: menuLopVaSinhVien(heThong); break;
            case 3: menuLopTinChi(heThong); break;
            case 4: menuDangKy(heThong); break;
            case 5: menuHuyLop(heThong); break;
            case 6: menuNhapDiem(heThong); break;
            case 7: menuBaoCao(heThong); break;
            case 8:
                if (luuDuLieu(TEN_FILE_DU_LIEU, heThong)) cout << "Da luu du lieu.\n";
                else cout << "Luu du lieu that bai.\n";
                break;
            case 9:
                cout << (kiemTraToanVenDuLieu(heThong) ? "Du lieu hop le.\n" : "Phat hien du lieu khong toan ven.\n");
                break;
        }
    }

    giaiPhongHeThong(heThong);
    return 0;
}