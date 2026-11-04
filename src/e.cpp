#include "../include/KhaiBaoDoAn.h"
#include <iostream>

using namespace std;

NodeMonHoc* taoNodeMonHoc(const MonHoc& monHoc) {
    NodeMonHoc* p = new NodeMonHoc;

    p->data = monHoc;
    p->left = nullptr;
    p->right = nullptr;

    return p;
}

NodeMonHoc* timMonHoc(NodeMonHoc* root, const char* maMH) {
    NodeMonHoc* p = root;

    while (p != nullptr) {
        int kq = soSanhChuoi(maMH, p->data.maMH);

        if (kq == 0) return p;

        if (kq < 0) {
            p = p->left;
        } else {
            p = p->right;
        }
    }

    return nullptr;
}

const NodeMonHoc* timMonHoc(
    const NodeMonHoc* root,
    const char* maMH
) {
    const NodeMonHoc* p = root;

    while (p != nullptr) {
        int kq = soSanhChuoi(maMH, p->data.maMH);

        if (kq == 0) return p;

        if (kq < 0) {
            p = p->left;
        } else {
            p = p->right;
        }
    }

    return nullptr;
}

bool themMonHoc(NodeMonHoc*& root, const MonHoc& monHoc) {
    if (!thongTinMonHocHopLe(monHoc)) return false;

    NodeMonHoc* moi = taoNodeMonHoc(monHoc);

    if (root == nullptr) {
        root = moi;
        return true;
    }

    NodeMonHoc* p = root;
    NodeMonHoc* truoc = nullptr;

    while (p != nullptr) {
        truoc = p;

        int kq = soSanhChuoi(
            monHoc.maMH,
            p->data.maMH
        );

        if (kq == 0) {
            delete moi;
            return false;
        }

        if (kq < 0) {
            p = p->left;
        } else {
            p = p->right;
        }
    }

    if (soSanhChuoi(monHoc.maMH, truoc->data.maMH) < 0) {
        truoc->left = moi;
    } else {
        truoc->right = moi;
    }

    return true;
}

static NodeMonHoc* timNodeTheMang(NodeMonHoc* root) {
    NodeMonHoc* p = root;

    while (p != nullptr && p->left != nullptr) {
        p = p->left;
    }

    return p;
}

bool xoaMonHoc(NodeMonHoc*& root, const char* maMH) {
    if (root == nullptr) return false;

    int kq = soSanhChuoi(maMH, root->data.maMH);

    if (kq < 0) {
        return xoaMonHoc(root->left, maMH);
    }

    if (kq > 0) {
        return xoaMonHoc(root->right, maMH);
    }

    if (root->left == nullptr) {
        NodeMonHoc* tam = root;
        root = root->right;
        delete tam;
        return true;
    }

    if (root->right == nullptr) {
        NodeMonHoc* tam = root;
        root = root->left;
        delete tam;
        return true;
    }

    NodeMonHoc* theMang = timNodeTheMang(root->right);

    root->data = theMang->data;

    return xoaMonHoc(root->right, theMang->data.maMH);
}

bool suaMonHoc(
    NodeMonHoc* root,
    const char* maMH,
    const MonHoc& monHocMoi
) {
    if (!thongTinMonHocHopLe(monHocMoi)) return false;

    NodeMonHoc* cu = timMonHoc(root, maMH);

    if (cu == nullptr) return false;

    NodeMonHoc* trung = timMonHoc(root, monHocMoi.maMH);

    if (trung != nullptr && trung != cu) return false;

    if (soSanhChuoi(maMH, monHocMoi.maMH) == 0) {
        cu->data = monHocMoi;
        return true;
    }

    if (!xoaMonHoc(root, maMH)) return false;

    return themMonHoc(root, monHocMoi);
}