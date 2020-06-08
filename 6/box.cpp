//#include "pyramid.cpp"

//vector<GLfloat> buffer = {
//        -1.0f, -1.0f, -1.0f,
//        -1.0f, -1.0f, 1.0f,
//        -1.0f, 1.0f, 1.0f,
//        1.0f, 1.0f, -1.0f,
//        -1.0f, -1.0f, -1.0f,
//        -1.0f, 1.0f, -1.0f,
//        1.0f, -1.0f, 1.0f,
//        -1.0f, -1.0f, -1.0f,
//        1.0f, -1.0f, -1.0f,
//        1.0f, 1.0f, -1.0f,
//        1.0f, -1.0f, -1.0f,
//        -1.0f, -1.0f, -1.0f,
//        -1.0f, -1.0f, -1.0f,
//        -1.0f, 1.0f, 1.0f,
//        -1.0f, 1.0f, -1.0f,
//        1.0f, -1.0f, 1.0f,
//        -1.0f, -1.0f, 1.0f,
//        -1.0f, -1.0f, -1.0f,
//        -1.0f, 1.0f, 1.0f,
//        -1.0f, -1.0f, 1.0f,
//        1.0f, -1.0f, 1.0f,
//        1.0f, 1.0f, 1.0f,
//        1.0f, -1.0f, -1.0f,
//        1.0f, 1.0f, -1.0f,
//        1.0f, -1.0f, -1.0f,
//        1.0f, 1.0f, 1.0f,
//        1.0f, -1.0f, 1.0f,
//        1.0f, 1.0f, 1.0f,
//        1.0f, 1.0f, -1.0f,
//        -1.0f, 1.0f, -1.0f,
//        1.0f, 1.0f, 1.0f,
//        -1.0f, 1.0f, -1.0f,
//        -1.0f, 1.0f, 1.0f,
//        1.0f, 1.0f, 1.0f,
//        -1.0f, 1.0f, 1.0f,
//        1.0f, -1.0f, 1.0f
//};



struct Box {
//    vector<Polygon> polygon;
//
//    void init() {
//        for (int i = 0; i < buffer.size(); i += 9) {
//            Polygon p;
//            p.p1.x = multy * buffer[i];
//            p.p1.y = multy * buffer[i + 1];
//            p.p1.z = multy * buffer[i + 2];
//
//            p.p2.x = multy * buffer[i + 3];
//            p.p2.y = multy * buffer[i + 4];
//            p.p2.z = multy * buffer[i + 5];
//
//            p.p3.x = multy * buffer[i + 6];
//            p.p3.y = multy * buffer[i + 7];
//            p.p3.z = multy * buffer[i + 8];
//
//            p.color = Point{1, 0, 0};
//            p.countNormal();
//            p.normalize();
//            polygon.push_back(p);
//        }
//    }
    void draw() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_QUADS);
        glColor3f(1,0,0);
        glVertex3f(-multy, -multy, -multy);
        glColor3f(1,0,0);
        glVertex3f(-multy, multy, -multy);
        glColor3f(1,0,0);
        glVertex3f(multy, multy, -multy);
        glColor3f(1,0,0);
        glVertex3f(multy, -multy, -multy);

//        for (int i = 0; i < polygon.size(); i++) {
//            polygon[i].draw();
//        }
        glEnd();
    }
};

