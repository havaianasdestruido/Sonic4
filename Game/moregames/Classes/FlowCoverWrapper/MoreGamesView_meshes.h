

#define IMG_VERTEX_NUM (6)

#define DEFAULT_ASPECT_YonX (0.75f)


GLfloat* createMGMesh(float AspectYonX) {
    float Y_SIZE =  DEFAULT_ASPECT_YonX;
    float X_SIZE =  DEFAULT_ASPECT_YonX/AspectYonX;
    const GLfloat data[IMG_VERTEX_NUM * 3] = //mesh
    {
        0.000000,-Y_SIZE,-X_SIZE,
        0.000000, Y_SIZE, X_SIZE,
        0.000000,-Y_SIZE, X_SIZE,
        0.000000, Y_SIZE, X_SIZE,
        0.000000,-Y_SIZE,-X_SIZE,
        0.000000, Y_SIZE,-X_SIZE,
    };
    GLfloat* res = (GLfloat*)malloc(sizeof(data));
    memcpy(res, data, sizeof(data));
    return res;
    
}

GLfloat* createMGTexCoords(float AspectYonX) {
    float TXCS = AspectYonX;    
	float TXCDY = ((1.f-TXCS)/2.f);
    const GLfloat data[IMG_VERTEX_NUM * 2] = //tc
    {
        0.0, 1.0 - (1.0*TXCS + TXCDY),
        1.0, 1.0 - (0.0*TXCS + TXCDY),
        1.0, 1.0 - (1.0*TXCS + TXCDY),
        1.0, 1.0 - (0.0*TXCS + TXCDY),
        0.0, 1.0 - (1.0*TXCS + TXCDY),
        0.0, 1.0 - (0.0*TXCS + TXCDY),
    };
    GLfloat* res = (GLfloat*)malloc(sizeof(data));
    memcpy(res, data, sizeof(data));
    return res;    
}



	


	

