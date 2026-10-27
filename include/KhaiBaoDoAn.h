#ifndef QLDIEM_HE_TIN_CHI_PROTOTYPE_NGHIEPVU_H
#define QLDIEM_HE_TIN_CHI_PROTOTYPE_NGHIEPVU_H



#include <cstddef>
#include <cstdio>  // FILE* cho cac prototype doc/ghi cay

// ========================== HANG SO ==========================

constexpr int MAX_LOP = 10000;
constexpr int MAX_LOP_TIN_CHI = 10000;

constexpr int MAX_MAMH = 11;        // MAMH toi da 10 ky tu + '\0'
constexpr int MAX_TENMH = 51;       // TENMH toi da 50 ky tu + '\0'
constexpr int MAX_MALOP = 16;       // MALOP toi da 15 ky tu + '\0'
constexpr int MAX_TENLOP = 51;      // TENLOP toi da 50 ky tu + '\0'
constexpr int MAX_MASV = 16;        // MASV toi da 15 ky tu + '\0'
constexpr int MAX_HO = 31;          // HO toi da 30 ky tu + '\0'
constexpr int MAX_TEN = 16;         // TEN toi da 15 ky tu + '\0'
constexpr int MAX_PHAI = 6;         // Vi du: "Nam" + '\0'
constexpr int MAX_SODT = 16;        // So dien thoai toi da 15 ky tu + '\0'
constexpr int MAX_NIEN_KHOA = 11;   // "2026-2027" + '\0'
constexpr float CHUA_CO_DIEM = -1.0f;

// ======================= CAU TRUC DU LIEU =======================

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
    float diem;             // CHUA_CO_DIEM neu chua nhap diem
    bool huyDangKy;
};

struct NodeDK {
    DangKy data;
    NodeDK* next;
};

