#ifndef QUAN_LY_DIEM_H
#define QUAN_LY_DIEM_H

#include <cstddef>

// Cac gioi han trong de bai
constexpr int MAX_LOP = 10000;
constexpr int MAX_LOP_TIN_CHI = 10000;
constexpr int MAX_MAMH = 11;       // C10 + ky tu ket thuc chuoi
constexpr int MAX_TENMH = 51;      // C50 + ky tu ket thuc chuoi
constexpr int MAX_MALOP = 16;
constexpr int MAX_TENLOP = 51;
constexpr int MAX_MASV = 16;
constexpr int MAX_HO = 31;
constexpr int MAX_TEN = 16;
constexpr int MAX_PHAI = 6;
constexpr int MAX_SODT = 16;
constexpr int MAX_NIEN_KHOA = 10;
constexpr float CHUA_CO_DIEM = -1.0f;

// ==================== KHAI BAO DU LIEU ====================

struct MonHoc {
    char maMH[MAX_MAMH];
    char tenMH[MAX_TENMH];
    int soTCLyThuyet;
    int soTCThucHanh;
};

struct NodeMonHoc {
    MonHoc data;
    NodeMonHoc* left;
    NodeMonHoc* right;
};

struct SinhVien {
    char maSV[MAX_MASV];
    char ho[MAX_HO];
    char ten[MAX_TEN];
    char phai[MAX_PHAI];
    char soDT[MAX_SODT];
};

struct NodeSV {
    SinhVien data;
    NodeSV* next;
};

struct Lop {
    char maLop[MAX_MALOP];
    char tenLop[MAX_TENLOP];
    NodeSV* dsSV;
};

struct DSLop {
    Lop ds[MAX_LOP];
    int soLuong;
};

struct DangKy {
    char maSV[MAX_MASV];
    float diem;
    bool huyDangKy;
};

struct NodeDK {
    DangKy data;
    NodeDK* next;
};

struct LopTinChi {
    int maLTC;
    char maMH[MAX_MAMH];
    char nienKhoa[MAX_NIEN_KHOA];
    int hocKy;
    int nhom;
    int soSVMin;
    int soSVMax;
    bool huyLop;
    NodeDK* dsDK;
};

struct DSLopTinChi {
    LopTinChi* ds[MAX_LOP_TIN_CHI];
    int soLuong;
};

struct HeThong {
    NodeMonHoc* dsMonHoc;
    DSLop dsLop;
    DSLopTinChi dsLopTinChi;
    int maLTCTiepTheo;
};

// ==================== KHOI TAO - GIAI PHONG ====================

void khoiTaoHeThong(HeThong& heThong);
void giaiPhongCayMonHoc(NodeMonHoc*& root);
void giaiPhongDSSV(NodeSV*& head);
void giaiPhongDSDK(NodeDK*& head);
void giaiPhongDSLop(DSLop& dsLop);
void giaiPhongDSLopTinChi(DSLopTinChi& dsLTC);
void giaiPhongHeThong(HeThong& heThong);

// ==================== e. NHAP - THEM - XOA - HIEU CHINH MON HOC ====================
// Cay BST duoc sap theo maMH. Dung chung cho cac chuc nang a, f, g, i, j, k, l.

NodeMonHoc* taoNodeMonHoc(const MonHoc& monHoc);
NodeMonHoc* timMonHoc(NodeMonHoc* root, const char* maMH);
const NodeMonHoc* timMonHoc(const NodeMonHoc* root, const char* maMH);
bool themMonHoc(NodeMonHoc*& root, const MonHoc& monHoc);
bool xoaMonHoc(NodeMonHoc*& root, const char* maMH);
bool suaMonHoc(NodeMonHoc* root, const char* maMH, const MonHoc& monHocMoi);

// ==================== f. IN DANH SACH MON HOC ====================

