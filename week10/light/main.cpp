//
// Created by Hidde Lycklama on 11/24/18.
//

#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

//typedef Radius int;
struct Info {
    int index;
    int radius;
    bool person;
    bool alive;
    int* exists = NULL;

    Info(int i, int a, bool b, bool aliv): index(i), radius(a), person(b), alive(aliv) {
        exists = &index;
    };
    Info() = default;
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_with_info_2<Info, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;

typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef K::Point_2 Point;
typedef K::Circle_2 Circle;

//typedef std::pair<Point, int> Person;

void testcase() {
    int m, n; std::cin >> m >> n;

    std::vector<Point> people(m);
    std::vector<bool> alive(m, true);
    std::vector<int> peopleRadius(m);
    std::map<std::pair<int, int>, int> indexMap;
    for (int i = 0; i < m; i ++) {
        int x, y;
        std::cin >> x >> y;
        Point center(x, y);
        int radius; std::cin >> radius;
        people[i] = center;
        peopleRadius[i] = radius;
        std::pair<int, int> pa = std::pair<int, int>(x, y);
        indexMap[pa] = i;
//        std::cout << people[i] << std::endl;
    }

    Triangulation t;
//    t.insert(people.begin(), people.end());

//    for (auto iter = t.finite_vertices_begin(); iter != t.finite_vertices_end(); iter++) {
//        Point p = iter->point();
//        int i = indexMap[std::make_pair<int, int>(p.x(), p.y())];
////        std::cerr << p << ": " << i << std::endl;
//        iter->info() = Info(i, peopleRadius[i], true, true);
//    }


    long h; std::cin >> h;
    long hSquared = h * h;
//    double light = h*h / 4.0;
    std::vector<Point> lights(n);
    for (int i = 0; i < n; i++) {
        Point center;
        std::cin >> center;
        lights[i] = center;
//
//        Vertex_handle nearest_vertex = t.nearest_vertex(center);
//        Point person = nearest_vertex->point();
    }

    t.insert(lights.begin(), lights.end());

    for (int a = n - 1; a >= 0; a--) {
        int numded = 0;
        for (int i = 0; i < m; i++) {
            Vertex_handle v = t.nearest_vertex(people[i]);
            long radius = peopleRadius[i];

            K::FT dist = CGAL::squared_distance(v->point(), people[i]);
            K::FT calc = K::FT(radius * radius) + K::FT(2 * radius * h) + K::FT(hSquared);

            if (dist < calc) {
                // ded
                alive[i] = false;
                numded++;
            } else {
                alive[i] = true;
            }
        }
        if (numded < m) {
            break; // We have winners!
        }
        // else, remove last vertex
        Vertex_handle l = t.nearest_vertex(lights[a]);
        t.remove(l);
    }



    for (int i = 0; i < m; i++) {
        if (alive[i]) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;

//    for (auto iter = t.finite_vertices_begin(); iter != t.finite_vertices_end(); iter++) {
//        if (iter->info().exists != NULL) {
//            long radius = iter->info().radius;
////            std::cerr << "At " << iter->info().index << " (" << iter->point() << ") " << iter->info().person << std::endl;
//            // check to see if any lights overlap
//            Triangulation::Edge_circulator c = t.incident_edges(iter);
//            do {
//                if (!t.is_infinite(c)) {
//                    Triangulation::Vertex_handle v2 = c->first->vertex((c->second + 1) % 3);
////                    std::cerr << "Found vertex: " << v2->point() << std::endl;
//                    if (v2->info().exists == NULL) {
////                        std::cerr << "Found light: " << v2->point() << std::endl;
//                        // it is a light!
//                        // we can probably flip this, first checkin the distance
//                        K::FT dist = t.segment(c).squared_length();
//                        K::FT calc = K::FT(radius * radius) + K::FT(2 * radius * h) + K::FT(hSquared);
//                        if (dist < calc) {
//                            // we are ded
//                            iter->info().alive = false;
//                            break;
//                        } else {
//                            std::cerr << iter->info().index << " not ded " << dist << " !< " << calc << std::endl;
//                        }
//                    }
//                }
//            } while (++c != t.incident_edges(iter));
//        }
//    }
//
//    std::vector<int> winners;
//    for (auto iter = t.finite_vertices_begin(); iter != t.finite_vertices_end(); iter++) {
//        if (iter->info().exists != NULL && iter->info().alive) {
////            std::cerr << iter->point() << " ";
//            winners.push_back(iter->info().index);
//        }
//    }
////    std::cerr << std::endl;
//
//    std::sort(winners.begin(), winners.end());
//    for (auto iter = winners.begin(); iter != winners.end(); iter++) {
//        std::cout << *iter << " ";
//    }
//    std::cout << std::endl;

//    for (int i = 0; i < m; i++) {
//        bool ded = false;
//        for (int a = 0; a < n; a++) {
//            if (CGAL::do_intersect(people[i], lights[a])) {
//                ded = true;
//                break;
//            }
//        }
//        if (!ded) {
//            std::cout << i << " ";
//        }
////        if (alive[i]) {
////            std::cout << i << " ";
////        }
//    }
//    std::cout << std::endl;
}

int main() {
    int t; std::cin >> t;
    while (t--) {
        testcase();
    }
    return 0;
}