/**
 * Author: Lucian Bicsi
 * Date: 2017-10-31
 * License: CC0
 * Source: folklore
 * Description: Zero-indexed max-tree. Bounds are inclusive to the left and exclusive to the right.
 * Can be changed by modifying T, f and unit.
 * Time: O(\log N)
 * Status: stress-tested
 */

#include <bits/stdc++.h>
using namespace std;
typedef vector<int> vi;
class SegmentTree { // OOP style
  private:
    int n;                                // n = (int)A.size()
    vi A, st, lazy;                       // the arrays
    int l(int p) { return p << 1; }       // go to left child
    int r(int p) { return (p << 1) + 1; } // go to right child
    int conquer(int a, int b) {
        if (a == -1)
            return b; // corner case
        if (b == -1)
            return a;
        return min(a, b); // RMQ
    }
    void build(int p, int L, int R) { // O(n)
        if (L == R)
            st[p] = A[L]; // base case
        else {
            int m = (L + R) / 2;
            build(l(p), L, m);
            build(r(p), m + 1, R);
            st[p] = conquer(st[l(p)], st[r(p)]);
        }
    }
    void propagate(int p, int L, int R) {
        if (lazy[p] != -1) {
            st[p] = lazy[p];
            if (L != R)
                lazy[l(p)] = lazy[r(p)] = lazy[p];
            else
                A[L] = lazy[p];
            lazy[p] = -1;
        }
    }
    int RMQ(int p, int L, int R, int i, int j) { // O(log n)
        propagate(p, L, R);                      // lazy propagation
        if (i > j)
            return -1; // infeasible
        if ((L >= i) && (R <= j))
            return st[p]; // found the segment
        int m = (L + R) / 2;
        return conquer(RMQ(l(p), L, m, i, min(m, j)), RMQ(r(p), m + 1, R, max(i, m + 1), j));
    }

    void update(int p, int L, int R, int i, int j, int val) { // O(log n)
        propagate(p, L, R);                                   // lazy propagation
        if (i > j)
            return;
        if ((L >= i) && (R <= j)) { // found the segment
            lazy[p] = val;          // update this
            propagate(p, L, R);     // lazy propagation
        } else {
            int m = (L + R) / 2;
            update(l(p), L, m, i, min(m, j), val);
            update(r(p), m + 1, R, max(i, m + 1), j, val);
            int lsubtree = (lazy[l(p)] != -1) ? lazy[l(p)] : st[l(p)];
            int rsubtree = (lazy[r(p)] != -1) ? lazy[r(p)] : st[r(p)];
            st[p] = (lsubtree <= rsubtree) ? st[l(p)] : st[r(p)];
        }
    }

  public:
    SegmentTree(int sz) : n(sz), st(4 * n), lazy(4 * n, -1) {}
    SegmentTree(const vi &initialA) : SegmentTree((int)initialA.size()) {
        A = initialA;
        build(1, 0, n - 1);
    }
    void update(int i, int j, int val) { update(1, 0, n - 1, i, j, val); }
    int RMQ(int i, int j) { return RMQ(1, 0, n - 1, i, j); }
};
