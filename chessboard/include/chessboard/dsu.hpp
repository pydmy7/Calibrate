#pragma once

#include <vector>

class DSU {
private:
    int n, cnt;
    std::vector<int> p, sz;

public:
    explicit DSU(int n);
    void init(int n);
    int leader(int x);
    bool same(int a, int b);
    int size(int x);
    int count();
    bool merge(int a, int b);
};
