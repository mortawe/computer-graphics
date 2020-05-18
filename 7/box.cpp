
struct Box {
    GLfloat *color;
    GLfloat *vertices;

    void init(GLfloat m) {
        destroy();
        vertices = new GLfloat[4 * 6 * 3]{
                // front face
                0 - m, 0 + m, 0 + m, // top left
                0 + m, 0 + m, 0 + m, // top right
                0 + m, 0 - m, 0 + m, // bottom right
                0 - m, 0 - m, 0 + m, // bottom left

                // back face
                0 - m, 0 + m, 0 - m, // top left
                0 + m, 0 + m, 0 - m, // top right
                0 + m, 0 - m, 0 - m, // bottom right
                0 - m, 0 - m, 0 - m, // bottom left

                // left face
                0 - m, 0 + m, 0 + m, // top left
                0 - m, 0 + m, 0 - m, // top right
                0 - m, 0 - m, 0 - m, // bottom right
                0 - m, 0 - m, 0 + m, // bottom left

                // right face
                0 + m, 0 + m, 0 + m, // top left
                0 + m, 0 + m, 0 - m, // top right
                0 + m, 0 - m, 0 - m, // bottom right
                0 + m, 0 - m, 0 + m, // bottom left

                // top face
                0 - m, 0 + m, 0 + m, // top left
                0 - m, 0 + m, 0 - m, // top right
                0 + m, 0 + m, 0 - m, // bottom right
                0 + m, 0 + m, 0 + m, // bottom left

                // bottom face
                0 - m, 0 - m, 0 + m, // top left
                0 - m, 0 - m, 0 - m, // top right
                0 + m, 0 - m, 0 - m, // bottom right
                0 + m, 0 - m, 0 + m  // bottom left
        };

        color = new GLfloat[4 * 6 * 3]{
                255 / 255.0f, 154 / 255.0f, 162 / 255.0f,
                255 / 255.0f, 154 / 255.0f, 162 / 255.0f,
                255 / 255.0f, 154 / 255.0f, 162 / 255.0f,
                255 / 255.0f, 154 / 255.0f, 162 / 255.0f,

                255 / 255.0f, 183 / 255.0f, 178 / 255.0f,
                255 / 255.0f, 183 / 255.0f, 178 / 255.0f,
                255 / 255.0f, 183 / 255.0f, 178 / 255.0f,
                255 / 255.0f, 183 / 255.0f, 178 / 255.0f,

                255 / 255.0f, 218 / 255.0f, 193 / 255.0f,
                255 / 255.0f, 218 / 255.0f, 193 / 255.0f,
                255 / 255.0f, 218 / 255.0f, 193 / 255.0f,
                255 / 255.0f, 218 / 255.0f, 193 / 255.0f,

                226 / 255.0f, 240 / 255.0f, 203 / 255.0f,
                226 / 255.0f, 240 / 255.0f, 203 / 255.0f,
                226 / 255.0f, 240 / 255.0f, 203 / 255.0f,
                226 / 255.0f, 240 / 255.0f, 203 / 255.0f,


                181 / 255.0f, 234 / 255.0f, 215 / 255.0f,
                181 / 255.0f, 234 / 255.0f, 215 / 255.0f,
                181 / 255.0f, 234 / 255.0f, 215 / 255.0f,
                181 / 255.0f, 234 / 255.0f, 215 / 255.0f,


                199 / 255.0f, 206 / 255.0f, 234 / 255.0f,
                199 / 255.0f, 206 / 255.0f, 234 / 255.0f,
                199 / 255.0f, 206 / 255.0f, 234 / 255.0f,
                199 / 255.0f, 206 / 255.0f, 234 / 255.0f,
        };
    };

    void destroy() {
        if (vertices != NULL) {
            delete[] vertices;
        }
        if (color != NULL) {
            delete[] color;
        }
    }

    void draw() {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glColorPointer(3, GL_FLOAT, 0, color);

        glDrawArrays(GL_QUADS, 0, 4 * 6);

        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }

};