void inMonHocTheoMa(const NodeMonHoc* root);
void inDSMonHocTangTheoTen(const NodeMonHoc* root);

// Khong cho xoa mon hoc neu mon hoc da duoc dung de mo lop tin chi.
bool monHocDaCoLopTinChi(const DSLopTinChi& dsLTC, const char* maMH);

// ==================== LOP VA SINH VIEN ====================
// Dung chung cho cac chuc nang b, c, d, g, i, j, k, l.

Lop* timLop(DSLop& dsLop, const char* maLop);
const Lop* timLop(const DSLop& dsLop, const char* maLop);
bool themLop(DSLop& dsLop, const Lop& lop);
bool xoaLop(DSLop& dsLop, const char* maLop);
bool suaLop(DSLop& dsLop, const char* maLop, const Lop& lopMoi);

NodeSV* taoNodeSV(const SinhVien& sinhVien);
NodeSV* timSVTrongLop(NodeSV* head, const char* maSV);
const NodeSV* timSVTrongLop(const NodeSV* head, const char* maSV);
NodeSV* timSV(DSLop& dsLop, const char* maSV, Lop** lopChuaSV = nullptr);
const NodeSV* timSV(const DSLop& dsLop, const char* maSV, const Lop** lopChuaSV = nullptr);

// Danh sach sinh vien trong moi lop luon duoc giu tang dan theo TEN, sau do HO va MASV.
void chenSVTheoTen(NodeSV*& head, const SinhVien& sinhVien);
bool themSV(Lop& lop, const SinhVien& sinhVien);
bool xoaSV(Lop& lop, const char* maSV);
bool suaSV(Lop& lop, const char* maSV, const SinhVien& sinhVienMoi);
void inDSSVTheoTen(const Lop& lop);
void inDSSVTheoMaSV(const Lop& lop);

// ==================== c. NHAP - THEM - XOA - HIEU CHINH SINH VIEN CUA 1 LOP ====================
// Khi nhap tren giao dien, maSV rong la dieu kien ket thuc. Danh sach van giu tang theo TEN.

bool themSVVaoLop(DSLop& dsLop, const char* maLop, const SinhVien& sinhVien);
bool xoaSVKhoiLop(DSLop& dsLop, const char* maLop, const char* maSV);
bool suaSVTrongLop(DSLop& dsLop, const char* maLop, const char* maSV,
                   const SinhVien& sinhVienMoi);

// ==================== d. IN DSSV CUA 1 LOP THEO MA SINH VIEN ====================

void inDSSVTheoMaSV(const DSLop& dsLop, const char* maLop);

// ==================== a. MO LOP TIN CHI ====================

LopTinChi* taoLopTinChi(const LopTinChi& duLieu);
LopTinChi* timLopTinChi(DSLopTinChi& dsLTC, int maLTC);
const LopTinChi* timLopTinChi(const DSLopTinChi& dsLTC, int maLTC);
LopTinChi* timLopTinChi(DSLopTinChi& dsLTC, const char* nienKhoa,
                         int hocKy, const char* maMH, int nhom);
const LopTinChi* timLopTinChi(const DSLopTinChi& dsLTC, const char* nienKhoa,
                               int hocKy, const char* maMH, int nhom);

bool themLopTinChi(DSLopTinChi& dsLTC, const NodeMonHoc* dsMonHoc,
                   LopTinChi duLieuMoi, int& maLTCTiepTheo);
bool xoaLopTinChi(DSLopTinChi& dsLTC, int maLTC);
bool suaLopTinChi(DSLopTinChi& dsLTC, const NodeMonHoc* dsMonHoc,
                  int maLTC, const LopTinChi& duLieuMoi);
int demSVDangKyHopLe(const LopTinChi& ltc);
int soChoTrong(const LopTinChi& ltc);

// ==================== b. IN DSSV DA DANG KY LOP TIN CHI ====================

