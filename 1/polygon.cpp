#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;
struct Edge;
GLfloat EPS = 0.1;

struct Point {
    GLfloat x, y;

    void draw() {
        glVertex2f(x, y);
    }

    GLfloat position(Edge *line);

    bool operator==(Point value) {
        return abs(x - value.x) < EPS;
    }

    bool operator<(Point value) {
        return (x < value.x);
    }

    bool operator<=(Point value) {
        return (x - value.x < EPS);
    }
};

struct Edge {
    Point begin, end;

    void draw() {
        begin.draw();
        end.draw();
    }

    Point *intersection(Point *b, Point *e) {
        GLfloat x = ((b->x * e->y - b->y * e->x) * (begin.x - end.x) -
                     (b->x - e->x) * (begin.x * end.y - begin.y * end.x)) /
                    ((b->x - e->x) * (begin.y - end.y) -
                     (b->y - e->y) * (begin.x - end.x));
        GLfloat y = ((b->x * e->y - b->y * e->x) * (begin.y - end.y) -
                     (b->y - e->y) * (begin.x * end.y - begin.y * end.x)) /
                    ((b->x - e->x) * (begin.y - end.y) -
                     (b->y - e->y) * (begin.x - end.x));
        Point *point = new Point{x, y};
        return point;
    }

    GLfloat length() {
        return sqrt((end.x - begin.x) * (end.x - begin.x) + (end.y - begin.y) * (end.y - begin.y));
    }

    Edge norm() {
        Edge result;
        if (end < begin) {
            result.begin = end;
            result.end = begin;
        } else {
            result.begin = begin;
            result.end = end;
        }
        return result;

    }

    bool contains(Edge v) {
        auto l = norm();
        auto r = v.norm();
        if (l.begin <= r.begin && r.end <= l.end) {
            return true;
        }
        return false;
    }

};

GLfloat Point::position(Edge *line) {
    return (line->end.x - line->begin.x) * (y - line->begin.y) -
           (line->end.y - line->begin.y) * (x - line->begin.x);
}

class Polygon {
public:
    Polygon() {};

    void addVertex(Point v);

    void drawPolygon(GLfloat *color);

    int size() {
        return edges.size();
    }

    void close();

    void clear() {
        vertices.clear();
        edges.clear();
    }

    int contains(Point v) {
        int count = 0;
        for (int i = 0; i < vertices.size(); i++) {
            if (abs(v.x - vertices[i].x) < EPS && abs(v.y - vertices[i].y) < EPS) {
                count++;
            }
        }
        cout << "CONTAINS : " << count << "\n";
        return count;
    }

    vector<Point> vertices;
    vector<Edge> edges;
};

void Polygon::addVertex(Point v) {
    if (vertices.size() > 0) {
        edges.push_back(Edge{vertices[vertices.size() - 1], v});
    }
    vertices.push_back(v);
}

void Polygon::drawPolygon(GLfloat *color) {
    glColor3fv(color);
    if (vertices.size() == 1) {
        glBegin(GL_POINTS);
        vertices[0].draw();
        glEnd();
        return;
    }
    glBegin(GL_LINES);
    for (auto it = edges.begin(); it != edges.end(); it++) {
        (*it).draw();
    }
    glEnd();
}

void Polygon::close() {
    edges.push_back(Edge{vertices[vertices.size() - 1], vertices[0]});
    vertices.push_back(vertices[0]);
}

class SuthHodg {
public:
    Polygon *polygon, *clipper, *cliped;