struct LopTinChi {
    int maLTC;               // Ma lop tin chi tu dong tang
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

// =========================== ENUM ===========================

enum class KieuTimKiemLTC {
    THEO_MALTC,     // Tim theo ma lop tin chi
    THEO_MAMH,      // Tim theo ma mon hoc
    THEO_TENMH      // Tim theo ten mon hoc
};

enum class PhamViTinhDiem {
    THEO_HOC_KY,    // Mot nien khoa + hoc ky
    TOAN_KHOA       // Tat ca cac hoc ky/nam hoc co diem hop le
};

// Ket qua cac ham bool:
// - true: thao tac thanh cong
// - false: khong hop le, khong tim thay, trung khoa, hoac khong the thao tac.
// Cac ham khong tu in thong bao giao dien.

// ===================== KHOI TAO / GIAI PHONG =====================

void khoiTaoHeThong(HeThong& heThong);
void giaiPhongCayMonHoc(NodeMonHoc*& root);
void giaiPhongDSSV(NodeSV*& head);
void giaiPhongDSDK(NodeDK*& head);
void giaiPhongDSLop(DSLop& dsLop);
void giaiPhongDSLopTinChi(DSLopTinChi& dsLTC);
void giaiPhongHeThong(HeThong& heThong);

// =========================== HAM CHUNG ===========================

bool chuoiRong(const char* text);
bool diemHopLe(float diem);
bool thongTinMonHocHopLe(const MonHoc& monHoc);
bool thongTinSinhVienHopLe(const SinhVien& sinhVien);
bool thongTinLopHopLe(const Lop& lop);
bool thongTinLopTinChiHopLe(const LopTinChi& ltc);

// So sanh chuoi khong phan biet chu hoa/chu thuong.
// Khong dung stricmp/strcasecmp neu mon hoc yeu cau tu cai dat.
int soSanhChuoi(const char* a, const char* b);
int soSanhChuoiKhongPhanBietHoaThuong(const char* a, const char* b);
bool chuoiChuaTuKhoa(const char* chuoi, const char* tuKhoa);

// ===================== MON HOC - CAY BST =====================

// BST duoc sap theo maMH.
NodeMonHoc* taoNodeMonHoc(const MonHoc& monHoc);
NodeMonHoc* timMonHoc(NodeMonHoc* root, const char* maMH);
const NodeMonHoc* timMonHoc(const NodeMonHoc* root, const char* maMH);
bool themMonHoc(NodeMonHoc*& root, const MonHoc& monHoc);
bool xoaMonHoc(NodeMonHoc*& root, const char* maMH);
bool suaMonHoc(NodeMonHoc* root, const char* maMH, const MonHoc& monHocMoi);

// In danh sach mon hoc tang dan theo TENMH; neu trung TENMH thi tang theo MAMH.
void inDSMonHocTangTheoTen(const NodeMonHoc* root);
void inMonHocTheoMa(const NodeMonHoc* root);

// Khong cho xoa mon hoc neu da duoc dung mo lop tin chi.
bool monHocDaCoLopTinChi(const DSLopTinChi& dsLTC, const char* maMH);

// Duyet cay phuc vu luu/nap file.
bool ghiCayMonHoc(FILE* file, const NodeMonHoc* root);
bool docCayMonHoc(FILE* file, NodeMonHoc*& root);

// =========================== LOP ===========================

Lop* timLop(DSLop& dsLop, const char* maLop);
const Lop* timLop(const DSLop& dsLop, const char* maLop);
bool themLop(DSLop& dsLop, const Lop& lop);
bool xoaLop(DSLop& dsLop, const char* maLop);
bool suaLop(DSLop& dsLop, const char* maLop, const Lop& lopMoi);

// Quy tac: khong xoa lop neu lop con sinh vien hoac sinh vien cua lop
// dang co dang ky lop tin chi chua huy. Neu muon quy tac khac, nhom can thong nhat.
bool lopCoSinhVienDangKyLTC(const DSLopTinChi& dsLTC, const Lop& lop);

// ======================== SINH VIEN ========================

NodeSV* taoNodeSV(const SinhVien& sinhVien);
NodeSV* timSVTrongLop(NodeSV* head, const char* maSV);
const NodeSV* timSVTrongLop(const NodeSV* head, const char* maSV);

// MaSV duy nhat tren toan he thong.
NodeSV* timSV(DSLop& dsLop, const char* maSV, Lop** lopChuaSV = nullptr);
const NodeSV* timSV(const DSLop& dsLop, const char* maSV,
                    const Lop** lopChuaSV = nullptr);

// Chen tang dan theo TEN, tiep theo HO, tiep theo MASV.
void chenSVTheoTen(NodeSV*& head, const SinhVien& sinhVien);
bool themSV(Lop& lop, const SinhVien& sinhVien);
bool xoaSV(Lop& lop, const char* maSV);
bool suaSV(Lop& lop, const char* maSV, const SinhVien& sinhVienMoi);
void inDSSVTheoTen(const Lop& lop);
void inDSSVTheoMaSV(const Lop& lop);

bool themSVVaoLop(DSLop& dsLop, const char* maLop, const SinhVien& sinhVien);
bool xoaSVKhoiLop(DSLop& dsLop, const char* maLop, const char* maSV);
bool suaSVTrongLop(DSLop& dsLop, const char* maLop, const char* maSV,
                   const SinhVien& sinhVienMoi);
void inDSSVTheoMaSV(const DSLop& dsLop, const char* maLop);

// Khong xoa SV neu con dang ky hop le/chua huy o lop tin chi.
// Neu can xoa SV co dang ky da huy, phai giu du lieu dang ky de bao toan lich su.
bool sinhVienCoDangKyChuaHuy(const DSLopTinChi& dsLTC, const char* maSV);

// ======================== LOP TIN CHI ========================

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
bool lopTinChiDaThi(const LopTinChi& ltc);
bool lopTinChiChoPhepDangKy(const LopTinChi& ltc);

// In danh sach LTC kem ten mon hoc tra tu cay BST; khong luu trung TENMH trong LTC.
void inDSLopTinChi(const DSLopTinChi& dsLTC, const NodeMonHoc* dsMonHoc);

// Tim/loc LTC theo maLTC, maMH hoac tenMH. Tim kiem TENMH khong phan biet hoa/thuong.
// Co the loc them theo nien khoa/hoc ky; nullptr/0 nghia la khong loc theo tieu chi do.
bool lopTinChiKhopTuKhoa(const LopTinChi& ltc, const NodeMonHoc* dsMonHoc,
                         KieuTimKiemLTC kieuTim, const char* tuKhoa);
void timKiemLocLopTinChi(const DSLopTinChi& dsLTC, const NodeMonHoc* dsMonHoc,
                         KieuTimKiemLTC kieuTim, const char* tuKhoa,
                         const char* nienKhoa = nullptr, int hocKy = 0);

// ======================== DANG KY LOP TIN CHI ========================

NodeDK* taoNodeDK(const DangKy& dangKy);
NodeDK* timDangKy(NodeDK* head, const char* maSV);
const NodeDK* timDangKy(const NodeDK* head, const char* maSV);
bool sinhVienDaDangKyMon(const DSLopTinChi& dsLTC, const char* maSV,
                         const char* nienKhoa, int hocKy, const char* maMH);

// Tu choi dang ky neu:
// - SV khong ton tai;
// - LTC khong ton tai, da huy, da thi hoac day si so;
// - SV da dang ky LTC nay va dang ky chua bi huy;
// - SV da dang ky mot nhom khac cua cung mon trong cung nien khoa/hoc ky.
bool dangKyLopTinChi(const DSLop& dsLop, DSLopTinChi& dsLTC,
                    const char* maSV, int maLTC);
bool huyDangKyLopTinChi(DSLopTinChi& dsLTC, const char* maSV, int maLTC);

void inDSDangKyLopTinChi(const DSLop& dsLop, const LopTinChi& ltc);
void inDSDangKyLopTinChi(const DSLop& dsLop, const DSLopTinChi& dsLTC,
                         const char* nienKhoa, int hocKy,
                         const char* maMH, int nhom);
void inLopTinChiMoTheoHocKy(const NodeMonHoc* dsMonHoc,
                            const DSLopTinChi& dsLTC,
                            const char* nienKhoa, int hocKy);

// Huy cac lop co so SV dang ky hop le < soSVMin.
// Ham nghiep vu chi thuc hien huy; phan hoi xac nhan phai do lop goi ham quyet dinh.
int huyCacLopKhongDuSiSo(DSLopTinChi& dsLTC,
                          const char* nienKhoa, int hocKy);

// ======================== NHAP / SUA DIEM ========================

// Chi chap nhan diem [0,10]. CHUA_CO_DIEM khong phai diem hop le.
// capNhatDiem sua truc tiep node DangKy trong bo nho.
bool capNhatDiem(LopTinChi& ltc, const char* maSV, float diem);
bool capNhatDiem(DSLopTinChi& dsLTC, const char* nienKhoa, int hocKy,
                 const char* maMH, int nhom, const char* maSV, float diem);
void inBangNhapDiem(const DSLop& dsLop, const LopTinChi& ltc);

// ======================== BANG DIEM MON HOC ========================

void inBangDiemMonHoc(const DSLop& dsLop, const NodeMonHoc* dsMonHoc,
                      const LopTinChi& ltc);
void inBangDiemMonHoc(const DSLop& dsLop, const DSLopTinChi& dsLTC,
                      const NodeMonHoc* dsMonHoc, const char* nienKhoa,
                      int hocKy, const char* maMH, int nhom);

// ======================== DIEM TRUNG BINH ========================

// Tong tin chi mon = soTCLyThuyet + soTCThucHanh.
// Diem mon de tinh diem tong ket la diem cao nhat cua SV o mon do trong cac
// lop tin chi da co diem hop le. Khong tinh mon chua co diem.
// Neu khong co mon nao co diem hop le, tra ve CHUA_CO_DIEM.
float diemCaoNhatMonCuaSV(const DSLopTinChi& dsLTC,
                          const char* maSV, const char* maMH);

// Diem TB co trong so tin chi. Pham vi TOAN_KHOA dung diem cao nhat moi mon;
// THEO_HOC_KY chi xet lop thuoc nien khoa + hoc ky chi dinh.
float tinhDTBTheoTinChi(const DSLopTinChi& dsLTC,
                        const NodeMonHoc* dsMonHoc,
                        const char* maSV,
                        PhamViTinhDiem phamVi,
                        const char* nienKhoa = nullptr,
                        int hocKy = 0);

void inBangDTBKetThucKhoa(const Lop& lop, const DSLopTinChi& dsLTC,
                          const NodeMonHoc* dsMonHoc);
void inBangDTBKetThucKhoa(const DSLop& dsLop, const DSLopTinChi& dsLTC,
                          const NodeMonHoc* dsMonHoc, const char* maLop);

// Bang diem tong ket cua lop: moi SV x moi mon chi in diem thi cao nhat.
// Cac mon hien thi can duoc sap xep tang dan theo MAMH de bang on dinh.
void inBangDiemTongKet(const Lop& lop, const DSLopTinChi& dsLTC,
                       const NodeMonHoc* dsMonHoc);
void inBangDiemTongKet(const DSLop& dsLop, const DSLopTinChi& dsLTC,
                       const NodeMonHoc* dsMonHoc, const char* maLop);

// ======================== LUU / DOC DU LIEU ========================

// tenFile la DUONG DAN DAY DU den file, vi du "data/du_lieu.txt".
bool luuDuLieu(const char* tenFile, const HeThong& heThong);
bool taiDuLieu(const char* tenFile, HeThong& heThong);

// Goi luuDuLieu() sau khi thao tac thay doi thanh cong va khi thoat moi chuc nang.
// Ham nay la wrapper de main/UI goi; can cai dat trong File.cpp.
bool luuDuLieuKhiThoatChucNang(const char* tenFile, const HeThong& heThong);

// ======================== KIEM TRA TOAN VEN DU LIEU ========================

// Kiem tra cac khoa trung, con tro/danh sach, tham so va quan he tham chieu.
// Tra ve false neu phat hien du lieu khong hop le.
bool kiemTraToanVenDuLieu(const HeThong& heThong);

#endif // QLDIEM_HE_TIN_CHI_PROTOTYPE_NGHIEPVU_H