void inDSDangKyLopTinChi(const DSLop& dsLop, const LopTinChi& ltc);
void inDSDangKyLopTinChi(const DSLop& dsLop, const DSLopTinChi& dsLTC,
                         const char* nienKhoa, int hocKy,
                         const char* maMH, int nhom);

// ==================== g. DANG KY LOP TIN CHI ====================

NodeDK* taoNodeDK(const DangKy& dangKy);
NodeDK* timDangKy(NodeDK* head, const char* maSV);
const NodeDK* timDangKy(const NodeDK* head, const char* maSV);
bool sinhVienDaDangKyMon(const DSLopTinChi& dsLTC, const char* maSV,
                         const char* nienKhoa, int hocKy, const char* maMH);
bool dangKyLopTinChi(DSLop& dsLop, DSLopTinChi& dsLTC, const char* maSV, int maLTC);
bool huyDangKyLopTinChi(DSLopTinChi& dsLTC, const char* maSV, int maLTC);
void inLopTinChiMoTheoHocKy(const NodeMonHoc* dsMonHoc, const DSLopTinChi& dsLTC,
                            const char* nienKhoa, int hocKy);

// ==================== h. HUY LOP TIN CHI KHONG DU SI SO ====================
// Ham nay chi duoc goi sau khi main.cpp da hoi va nhan xac nhan tu nguoi dung.

int huyCacLopKhongDuSiSo(DSLopTinChi& dsLTC, const char* nienKhoa, int hocKy);

// ==================== i. NHAP VA HIEU CHINH DIEM ====================

bool capNhatDiem(LopTinChi& ltc, const char* maSV, float diem);
bool capNhatDiem(DSLopTinChi& dsLTC, const char* nienKhoa, int hocKy,
                 const char* maMH, int nhom, const char* maSV, float diem);
void inBangNhapDiem(const DSLop& dsLop, const LopTinChi& ltc);

// ==================== j. IN BANG DIEM MON HOC ====================

void inBangDiemMonHoc(const DSLop& dsLop, const NodeMonHoc* dsMonHoc,
                      const LopTinChi& ltc);
void inBangDiemMonHoc(const DSLop& dsLop, const DSLopTinChi& dsLTC,
                      const NodeMonHoc* dsMonHoc, const char* nienKhoa,
                      int hocKy, const char* maMH, int nhom);

// ==================== k. IN DIEM TRUNG BINH KET THUC KHOA ====================

// Tra ve CHUA_CO_DIEM neu sinh vien chua co diem hop le cho mon hoc do.
float diemCaoNhatMonCuaSV(const DSLopTinChi& dsLTC, const char* maSV, const char* maMH);
float tinhDTBTheoTinChi(const DSLopTinChi& dsLTC, const NodeMonHoc* dsMonHoc,
                        const char* maSV);
void inBangDTBKetThucKhoa(const Lop& lop, const DSLopTinChi& dsLTC,
                          const NodeMonHoc* dsMonHoc);

void inBangDTBKetThucKhoa(const DSLop& dsLop, const DSLopTinChi& dsLTC,
                          const NodeMonHoc* dsMonHoc, const char* maLop);

// ==================== l. IN BANG DIEM TONG KET ====================

void inBangDiemTongKet(const Lop& lop, const DSLopTinChi& dsLTC,
                       const NodeMonHoc* dsMonHoc);
void inBangDiemTongKet(const DSLop& dsLop, const DSLopTinChi& dsLTC,
                       const NodeMonHoc* dsMonHoc, const char* maLop);

// ==================== DOC GHI FILE ====================

bool luuDuLieu(const char* thuMuc, const HeThong& heThong);
bool taiDuLieu(const char* thuMuc, HeThong& heThong);

// ==================== KIEM TRA DU LIEU VA GIAO DIEN ====================

bool chuoiRong(const char* text);
bool diemHopLe(float diem);
bool thongTinLopTinChiHopLe(const LopTinChi& ltc);

#endif
