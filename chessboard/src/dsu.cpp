#include "chessboard/dsu.hpp"

#include <numeric>
#include <cassert>

DSU::DSU(int n) {
    init(n);
}

void DSU::init(int n_) {
    this->n = n_;
    this->cnt = n_;
    sz.assign(n_, 1);
    p.resize(n_);
    std::iota(p.begin(), p.end(), 0);
}

int DSU::leader(int x) {
    assert(0 <= x && x < n);
    return x == p[x] ? x : p[x] = leader(p[x]);
}

bool DSU::same(int a, int b) {
    assert(0 <= a && a < n);
    assert(0 <= b && b < n);
    return leader(a) == leader(b);
}

int DSU::size(int x) {
    assert(0 <= x && x < n);
    return sz[leader(x)];
}

int DSU::count() {
    return cnt;
}

bool DSU::merge(int a, int b) {
    assert(0 <= a && a < n);
    assert(0 <= b && b < n);
    int pa = leader(a), pb = leader(b);
    if (pa == pb) {
        return false;
    }
    sz[pa] += sz[pb];
    p[pb] = pa;
    --cnt;
    return true;
}