    Polygon *clip(bool isInside) {
        vector<Point *> polyPoints;
        vector<Edge> polyEdges;
        vector<Edge> blackList;
        vector<Edge> whiteList;
        for (int i = 0; i <= polygon->size(); i++) {
            polyPoints.push_back(&polygon->vertices[i]);
            polyEdges.push_back(polygon->edges[i]);
        }

        for (auto clipEdge = clipper->edges.begin(); clipEdge != clipper->edges.end(); clipEdge++) {
            vector<Point *> newPoints;
            vector<Edge> edges;
            for (auto polyPoint = polyPoints.begin(); polyPoint < polyPoints.end() - 1; polyPoint++) {
                //                                for (auto edge = polyEdges.begin(); edge < polyEdges.end(); edge++){
                //                                Point *begin = &edge->begin, *end = &edge->end;
                Point *begin = *(polyPoint), *end = *(polyPoint + 1);
                GLfloat bPointPosition = begin->position(&*clipEdge);
                GLfloat ePointPosition = end->position(&*clipEdge);
                if (bPointPosition < 0 && ePointPosition < 0) {
                    newPoints.push_back(end);
                    edges.push_back(Edge{*begin, *end});
                }
                if (bPointPosition >= 0 && ePointPosition < 0) {
                    newPoints.push_back(clipEdge->intersection(begin, end));
                    newPoints.push_back(end);
                    edges.push_back(Edge{*clipEdge->intersection(begin, end), *end});
                }
                if (bPointPosition < 0 && ePointPosition >= 0) {
                    newPoints.push_back(clipEdge->intersection(begin, end));
                    edges.push_back(Edge{*begin, *clipEdge->intersection(begin, end)});
                }

            }

            if (newPoints.size() > 0) {
                newPoints.push_back(newPoints[0]);
                edges.push_back(Edge{*newPoints[newPoints.size() - 1], *newPoints[0]});
            }

            polyPoints.resize(newPoints.size());
            copy(newPoints.begin(), newPoints.end(), polyPoints.begin());
            polyEdges.resize(edges.size());
            copy(edges.begin(), edges.end(), polyEdges.begin());

        }


        for (int i = 0; i < polyPoints.size(); i++) {
            cliped->addVertex(*polyPoints[i]);
        }

        for (auto clipEdge = clipper->edges.begin(); clipEdge != clipper->edges.end(); clipEdge++) {
            for (int i = 0; i < cliped->edges.size() - 1; i++) {
                Point *begin = &cliped->edges[i].begin, *end = &cliped->edges[i].end;
                GLfloat bPointPosition = begin->position(&*clipEdge);
                GLfloat ePointPosition = end->position(&*clipEdge);
                if (abs(bPointPosition) < EPS && abs(ePointPosition) < EPS) {
                    int k = 0;
                    for (int j = i + 1; j < cliped->edges.size(); j++) {
                        Point *begin2 = &(cliped->edges[j]).begin, *end2 = &(cliped->edges[j]).end;
                        GLfloat b2PointPosition = begin2->position(&*clipEdge);
                        GLfloat e2PointPosition = end2->position(&*clipEdge);
                        if (abs(b2PointPosition) < EPS && abs(e2PointPosition) < EPS) {
                            if (Edge{*begin, *end}.contains(Edge{*begin2, *end2})) {
                                if (k == 0) {
                                    blackList.push_back(Edge{*begin, *end});
                                }
                                blackList.push_back(Edge{*begin2, *end2});
                                Edge n = Edge{*begin, *end}.norm();
                                Edge m = Edge{*begin2, *end2}.norm();
                                whiteList.push_back(Edge{n.begin, m.begin});
                                whiteList.push_back(Edge{n.end, m.end});
                                k++;
                            }
                            if (Edge{*begin2, *end2}.contains(Edge{*begin, *end})) {
                                if (k == 0) {
                                    blackList.push_back(Edge{*begin2, *end2});
                                }
                                blackList.push_back(Edge{*begin, *end});
                                Edge n = Edge{*begin2, *end2}.norm();
                                Edge m = Edge{*begin, *end}.norm();
                                whiteList.push_back(Edge{n.begin, m.begin});
                                whiteList.push_back(Edge{n.end, m.end});
                                k++;
                            }
                        }

                    }

                }
            }
        }

        cout << " WHITE : " << whiteList.size() << endl;
        cout << " BLACK : " << blackList.size() << endl;
        for (int i = 0; i < whiteList.size(); i++) {
            cliped->edges.push_back(whiteList[i]);
        }

        for (int i = 0; i < cliped->edges.size(); i++) {
            auto edge = cliped->edges[i];
            cout << "EDGE : " << i << " " << edge.begin.x << " " << edge.begin.y << " " << edge.end.x << " "
                 << edge.end.y << "\n";
            for (int j = 0; j < blackList.size(); j++) {
                if ((blackList[j].begin == edge.begin && blackList[j].end == edge.end) ||
                    (blackList[j].end == edge.begin && blackList[j].begin == edge.end)) {
                    cliped->edges.erase(cliped->edges.begin() + i);
                    break;
                }
            }
        }
    }
};