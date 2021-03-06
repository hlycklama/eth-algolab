//
// Created by Hidde Lycklama on 10/27/18.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <set>
#include <stack>
#include <map>
#include <cmath>
#include <tuple>
#include <deque>

struct Beverage {
    int cost;
    int volume;

    Beverage(int c, int v): cost(c), volume(v) {}
    Beverage() = default;

    long ratio() const {
        long r = std::round(((double)cost / (double)volume) * 1e8);
        if (r <= 0) {
            std::cout << "AYY " << r << std::endl;
        }
        return r;
    }

    bool operator < (const Beverage &o) const {
        if (this->ratio() < o.ratio()) {
            return this->volume > o.volume; // higher volume first
        }
        return this->ratio() < o.ratio();
//        return this->ratio() < o.ratio();
    }
};

typedef std::pair<long, long> Result; // COST, NUMDIFF

typedef std::vector<std::vector<Result> > Table;

Table DP;

int n;

//Result KNAP(int i, int curvol, int vol, std::vector<Beverage> &drinks) {
//    if (curvol >= vol) {
//        // we are done
//        return Result(0, 0);
//    }
//    if (i == -1) {
//        // we are done
//        return Result(INT_MAX, 0);
//    }
//    if (DP[i][curvol] != Result(-1, -1)) {
//        return DP[i][curvol];
//    }
//
//
//    // fill the bottom
//    Result pick = KNAP(i, curvol + drinks[i].volume, vol, drinks);
////    std::cerr << "pick " << pick.first << " " << pick.second << " " << lastpick << " " << i << std::endl;
//
//
//    int add = i == n - 1 ? 1 : 0;
//    if (i < n - 1 && DP[i + 1][curvol + drinks[i].volume] == DP[i][curvol + drinks[i].volume]) {
//        add = 1;
//    }
//    Result pickAdd = Result(drinks[i].cost + pick.first, pick.second + add);
//
//
//    Result dontpick = KNAP(i - 1, curvol, vol, drinks);
//
//    Result val;
//    if (pickAdd.first == dontpick.first) {
//        if (pickAdd.second > dontpick.second) {
//            val = pickAdd;
//        } else {
//            val = dontpick;
//        }
//    } else if (pickAdd.first < dontpick.first) {
//        val = pickAdd;
//    } else {
//        val = dontpick;
//    }
//    DP[i][curvol] = val;
//    return val;
//}

void testcase() {
    int people;
    std::cin >> n >> people;
    std::vector<Beverage> drinks(n);
    for (int i = 0; i < n; i++) {
        int c; int v;
        std::cin >> c >> v;
//        drinks[i] = Beverage(c, v);
//        for (int a = 0; a < people / v;)
        Beverage b = Beverage(c, v);
        drinks[i] = b;
    }

    DP = Table(n, std::vector<Result>(people + 1));

    for (int i = 0; i < n; i++) {
        Beverage *b = &drinks[i];
        for (int j = 1; j <= people; j++) {
            long cost = 0;
            long numBev = 0;
            if (j - b->volume <= 0) {
                //
                cost = b->cost;
                numBev = 1;
            } else {
                Result prev = DP[i][j - b->volume];
                cost = prev.first + b->cost;
                numBev = prev.second;
                if (i > 0) {
                    Result prevLeft = DP[i - 1][j - b->volume];
                    if (prevLeft == prev) { // we used the prev value... but we have to compare both sides because cost may be the same but we still use ours
                        numBev = prev.second + 1; // this is the first time we use this
                    }
                }
            }
            if (i == 0) {
                DP[i][j] = Result(cost, numBev);
            } else if (DP[i - 1][j].first == cost) {
                DP[i][j] = Result(cost, std::max(numBev, DP[i - 1][j].second));
            } else if (DP[i - 1][j].first < cost) {
                DP[i][j] = DP[i - 1][j];
            } else {
                DP[i][j] = Result(cost, numBev);
            }
        }
    }

//    for (int i = 0; i < n; i++) {
//        std::cerr << i << ": ";
//        for (int j = 0; j <= people; j++) {
//            std::cerr << DP[i][j].first << "-" << DP[i][j].second << " ";
//        }
//        std::cerr << std::endl;
//    }

    std::cout << DP[n - 1][people].first << " " << DP[n - 1][people].second << std::endl;

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while (t > 0) {
        testcase();
        t--;
    }
}