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
void funTimer(int value);

void setLights(glm::mat4 P, glm::mat4 V);

void drawAstro(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAsfalto(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawUtilitario(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawTodoterreno(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawDeportivo(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawFaro(int index, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawChasisUtilitario(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawChasisTodoterreno(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawChasisDeportivo(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawLucesPosicion(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawLucesPosicionDeportivo(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawRuedasUtilitario(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawRuedasTodoterreno(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawRuedasDeportivo(glm::mat4 P, glm::mat4 V, glm::mat4 M);
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
#define   NLP 2
#define   NLF 2
Light     lightG;
Light     lightP[NLP];
Light     lightF[NLF];
Material  mluz;
Material  mluzR;
Material  mSol;
Material  mLuna;
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
float rotRueda = 0.0;
float rotAstro = 0.0;
int   speed  = 1000;
float alphaX =  0.0;
float alphaY =  0.0;

// Controles y Mundo
#define     NCOCHES 3
std::string coches[NCOCHES] = {"Utilitario", "Todoterreno", "Deportivo"};
int cocheSeleccionado = 0;
bool  dia   = true;

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
    glutTimerFunc(speed, funTimer, 0);

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


 // Luces Globales
    lightG.ambient        = glm::vec3(0.9, 0.9, 0.9);

 // Luces Posicionales
    // Sol
    lightP[0].position  = glm::vec3(8.0, 0.0, 0.0);
    lightP[0].ambient   = glm::vec3( 5.0, 0.2, 0.2);
    lightP[0].diffuse   = glm::vec3( 5.0, 0.2, 0.2);
    lightP[0].specular  = glm::vec3( 0.7, 0.7, 0.7);
    lightP[0].c0 = 1.0;
    lightP[0].c1 = 0.22;
    lightP[0].c2 = 0.20;
    // Luna
    lightP[1].position  = glm::vec3(-8.0, 0.0, 0.0);
    lightP[1].ambient   = glm::vec3( 0.2, 0.2, 5.0);
    lightP[1].diffuse   = glm::vec3( 0.2, 0.2, 5.0);
    lightP[1].specular  = glm::vec3( 0.7, 0.7, 0.7);
    lightP[1].c0 = 1.0;
    lightP[1].c1 = 0.22;
    lightP[1].c2 = 0.20;

 // Luces Focales
    // Faro Derecho
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
    // Faro Izquierdo
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

    mSol.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mSol.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mSol.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mSol.emissive  = glm::vec4( 5.0, 0.2, 0.2, 1.0);
    mSol.shininess = 1.0;

    mLuna.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mLuna.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mLuna.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mLuna.emissive  = glm::vec4( 0.2, 0.2, 5.0, 1.0);
    mLuna.shininess = 1.0;

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
    float x = 15.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
    float y = 15.0f*glm::sin(glm::radians(alphaY)) + 10.0;
    float z = 15.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    glm::vec3 pos   (  x,   y,   z);
    glm::vec3 lookat(0.0, 0.0, 0.0);
    glm::vec3 up    (0.0, 1.0, 0.0);
    glm::mat4 V = glm::lookAt(pos, lookat, up);
    shaders.setVec3("ucpos",pos);

    // Fijamos las luces
    setLights(P,V);

    // Dibujamos la escena
    drawAstro(P, V, I);
    drawAsfalto(P,V,I);

    glm::mat4 T = glm::translate(I,glm::vec3(faroX, 0.0, faroZ));
    glm::mat4 R = glm::rotate(I, glm::radians(rotY), glm::vec3(0, 1, 0));

    // Dibujar el coche seleccionado
    switch (cocheSeleccionado) {
        case 0:  drawUtilitario(P,V,T*R);  break;
        case 1:  drawTodoterreno(P,V,T*R); break;
        case 2:  drawDeportivo(P,V,T*R);   break;
        default: drawUtilitario(P,V,T*R);
    }

    // Intercambiamos los buffers
    glutSwapBuffers();

}

void setLights(glm::mat4 P, glm::mat4 V) {

 // Lucz Globales
    shaders.setLight("ulightG",lightG);

 // Luz Posicional
    // Luz
    Light lAstro;
    if (dia) {
        lAstro = lightP[0];
    } else {
        lAstro = lightP[1];
    }
    // Posicion
    glm::mat4 rotLuz = glm::rotate(I, glm::radians(rotAstro), glm::vec3(0, 0, 1));
    glm::vec4 posLuz = glm::vec4(lAstro.position, 1.0);
    lAstro.position = rotLuz * posLuz;

    shaders.setLight("ulightP", lAstro);

 // Luces Focales
    for (int i=0; i<NLF; i++) {
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

void drawAstro(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    Light lAstro;
    Material mAstro;
    float scale = 0.0;

    if (dia) {
        lAstro = lightP[0];
        mAstro = mSol;
        scale = 0.3;
    } else {
        lAstro = lightP[1];
        mAstro = mLuna;
        scale = 0.15;
    }

    glm::mat4 S = glm::scale(I, glm::vec3(scale));
    glm::mat4 T = glm::translate(I, lAstro.position);
    glm::mat4 R = glm::rotate(I, glm::radians(rotAstro), glm::vec3(0, 0, 1));
    drawObject(sphere, mAstro, P, V, M*R*T*S);
}

void drawAsfalto(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(6.0, 1.0, 6.0));
    drawObject(plane, wPlastic, P, V, S);

}

void drawUtilitario(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    drawRuedasUtilitario(P,V,M);

    drawChasisUtilitario(P,V,M);

    drawLucesPosicion(P,V,M);

    glm::mat4 der = translate(I, glm::vec3(0.3,0.0,-0.8));
    glm::mat4 izq = translate(I, glm::vec3(-0.3,0.0,-0.8));
    drawFaro(0,P,V,M*der);
    drawFaro(1,P,V,M*izq);

}

void drawTodoterreno(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    drawRuedasTodoterreno(P,V,M);

    drawChasisTodoterreno(P,V,M);

    drawLucesPosicion(P,V,M);

    glm::mat4 der = translate(I, glm::vec3(0.3,0.0,-0.8));
    glm::mat4 izq = translate(I, glm::vec3(-0.3,0.0,-0.8));
    drawFaro(0,P,V,M*der);
    drawFaro(1,P,V,M*izq);

}

void drawDeportivo(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    drawRuedasDeportivo(P,V,M);

    drawChasisDeportivo(P,V,M);

    drawLucesPosicionDeportivo(P,V,M);

    glm::mat4 der = translate(I, glm::vec3(0.3,0.0,-0.8));
    glm::mat4 izq = translate(I, glm::vec3(-0.3,0.0,-0.8));
    drawFaro(0,P,V,M*der);
    drawFaro(1,P,V,M*izq);

}

void drawFaro(int index, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I,glm::vec3(0.0, lightF[index].position.y, 0.0));
    glm::mat4 S = glm::scale(I,glm::vec3(0.1/2.0, 0.1/2.0, 0.01/2.0));
    glm::mat4 Rini = glm::rotate(I, glm::radians(90.0f), glm::vec3(-1, 0, 0));

    Material mFaro = mluz;
    mFaro.emissive = glm::vec4(lightF[index].diffuse, 1.0);

    drawObject(cylinder,mFaro,P,V,M*T*S*Rini);

}

void drawChasisUtilitario (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    //base turismo y todoterreno
    glm::mat4 S = scale(I, glm::vec3(0.4,0.15,0.8));
    glm::mat4 T = translate(I, glm::vec3(0,0.3,0));
    drawObject(cube,pSilver,P,V,M*T*S);

    //coche normal familiar
    glm::mat4 Scab = scale(I, glm::vec3(0.38,0.15,0.55));
    glm::mat4 Tcab = translate(I, glm::vec3(0,0.6,0.2));
    drawObject(cube,pSilver,P,V,M*Tcab*Scab);
}

void drawChasisTodoterreno (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    //base turismo y todoterreno
    glm::mat4 S = scale(I, glm::vec3(0.4,0.2,0.8));
    glm::mat4 T = translate(I, glm::vec3(0,0.3,0));
    drawObject(cube,pSilver,P,V,M*T*S);

    //coche todoterreno
    glm::mat4 Scab = scale(I, glm::vec3(0.4,0.30,0.55));
    glm::mat4 Tcab = translate(I, glm::vec3(0,0.6,0.25));
    drawObject(cube,pSilver,P,V,M*Tcab*Scab);

    glm::mat4 SR = scale(I, glm::vec3(0.4/2.0,0.15/2.0,0.4/2.0));
    glm::mat4 RR = rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
    glm::mat4 TR = translate(I, glm::vec3(0,0.55,0.9));
    drawObject(cylinder, obsidian, P, V, M * TR * RR * SR);
}

void drawChasisDeportivo (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    //base coche deportivo
    glm::mat4 S1 = scale(I, glm::vec3(0.4,0.02,0.78));
    glm::mat4 T1 = translate(I, glm::vec3(0,0.10,-0.15));
    drawObject(cube,pSilver,P,V,M*T1*S1);

    glm::mat4 S2 = scale(I, glm::vec3(0.4,0.02,0.82));
    glm::mat4 T2 = translate(I, glm::vec3(0,0.12,-0.15));
    drawObject(cube,pSilver,P,V,M*T2*S2);

    glm::mat4 S3 = scale(I, glm::vec3(0.4,0.02,0.84));
    glm::mat4 T3 = translate(I, glm::vec3(0,0.14,-0.15));
    drawObject(cube,pSilver,P,V,M*T3*S3);

    glm::mat4 S4 = scale(I, glm::vec3(0.4,0.02,0.88));
    glm::mat4 T4 = translate(I, glm::vec3(0,0.16,-0.15));
    drawObject(cube,pSilver,P,V,M*T4*S4);

    glm::mat4 S5 = scale(I, glm::vec3(0.4,0.02,0.88));
    glm::mat4 T5 = translate(I, glm::vec3(0,0.18,-0.15));
    drawObject(cube,pSilver,P,V,M*T5*S5);

    glm::mat4 S6 = scale(I, glm::vec3(0.4,0.02,0.88));
    glm::mat4 T6 = translate(I, glm::vec3(0,0.2,-0.15));
    drawObject(cube,pSilver,P,V,M*T6*S6);

    glm::mat4 S7 = scale(I, glm::vec3(0.4,0.02,0.84));
    glm::mat4 T7 = translate(I, glm::vec3(0,0.22,-0.15));
    drawObject(cube,pSilver,P,V,M*T7*S7);

    glm::mat4 S8 = scale(I, glm::vec3(0.4,0.02,0.80));
    glm::mat4 T8 = translate(I, glm::vec3(0,0.24,-0.15));
    drawObject(cube,pSilver,P,V,M*T8*S8);

    glm::mat4 SFaro = scale(I, glm::vec3(0.05,0.05,0.05));
    glm::mat4 TFaro1 = translate(I, glm::vec3(0.3,0.3,-0.75));
    drawObject(cube,pSilver,P,V,M*TFaro1*SFaro);
    glm::mat4 TFaro2 = translate(I, glm::vec3(-0.3,0.3,-0.75));
    drawObject(cube,pSilver,P,V,M*TFaro2*SFaro);

    //cabina base
    glm::mat4 SCB1 = scale(I, glm::vec3(0.4,0.02,0.6));
    glm::mat4 TCB1 = translate(I, glm::vec3(0,0.26,0.02));
    drawObject(cube,pSilver,P,V,M*TCB1*SCB1);

    glm::mat4 SCB2 = scale(I, glm::vec3(0.4,0.02,0.58));
    glm::mat4 TCB2 = translate(I, glm::vec3(0,0.28,0.03));
    drawObject(cube,pSilver,P,V,M*TCB2*SCB2);

    //cabina
    glm::mat4 SC1 = scale(I, glm::vec3(0.4,0.02,0.55));
    glm::mat4 TC1 = translate(I, glm::vec3(0,0.3,0.04));
    drawObject(cube,pSilver,P,V,M*TC1*SC1);

    glm::mat4 SC2 = scale(I, glm::vec3(0.38,0.02,0.53));
    glm::mat4 TC2 = translate(I, glm::vec3(0,0.32,0.04));
    drawObject(cube,pSilver,P,V,M*TC2*SC2);

    glm::mat4 SC3 = scale(I, glm::vec3(0.36,0.02,0.51));
    glm::mat4 TC3 = translate(I, glm::vec3(0,0.34,0.04));
    drawObject(cube,pSilver,P,V,M*TC3*SC3);

    glm::mat4 SC4 = scale(I, glm::vec3(0.34,0.02,0.49));
    glm::mat4 TC4 = translate(I, glm::vec3(0,0.36,0.04));
    drawObject(cube,pSilver,P,V,M*TC4*SC4);

    glm::mat4 SC5 = scale(I, glm::vec3(0.32,0.02,0.47));
    glm::mat4 TC5 = translate(I, glm::vec3(0,0.38,0.04));
    drawObject(cube,pSilver,P,V,M*TC5*SC5);

    glm::mat4 SC6 = scale(I, glm::vec3(0.3,0.02,0.45));
    glm::mat4 TC6 = translate(I, glm::vec3(0,0.4,0.04));
    drawObject(cube,pSilver,P,V,M*TC6*SC6);

    //aleron base
    glm::mat4 SA = scale(I, glm::vec3(0.42,0.005,0.05));
    glm::mat4 TA = translate(I, glm::vec3(0,0.45,0.75));
    drawObject(cube,pSilver,P,V,M*TA*SA);

    //aleron lados
    glm::mat4 SAT = scale(I, glm::vec3(0.005,0.02,0.04));
    glm::mat4 TAD1 = translate(I, glm::vec3(0.3,0.43,0.74));
    drawObject(cube,pSilver,P,V,M*TAD1*SAT);
    glm::mat4 TAD2 = translate(I, glm::vec3(0.3,0.41,0.72));
    drawObject(cube,pSilver,P,V,M*TAD2*SAT);
    glm::mat4 TAD3 = translate(I, glm::vec3(0.3,0.39,0.70));
    drawObject(cube,pSilver,P,V,M*TAD3*SAT);
    glm::mat4 TAD4 = translate(I, glm::vec3(0.3,0.37,0.68));
    drawObject(cube,pSilver,P,V,M*TAD4*SAT);
    glm::mat4 TAD5 = translate(I, glm::vec3(0.3,0.35,0.66));
    drawObject(cube,pSilver,P,V,M*TAD5*SAT);
    glm::mat4 TAD6 = translate(I, glm::vec3(0.3,0.33,0.64));
    drawObject(cube,pSilver,P,V,M*TAD6*SAT);
    glm::mat4 TAD7 = translate(I, glm::vec3(0.3,0.31,0.62));
    drawObject(cube,pSilver,P,V,M*TAD7*SAT);
    glm::mat4 TAD8 = translate(I, glm::vec3(0.3,0.29,0.6));
    drawObject(cube,pSilver,P,V,M*TAD8*SAT);
    glm::mat4 TAI1 = translate(I, glm::vec3(-0.3,0.43,0.74));
    drawObject(cube,pSilver,P,V,M*TAI1*SAT);
    glm::mat4 TAI2 = translate(I, glm::vec3(-0.3,0.41,0.72));
    drawObject(cube,pSilver,P,V,M*TAI2*SAT);
    glm::mat4 TAI3 = translate(I, glm::vec3(-0.3,0.39,0.70));
    drawObject(cube,pSilver,P,V,M*TAI3*SAT);
    glm::mat4 TAI4 = translate(I, glm::vec3(-0.3,0.37,0.68));
    drawObject(cube,pSilver,P,V,M*TAI4*SAT);
    glm::mat4 TAI5 = translate(I, glm::vec3(-0.3,0.35,0.66));
    drawObject(cube,pSilver,P,V,M*TAI5*SAT);
    glm::mat4 TAI6 = translate(I, glm::vec3(-0.3,0.33,0.64));
    drawObject(cube,pSilver,P,V,M*TAI6*SAT);
    glm::mat4 TAI7 = translate(I, glm::vec3(-0.3,0.31,0.62));
    drawObject(cube,pSilver,P,V,M*TAI7*SAT);
    glm::mat4 TAI8 = translate(I, glm::vec3(-0.3,0.29,0.6));
    drawObject(cube,pSilver,P,V,M*TAI8*SAT);

}

void drawLucesPosicion (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 Sf = scale(I, glm::vec3(0.1/2.0,0.08/2.0,0.01/2.0));

    //luces traseras turismo
    glm::mat4 Tf1 =  translate(I, glm::vec3(0.3,0.35,0.8));
    glm::mat4 Tf2 =  translate(I, glm::vec3(-0.3,0.35,0.8));

    drawObject(cube, mluzR, P, V, M * Tf1 * Sf);
    drawObject(cube, mluzR, P, V, M * Tf2 * Sf);

}

void drawLucesPosicionDeportivo (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 Sf = scale(I, glm::vec3(0.1/2.0,0.08/2.0,0.01/2.0));

    //luces traseras deportivo
    glm::mat4 Tf1 =  translate(I, glm::vec3(0.3,0.18,0.73));
    glm::mat4 Tf2 =  translate(I, glm::vec3(-0.3,0.18,0.73));

    drawObject(cube, mluzR, P, V, M * Tf1 * Sf);
    drawObject(cube, mluzR, P, V, M * Tf2 * Sf);

}

void drawRuedasUtilitario (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    //ruedas turismo
    glm::mat4 Tru1 = translate(I, glm::vec3(0.4,0.15,0.6));
    glm::mat4 Tru2 = translate(I, glm::vec3(-0.4,0.15,0.6));
    glm::mat4 Tru3 = translate(I, glm::vec3(-0.4,0.15,-0.4));
    glm::mat4 Tru4 = translate(I, glm::vec3(0.4,0.15,-0.4));

    // Hacer girar las ruedas al moverse el coche
    glm::mat4 R = glm::rotate(I, glm::radians(rotRueda), glm::vec3(1, 0, 0));

    drawRueda(P, V, M * Tru1 * R);
    drawRueda(P, V, M * Tru2 * R);
    drawRueda(P, V, M * Tru3 * R);
    drawRueda(P, V, M * Tru4 * R);

}

void drawRuedasTodoterreno (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    //ruedas todoterreno
    glm::mat4 Tru1 = translate(I, glm::vec3(0.4,0.21,0.55));
    glm::mat4 Tru2 = translate(I, glm::vec3(-0.4,0.21,0.55));
    glm::mat4 Tru3 = translate(I, glm::vec3(-0.4,0.21,-0.4));
    glm::mat4 Tru4 = translate(I, glm::vec3(0.4,0.21,-0.4));

    // Rueda gorda
    glm::mat4 S = scale(I, glm::vec3(1.15,1.4,1.4));

    // Hacer girar las ruedas al moverse el coche
    glm::mat4 R = glm::rotate(I, glm::radians(rotRueda), glm::vec3(1, 0, 0));

    drawRueda(P, V, M * Tru1 * R * S);
    drawRueda(P, V, M * Tru2 * R * S);
    drawRueda(P, V, M * Tru3 * R * S);
    drawRueda(P, V, M * Tru4 * R * S);

}

void drawRuedasDeportivo (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    //ruedas deportivo
    glm::mat4 Tru1 = translate(I, glm::vec3(0.4,0.15,0.5));
    glm::mat4 Tru2 = translate(I, glm::vec3(-0.4,0.15,0.5));
    glm::mat4 Tru3 = translate(I, glm::vec3(-0.4,0.15,-0.4));
    glm::mat4 Tru4 = translate(I, glm::vec3(0.4,0.15,-0.4));

    // Hacer girar las ruedas al moverse el coche
    glm::mat4 R = glm::rotate(I, glm::radians(rotRueda), glm::vec3(1, 0, 0));

    drawRueda(P, V, M * Tru1 * R);
    drawRueda(P, V, M * Tru2 * R);
    drawRueda(P, V, M * Tru3 * R);
    drawRueda(P, V, M * Tru4 * R);

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
                             rotRueda += 10.0;
                             break;
        case GLUT_KEY_DOWN:  faroX += 0.1 * sinf(glm::radians(rotY));
                             faroZ += 0.1 * cosf(glm::radians(rotY));
                             rotRueda -= 10.0;
                             break;
    }

    glutPostRedisplay();

}

void funKeyboard(unsigned char key, int x, int y) {

    switch(key) {
        case ' ': cocheSeleccionado++;
                  break;
    }

    if (cocheSeleccionado > 2) {
        cocheSeleccionado = 0;
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

void funTimer(int value) {

    rotAstro += 5.0;
    if (rotAstro > 360.0) rotAstro = 0.0;

    if (rotAstro < 181.0) {
        dia = true;
    } else {
        dia = false;
    }

    glutPostRedisplay();
    glutTimerFunc(speed, funTimer, 0);

}
