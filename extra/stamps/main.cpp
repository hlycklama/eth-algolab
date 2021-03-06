// example: find the risk-optimal investment strategy
// in Swatch and Credit Suisse shares (data as in lecture)
#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;

// choose exact rational type
#include <CGAL/Gmpq.h>
typedef CGAL::Gmpq ET;
// solution type the solver provides
typedef CGAL::Quotient<ET> SolT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// round up to next integer double
double ceil_to_double(const SolT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

void testcase() {
    // read minimum expected return
    int l, s, w; std::cin >> l >> s >> w;

    std::vector<P> lights(l);
    for (int i = 0; i < l; i++) {
        int x, y; std::cin >> x >> y;
        P p(x, y);
        lights[i] = p;
    }

    Program lp (CGAL::SMALLER, true, 1, true, 4097);

    std::vector<P> stamps(s);
    std::vector<int> limits(s);
    for (int i = 0; i < s; i++) {
        int x, y, M; std::cin >> x >> y >> M;
        limits[i] = M;
        P p(x, y);
        stamps[i] = p;

        lp.set_c(i, -1); // just set it
    }

    std::vector<S> walls(w);
    for (int i = 0; i < w; i++) {
        int x1, y1, x2, y2; std::cin >> x1 >> y1 >> x2 >> y2;
        P p1(x1, y1), p2(x2, y2);
        walls[i] = S(p1, p2);
    }

    const int OFFSET_ONE = s;
    for (int i = 0; i < s; i++) {
        for (int j = 0; j < l; j++) {
            // no walls?
            S ray = S(stamps[i], lights[j]);
            bool inter = false;
            for (int k = 0; k < w; k++) {
                if (CGAL::do_intersect(ray,walls[k])) {
//                    std::cerr << ray << " and " << walls[k] << " intersect" << std::endl;
                    inter = true;
                    break;
                }
            }

            if (!inter) {
                K::FT r = ray.squared_length();
                //std::cerr << stamps[i] <<  " " << lights[j] << ": " << r << std::endl;
                lp.set_a(j, i, ET(1) / r);
                lp.set_a(j, i + OFFSET_ONE, ET(1) / r);
            }

        }

        lp.set_b(i, limits[i]);
        lp.set_b(i + OFFSET_ONE, 1);
        lp.set_r(i + OFFSET_ONE, CGAL::LARGER);
    }

    // solve the program, using ET as the exact type
//    assert(lp.is_nonnegative());
    Solution sol = CGAL::solve_linear_program(lp, ET());
    assert(sol.solves_linear_program(lp));

    if (sol.status() == CGAL::QP_INFEASIBLE) {
        std::cout << "no" << std::endl;
    } else {
        std::cout << "yes" << std::endl;
    }

}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    while(t-- > 0) {
        testcase();
    }
    return 0;
}