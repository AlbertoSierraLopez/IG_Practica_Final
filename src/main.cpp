#include <iostream>
#include <ogl/glew.h>
#include <ogl/freeglut.h>

#include "Shaders.h"
#include "Model.h"

#include <sstream>
std::string toString(const int &i) {
    std::stringstream ss;
    ss << i;
    return ss.str();
}

#define I glm::mat4(1.0)

void funInit();
void funReshape(int w, int h);
void funDisplay();
void funSpecial(int key, int x, int y);
void funKeyboard(unsigned char key, int x, int y);
void funMouse(int button, int state, int x, int y);
void funMotion(int x, int y);

void setLights(glm::mat4 P, glm::mat4 V);

void drawAsfalto(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAuto(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawFaro(int index, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawChasis(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawLucesPosicion(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawRuedas(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawRueda(glm::mat4 P, glm::mat4 V, glm::mat4 M);

void drawObject(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);

// Shaders
Shaders shaders;

// Modelos
Model sphere;
Model plane;
Model cylinder;
Model cube;

// Luces y materiales
#define   NLF 2
Light     lightG;
Light     lightF[NLF];
Material  mluz;
Material  mluzR;
Material  ruby;
Material  obsidian;
Material  wPlastic;
Material  pSilver;

// Viewport
int w = 800;
int h = 600;

// Animaciones
float fovy   = 60.0;
float rotY   =  0.0;
float faroX  =  0.0;
float faroZ  =  0.0;
float alphaX =  0.0;
float alphaY =  0.0;

int main(int argc, char** argv) {

    // Inicializamos GLUT
    glutInit(&argc, argv);
    glutInitContextVersion(3,3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(w,h);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Informatica Grafica: Practica Final");

    // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return 0;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

    // Inicializaciones específicas
    funInit();

    // Configuración CallBacks
    glutReshapeFunc(funReshape);
    glutDisplayFunc(funDisplay);
    glutSpecialFunc(funSpecial);
    glutKeyboardFunc(funKeyboard);
    glutMouseFunc(funMouse);
    glutMotionFunc(funMotion);

    // Bucle principal
    glutMainLoop();

    return 0;
}

void funInit() {

    // Test de profundidad
    glEnable(GL_DEPTH_TEST);

    // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

    // Modelos
    sphere.initModel("resources/models/sphere.obj");
    plane.initModel("resources/models/plane.obj");
    cylinder.initModel("resources/models/cylinder.obj");
    cube.initModel("resources/models/cube.obj");


    // Luz ambiental global
    lightG.ambient        = glm::vec3(0.9, 0.9, 0.9);

    // Luces focales
    lightF[0].position    = glm::vec3(0.3,  0.3,  -0.8);
    lightF[0].direction   = glm::vec3( 0.0, -0.575, -3.0);
    lightF[0].ambient     = glm::vec3( 0.0,  0.0,  0.0);
    lightF[0].diffuse     = glm::vec3(  1.2,  1.2,  0.6);
    lightF[0].specular    = glm::vec3( 0.8,  0.8,  0.4);
    lightF[0].innerCutOff = 10.0;
    lightF[0].outerCutOff = lightF[0].innerCutOff + 5.0;
    lightF[0].c0          = 1.000;
    lightF[0].c1          = 0.090;
    lightF[0].c2          = 0.032;

    lightF[1].position    = glm::vec3(-0.3,  0.3,  -0.8);
    lightF[1].direction   = glm::vec3( 0.0, -0.575, -3.0);
    lightF[1].ambient     = glm::vec3( 0.0,  0.0,  0.0);
    lightF[1].diffuse     = glm::vec3(  1.2,  1.2,  0.6);
    lightF[1].specular    = glm::vec3( 0.8,  0.8,  0.4);
    lightF[1].innerCutOff = 10.0;
    lightF[1].outerCutOff = lightF[0].innerCutOff + 5.0;
    lightF[1].c0          = 1.000;
    lightF[1].c1          = 0.090;
    lightF[1].c2          = 0.032;

    // Materiales
    mluz.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.emissive  = glm::vec4(1.0, 1.0, 1.0, 1.0);
    mluz.shininess = 1.0;

    mluzR.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluzR.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluzR.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluzR.emissive  = glm::vec4(1.0, 0.0, 0.0, 1.0);
    mluzR.shininess = 1.0;

    // Ruby
    ruby.ambient   = glm::vec4(0.174500, 0.011750, 0.011750, 0.55);
    ruby.diffuse   = glm::vec4(0.614240, 0.041360, 0.041360, 0.55);
    ruby.specular  = glm::vec4(0.727811, 0.626959, 0.626959, 0.55);
    ruby.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    ruby.shininess = 76.8;

    // Obsidian
    obsidian.ambient   = glm::vec4(0.05375f, 0.05f, 0.06625f, 0.82f);
    obsidian.diffuse   = glm::vec4(0.18275f, 0.17f, 0.22525f, 0.82f);
    obsidian.specular  = glm::vec4(0.332741f, 0.328634f, 0.346435f, 0.82f);
    obsidian.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 0.82f);
    obsidian.shininess = 38.4f;

    // White Plastic
    wPlastic.ambient   = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f );
    wPlastic.diffuse   = glm::vec4(0.55f, 0.55f, 0.55f, 1.0f);
    wPlastic.specular  = glm::vec4(0.70f, 0.70f, 0.70f, 1.0f);
    wPlastic.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.0f);
    wPlastic.shininess = 32.0f ;

    // Polished Silver
    pSilver.ambient   = glm::vec4(0.23125f, 0.23125f, 0.23125f, 1.0f);
    pSilver.diffuse   = glm::vec4(0.2775f, 0.2775f, 0.2775f, 1.0f);
    pSilver.specular  = glm::vec4(0.773911f, 0.773911f, 0.773911f, 1.0f);
    pSilver.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.0f);
    pSilver.shininess = 89.6f ;

}

void funReshape(int wnew, int hnew) {

    // Configuración del Viewport
    glViewport(0, 0, wnew, hnew);

    // Actualización de w y h
    w = wnew;
    h = hnew;

}

void funDisplay() {

    // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Indicamos los shaders a utilizar
    shaders.useShaders();

    // Matriz P
    float nplane =  0.1;
    float fplane = 25.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P  = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

    // Matriz V
    float x = 10.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
    float y = 10.0f*glm::sin(glm::radians(alphaY));
    float z = 10.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    glm::vec3 pos   (  x,   y,   z);
    glm::vec3 lookat(0.0, 0.0, 0.0);
    glm::vec3 up    (0.0, 1.0, 0.0);
    glm::mat4 V = glm::lookAt(pos, lookat, up);
    shaders.setVec3("ucpos",pos);

    // Fijamos las luces
    setLights(P,V);

    // Dibujamos la escena
    drawAsfalto(P,V,I);

    glm::mat4 T = glm::translate(I,glm::vec3(faroX, 0.0, faroZ));
    glm::mat4 R = glm::rotate(I, glm::radians(rotY), glm::vec3(0, 1, 0));
    drawAuto(P,V,T*R);

    // Intercambiamos los buffers
    glutSwapBuffers();

}

void setLights(glm::mat4 P, glm::mat4 V) {

    shaders.setLight("ulightG",lightG);

    for(int i=0; i<NLF; i++) {
        // Luz
        Light lFaro = lightF[i];

        // Posicion
        lFaro.position =  glm::rotate(I, glm::radians(rotY), glm::vec3(0, 1, 0)) * glm::vec4(lFaro.position, 1.0);
        lFaro.position.x += faroX;
        lFaro.position.z += faroZ;

        // Direccion
        lFaro.direction =  glm::rotate(I, glm::radians(rotY), glm::vec3(0, 1, 0)) * glm::vec4(lFaro.direction, 1.0);

        shaders.setLight("ulightF["+toString(i)+"]",lFaro);
    }

}

void drawAsfalto(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(6.0, 1.0, 6.0));
    drawObject(plane, wPlastic, P, V, S);

}

void drawAuto(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    drawRuedas(P,V,M);

    drawChasis(P,V,M);

    drawLucesPosicion(P,V,M);

    glm::mat4 der = translate(I, glm::vec3(0.3,0.0,-0.8));
    glm::mat4 izq = translate(I, glm::vec3(-0.3,0.0,-0.8));
    drawFaro(0,P,V,M*der);
    drawFaro(1,P,V,M*izq);

}

void drawFaro(int index, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I,glm::vec3(0.0, lightF[index].position.y, 0.0));
    glm::mat4 S = glm::scale(I,glm::vec3(0.1/2.0, 0.1/2.0, 0.01/2.0));
    glm::mat4 Rini = glm::rotate(I, glm::radians(90.0f), glm::vec3(-1, 0, 0));;

    Material mFaro = mluz;
    mFaro.emissive = glm::vec4(lightF[index].diffuse, 1.0);

    drawObject(cylinder,mFaro,P,V,M*T*S*Rini);

}

void drawChasis(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = scale(I, glm::vec3(0.4,0.15,0.8));
    glm::mat4 T = translate(I, glm::vec3(0,0.3,0));
    drawObject(cube,pSilver,P,V,M*T*S);

    glm::mat4 Scab = scale(I, glm::vec3(0.38,0.15,0.55));
    glm::mat4 Tcab = translate(I, glm::vec3(0,0.6,0.2));
    drawObject(cube,pSilver,P,V,M*Tcab*Scab);

}

void drawLucesPosicion(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 Sf = scale(I, glm::vec3(0.1/2.0,0.08/2.0,0.01/2.0));
    glm::mat4 Tf1 =  translate(I, glm::vec3(0.3,0.35,0.8));
    glm::mat4 Tf2 =  translate(I, glm::vec3(-0.3,0.35,0.8));
    drawObject(cube, mluzR, P, V, M * Tf1 * Sf);
    drawObject(cube, mluzR, P, V, M * Tf2 * Sf);

}

void drawRuedas (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 Tru1 = translate(I, glm::vec3(0.4,0.15,0.6));
    glm::mat4 Tru2 = translate(I, glm::vec3(-0.4,0.15,0.6));
    glm::mat4 Tru3 = translate(I, glm::vec3(-0.4,0.15,-0.4));
    glm::mat4 Tru4 = translate(I, glm::vec3(0.4,0.15,-0.4));

    drawRueda(P, V, M * Tru1);
    drawRueda(P, V, M * Tru2);
    drawRueda(P, V, M * Tru3);
    drawRueda(P, V, M * Tru4);

}

void drawRueda(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = scale(I, glm::vec3(0.3/2.0,0.15/2.0,0.3/2.0));
    glm::mat4 R = rotate(I, glm::radians(90.0f), glm::vec3(0, 0, -1));
    drawObject(cylinder, obsidian, P, V, M * R * S);

}

void drawObject(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setMaterial("umaterial",material);
    model.renderModel(GL_FILL);

}

void funSpecial(int key, int x, int y) {

    switch(key) {
        case GLUT_KEY_LEFT:  rotY += 5.0f;   break;
        case GLUT_KEY_RIGHT: rotY -= 5.0f;   break;
        case GLUT_KEY_UP:    faroX -= 0.1 * sinf(glm::radians(rotY));
                             faroZ -= 0.1 * cosf(glm::radians(rotY));
                             break;
        case GLUT_KEY_DOWN:  faroX += 0.1 * sinf(glm::radians(rotY));
                             faroZ += 0.1 * cosf(glm::radians(rotY));
                             break;
    }
    glutPostRedisplay();

}

void funKeyboard(unsigned char key, int x, int y) {

    switch(key) {
        case ' ': rotY = 0.0f;
                  break;
    }
    glutPostRedisplay();

}

void funMouse(int button, int state, int x, int y) {

    switch(button) {
        case 3: fovy  -= fovy> 10.0f ? 1.0f : 0.0f; break;
        case 4: fovy  += fovy<110.0f ? 1.0f : 0.0f; break;
    }
    glutPostRedisplay();

}

void funMotion(int x, int y) {

    float limY = 89.0;
    alphaX = 90.0*(2.0*x/(float)w - 1.0);
    alphaY = 90.0*(1.0 - 2.0*y/(float)h);
    if(alphaY<-limY) alphaY = -limY;
    if(alphaY> limY) alphaY =  limY;
    glutPostRedisplay();

}