
struct Box {
    GLfloat *color;
    GLfloat *vertices;
    GLuint VAO, VBO;
    void init(GLfloat s) {
        destroy();
        vertices = new GLfloat[ 6 * 6 * 4] {
//                -s, s, s, 1.0f, 0.0f, 0.0f,
//                -s, -s, s, 1.0f, 0.0f, 0.0f,
//                s, s, s, 1.0f, 0.0f, 0.0f,
//                s, -s, s, 1.0f, 0.0f, 0.0f,

                -s, s, -s, 1.0f, 1.0f, 0.0f,
                -s, -s, -s, 1.0f, 1.0f, 0.0f,
                s, s, -s, 1.0f, 1.0f, 0.0f,
                s, -s, -s, 1.0f, 1.0f, 0.0f,

                -s, s, s, 0.0f, 1.0f, 0.0f,
                -s, -s, s, 0.0f, 1.0f, 0.0f,
                -s, s, -s, 0.0f, 1.0f, 0.0f,
                -s, -s, -s, 0.0f, 1.0f, 0.0f,

                s, s, s, 0.0f, 1.0f, 1.0f,
                s, -s, s, 0.0f, 1.0f, 1.0f,
                s, s, -s, 0.0f, 1.0f, 1.0f,
                s, -s, -s, 0.0f, 1.0f, 1.0f,

                -s, s, -s, 0.0f, 0.0f, 1.0f,
                -s, s, s, 0.0f, 0.0f, 1.0f,
                s, s, -s, 0.0f, 0.0f, 1.0f,
                s, s, s, 0.0f, 0.0f, 1.0f,

                -s, -s, -s, 1.0f, 0.0f, 1.0f,
                -s, -s, s, 1.0f, 0.0f, 1.0f,
                s, -s, -s, 1.0f, 0.0f, 1.0f,
                s, -s, s, 1.0f, 0.0f, 1.0f
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

    void initVAO() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, (6 * 6 * 4) * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glBindVertexArray(VAO);

        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void *) (0*sizeof(GLfloat)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void *) (3*sizeof(GLfloat)));
        glEnableVertexAttribArray(4);
    }

};

