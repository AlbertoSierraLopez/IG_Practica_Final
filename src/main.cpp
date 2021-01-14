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

void drawCarretera(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAsfalto(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawFarola(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawFarolas(glm::mat4 P, glm::mat4 V, glm::mat4 M);


void drawOficina(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawEdificio(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawRestaurante(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawColumnas(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawColumna(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawParque(glm::mat4 P, glm::mat4 V, glm::mat4 M);

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
#define   NCOCHES 2

Light     lightG;
Light     lightP[NLP];
Light     lightF[NCOCHES*2];
Material  mluz;
Material  mluzoff;
Material  mluzR;
Material  mSol;
Material  mLuna;
Material  ruby;
Material xgreen;
Material  obsidian;
Material  wPlastic;
Material  pSilver;


// Viewport
int w = 800;
int h = 600;

// Animaciones
float fovy   = 60.0;

float rotY     = 0.0;
float faroX    = 0.0;
float faroZ    = 0.0;
float rotRueda = 0.0;

float rotAstro = 0.0;
int   speed    = 1000;

int npcDir     = 0;     // Indice del array npcControl[]
float npcTimer = 0.0;   // Temporizador, va anclado a la longitud de las carreteras (7)
float npcRot   = 90.0;
float npcControl[4] = {0.0, 0.0, 0.0, 0.0};
//                      X    Z   -X   -Z

// Ventanas aleatoriamente iluminadas
#define     NVENTANAS 14
int randomBooleanArray[NVENTANAS];

float alphaX =  0.0;
float alphaY =  0.0;

// Controles y Mundo
#define     NCOCHEST 3
std::string coches[NCOCHEST] = {"Utilitario", "Todoterreno", "Deportivo"};
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
    lightP[0].position  = glm::vec3(16.0,0.0, 0.0);
    lightP[0].ambient   = glm::vec3(2.53,2.04,1.08);
    lightP[0].diffuse   = glm::vec3(2.53,2.04,1.08);
    lightP[0].specular  = glm::vec3(0.7,0.7,  0.7);
    lightP[0].c0 = 1.0;
    lightP[0].c1 = 0.06;
    lightP[0].c2 = 0.02;
    // Luna
    lightP[1].position  = glm::vec3(-16.0,0.0, 0.0);
    lightP[1].ambient   = glm::vec3(1.94, 1.97,2.04);
    lightP[1].diffuse   = glm::vec3(1.94, 1.97,2.04);
    lightP[1].specular  = glm::vec3(0.5,  0.5, 0.5);
    lightP[1].c0 = 1.0;
    lightP[1].c1 = 0.18;
    lightP[1].c2 = 0.08;

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

    mluzoff.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluzoff.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluzoff. specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluzoff. emissive  = glm::vec4(0.3, 0.3, 0.3, 0.3);
    mluzoff. shininess = 1.0;

    mluzR.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluzR.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluzR.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluzR.emissive  = glm::vec4(1.0, 0.0, 0.0, 1.0);
    mluzR.shininess = 1.0;

    mSol.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mSol.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mSol.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mSol.emissive  = glm::vec4(253.0/255.0,204.0/255.0,108.0/255.0,1.0);
    mSol.shininess = 1.0;

    mLuna.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mLuna.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mLuna.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mLuna.emissive  = glm::vec4(194.0/255.0,197.0/255.0,204.0/255.0,1.0);
    mLuna.shininess = 1.0;

    // Ruby
    ruby.ambient   = glm::vec4(0.174500, 0.011750, 0.011750, 0.55);
    ruby.diffuse   = glm::vec4(0.614240, 0.041360, 0.041360, 0.55);
    ruby.specular  = glm::vec4(0.727811, 0.626959, 0.626959, 0.55);
    ruby.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    ruby.shininess = 76.8;
    // X
    xgreen.ambient   = glm::vec4(0.011750,0.174500 , 0.011750, 0.55);
    xgreen.diffuse   = glm::vec4(0.041360,0.614240 , 0.041360, 0.55);
    xgreen.specular  = glm::vec4(0.626959, 0.727811, 0.626959, 0.55);
    xgreen .emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    xgreen .shininess = 56.8;

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

    // Inicializar array de booleanos aleatorios para las ventanas
    for (int i = 0; i < NVENTANAS; i++) {
        randomBooleanArray[i] = rand() % 2;
    }

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
    float fplane = 50.0;
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

    // Dibujar Carreteras
    //glm::mat4 S = glm::scale(I, glm::vec3(1.0));
    drawCarretera(P,V,I);
    drawAsfalto(P,V,I);
    drawFarolas(P,V,I);

    // Dibujar Edificios
    glm::mat4 TEdificio   = glm::translate(I, glm::vec3(3.5, 0.0, 3.8));
    drawEdificio(P,V,I * TEdificio);
    glm::mat4 TOficina = glm::translate(I,glm::vec3(-3.5,0,-3.8));
    drawOficina(P,V,I * TOficina);
    glm::mat4 TParque = glm::translate(I,glm::vec3(3.5,0,-3.5));
    drawParque(P,V,I * TParque);

    // Dibujar Coches
    glm::mat4 Tcoche = glm::translate(I,glm::vec3(faroX, 0.05, faroZ));
    glm::mat4 Rcoche = glm::rotate(I, glm::radians(rotY), glm::vec3(0, 1, 0));

    // Coche Seleccionado
    switch (cocheSeleccionado) {
        case 0:  drawUtilitario(P,V,Tcoche*Rcoche);  break;
        case 1:  drawTodoterreno(P,V,Tcoche*Rcoche); break;
        case 2:  drawDeportivo(P,V,Tcoche*Rcoche);   break;
        default: drawUtilitario(P,V,Tcoche*Rcoche);
    }

    // Coche npc
    glm::mat4 Tnpc = glm::translate(I,glm::vec3(npcControl[0]-npcControl[2], 0.05, npcControl[1]-npcControl[3]));
    glm::mat4 Rnpc = glm::rotate(I, glm::radians(npcRot), glm::vec3(0, -1, 0));
    drawUtilitario(P,V,Tnpc*Rnpc);

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
    // Focos coche principal
    for (int i=0; i<2; i++) {
        // Luz
        Light lFaro = lightF[i];

        // Posicion
        lFaro.position =  glm::rotate(I, glm::radians(rotY), glm::vec3(0, 1, 0)) * glm::vec4(lFaro.position, 1.0);
        lFaro.position.x += faroX;
        lFaro.position.z += faroZ;

        // Direccion
        lFaro.direction =  glm::rotate(I, glm::radians(rotY), glm::vec3(0, 1, 0)) * glm::vec4(lFaro.direction, 1.0);

        // Intensidad
        if (dia) {
            lFaro.diffuse  = glm::vec3(0.0);
            lFaro.specular = glm::vec3(0.0);
        }

        shaders.setLight("ulightF["+toString(i)+"]",lFaro);
    }

    // Focos coches npc
    for (int i=2; i<NCOCHES*2; i++) {
        // Luz
        Light lFaro;
        if (i % 2 == 0) {
            lFaro = lightF[0];  // Faro derecho
        } else {
            lFaro = lightF[1];  // Faro izquierdo
        }


        // Posicion
        lFaro.position   = glm::rotate(I, glm::radians(npcRot), glm::vec3(0, -1, 0)) * glm::vec4(lFaro.position, 1.0);
        lFaro.position.x += npcControl[0] - npcControl[2];
        lFaro.position.z += npcControl[1] - npcControl[3];

        // Direccion
        lFaro.direction =  glm::rotate(I, glm::radians(npcRot), glm::vec3(0, -1, 0)) * glm::vec4(lFaro.direction, 1.0);

        // Intensidad
        if (dia) {
            lFaro.diffuse  = glm::vec3(0.0);
            lFaro.specular = glm::vec3(0.0);
        }

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
        scale = 1.0;
    } else {
        lAstro = lightP[1];
        mAstro = mLuna;
        scale = 0.25;
    }

    glm::mat4 S = glm::scale(I, glm::vec3(scale));
    glm::mat4 T = glm::translate(I, lAstro.position);
    glm::mat4 R = glm::rotate(I, glm::radians(rotAstro), glm::vec3(0, 0, 1));
    drawObject(sphere, mAstro, P, V, M*R*T*S);
}

void drawCarretera(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(1.0, 0.025, 8.0));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 0.025, 0.0));
    glm::mat4 R = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 1, 0));

    glm::mat4 Tizquierda = glm::translate(I, glm::vec3(7.0, 0.0, 0.0));
    glm::mat4 Tderecha    = glm::translate(I, glm::vec3(-7.0, 0.0, 0.0));
    glm::mat4 Tarriba     = glm::translate(I, glm::vec3(0.0, 0.0, 7.0));
    glm::mat4 Tabajo      = glm::translate(I, glm::vec3(0.0, 0.0, -7.0));

    drawObject(cube, wPlastic, P, V, M * T * S);
    drawObject(cube, wPlastic, P, V, M * Tizquierda * T * S);
    drawObject(cube, wPlastic, P, V, M * Tderecha * T * S);

    drawObject(cube, wPlastic, P, V, M * R * T * S);
    drawObject(cube, wPlastic, P, V, M * Tarriba * R * T * S);
    drawObject(cube, wPlastic, P, V, M * Tabajo * R * T * S);

}
void drawParque(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 S = glm::scale(I, glm::vec3(2.5, 0.025, 2.5));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 0.025, 0.0));
    drawObject(cube,xgreen,P,V,M*T*S);
}
void drawFarolas(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 TOficina = glm::translate(I, glm::vec3(-1.0, 0.0, -1.0));
    glm::mat4 TOficina2 = glm::translate(I, glm::vec3(-1.0, 0.0, -6.0));
    glm::mat4 TOficina3 = glm::translate(I, glm::vec3(-6.0, 0.0, -1.0));
    glm::mat4 THotel    = glm::translate(I, glm::vec3(1.0, 0.0, 1.0));
    glm::mat4 THotel2    = glm::translate(I, glm::vec3(1.0, 0.0, 6.0));
    glm::mat4 THotel3    = glm::translate(I, glm::vec3(6.0, 0.0, 1.0));
    glm::mat4 TParque    = glm::translate(I, glm::vec3(1.0, 0.0, -1.0));
    glm::mat4 TParque2   = glm::translate(I, glm::vec3(1.0, 0.0, -6.0));
    glm::mat4 TParque3   = glm::translate(I, glm::vec3(6.0, 0.0, -1.0));
    glm::mat4 TParque4   = glm::translate(I, glm::vec3(3.5, 0.0, -3.5));
    glm::mat4 TRestaurante      = glm::translate(I, glm::vec3(-1.0, 0.0, 1.0));
    glm::mat4 TRestaurante2      = glm::translate(I, glm::vec3(-1.0, 0.0, 6.0));
    glm::mat4 TRestaurante3      = glm::translate(I, glm::vec3(-6.0, 0.0, 1.0));
    drawFarola(P,V,M * TOficina);
    drawFarola(P,V,M * TOficina2);
    drawFarola(P,V,M * TOficina3);
    drawFarola(P,V,M * THotel);
    drawFarola(P,V,M * THotel2);
    drawFarola(P,V,M * THotel3);
    drawFarola(P,V,M * TParque);
    drawFarola(P,V,M * TParque2);
    drawFarola(P,V,M * TParque3);
    drawFarola(P,V,M * TParque4);
    drawFarola(P,V,M * TRestaurante);
    drawFarola(P,V,M * TRestaurante2);
    drawFarola(P,V,M * TRestaurante3);
}
void drawAsfalto(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
        glm::mat4 S = glm::scale(I, glm::vec3(6.0, 1.0, 6.0));
        drawObject(plane, obsidian, P, V, M * S);

        glm::mat4 R = glm::rotate   (I, glm::radians(180.0f), glm::vec3(1, 0, 0));
        drawObject(plane, obsidian, P, V, M * S * R);
    glDisable(GL_CULL_FACE);

}
void drawOficina(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S   = glm::scale(I, glm::vec3(2.0, 0.1, 2.0));
    glm::mat4 T   = glm::translate(I, glm::vec3(0, 0.1, 0));
    glm::mat4 TP1 = glm::translate(I, glm::vec3(0, 1.3, 0));
    glm::mat4 TP2 = glm::translate(I, glm::vec3(0, 2.5, 0));
    drawObject(cube, ruby, P, V, M * T * S);
    drawObject(cube, ruby, P, V, M * TP1 * S);
    drawObject(cube, ruby, P, V, M * TP2 * S);

    glm::mat4 SColumna   = glm::scale(I, glm::vec3(0.2, 1.2, 0.2));
    glm::mat4 TColumna1 = glm::translate(I, glm::vec3(2.0 - 0.2, 1.2, 2.0 - 0.2));
    glm::mat4 TColumna2 = glm::translate(I, glm::vec3(-2.0 + 0.2, 1.2, 2.0 - 0.2));
    glm::mat4 TColumna3 = glm::translate(I, glm::vec3(2.0 - 0.2, 1.2, -2.0+ 0.2));
    glm::mat4 TColumna4 = glm::translate(I, glm::vec3(-2.0 + 0.2, 1.2, -2.0+ 0.2));

    drawObject(cube, ruby, P, V, M * TColumna1 * SColumna);
    drawObject(cube, ruby, P, V, M * TColumna2 * SColumna);
    drawObject(cube, ruby, P, V, M * TColumna3 * SColumna);
    drawObject(cube, ruby, P, V, M * TColumna4 * SColumna);

    glm::mat4 TColumna11 = glm::translate(I, glm::vec3(2.0 - 0.2, 1.2, 2.0 - 1.6));
    glm::mat4 TColumna22 = glm::translate(I, glm::vec3(-2.0 + 0.2, 1.2, 2.0 - 1.6));
    glm::mat4 TColumna33 = glm::translate(I, glm::vec3(2.0 - 0.2, 1.2, -2.0+ 1.6));
    glm::mat4 TColumna44 = glm::translate(I, glm::vec3(-2.0 + 0.2, 1.2, -2.0+ 1.6));
    glm::mat4 TColumna33c = glm::translate(I, glm::vec3(2.0 - 0.2, 1.2, -2.0+ 1.8));
    glm::mat4 TColumna44c = glm::translate(I, glm::vec3(-2.0 + 0.2, 1.2, -2.0+ 1.8));
    glm::mat4 SColumnac   = glm::scale(I, glm::vec3(0.2, 1.2, 0.4));

    drawObject(cube, ruby, P, V, M * TColumna11 * SColumna);
    drawObject(cube, ruby, P, V, M * TColumna22 * SColumna);
    drawObject(cube, ruby, P, V, M * TColumna33 * SColumna);
    drawObject(cube, ruby, P, V, M * TColumna44 * SColumna);
    drawObject(cube, ruby, P, V, M * TColumna33c * SColumnac);
    drawObject(cube, ruby, P, V, M * TColumna44c * SColumnac);

    glm::mat4 TColumna111 = glm::translate(I, glm::vec3(2.0 - 1.6, 1.9, 2.0 - 0.2));
    glm::mat4 TColumna222 = glm::translate(I, glm::vec3(-2.0 + 1.6, 1.9, 2.0 - 0.2));
    glm::mat4 TColumna333 = glm::translate(I, glm::vec3(2.0 - 1.6, 1.2, -2.0+ 0.2));
    glm::mat4 TColumna444 = glm::translate(I, glm::vec3(-2.0 + 1.6, 1.2, -2.0+ 0.2));
    glm::mat4 TColumna333c = glm::translate(I, glm::vec3(2.0 - 1.8, 1.2, -2.0+ 0.2));
    glm::mat4 TColumna111ac = glm::translate(I, glm::vec3(2.0 - 1.8, 1.9, 2.0 - 0.2));
    glm::mat4 TColumna111D = glm::translate(I, glm::vec3(2.0 - 1.0, 0.7, 2.0 - 0.2));
    glm::mat4 TColumna222D = glm::translate(I, glm::vec3(-2.0 + 1.0, 0.7, 2.0 - 0.2));
    glm::mat4 SColumnac2   = glm::scale(I, glm::vec3(0.4, 1.2, 0.2));
    glm::mat4 SColumnaA   = glm::scale(I, glm::vec3(0.2, 0.6, 0.2));
    glm::mat4 SColumnaAc   = glm::scale(I, glm::vec3(0.4, 0.6, 0.2));
    glm::mat4 SColumnaD   = glm::scale(I, glm::vec3(0.6, 0.6, 0.2));

    drawObject(cube, ruby, P, V, M * TColumna111 * SColumnaA);
    drawObject(cube, ruby, P, V, M * TColumna222 * SColumnaA);
    drawObject(cube, ruby, P, V, M * TColumna333 * SColumna);
    drawObject(cube, ruby, P, V, M * TColumna444 * SColumna);
    drawObject(cube, ruby, P, V, M * TColumna333c * SColumnac2);
    drawObject(cube, ruby, P, V, M * TColumna111ac * SColumnaAc);
    drawObject(cube, ruby, P, V, M * TColumna111D * SColumnaD);
    drawObject(cube, ruby, P, V, M * TColumna222D * SColumnaD);

    //puerta
    glm::mat4 SPuerta   = glm::scale(I, glm::vec3(0.4, 0.5, 0.2));
    glm::mat4 TPuerta = glm::translate(I, glm::vec3(2.0 - 2.0, 0.7, 2.0 - 0.2));
    drawObject(cube, obsidian, P, V, M * TPuerta * SPuerta);

    //ventanas
    glm::mat4 SVentanaa   = glm::scale(I, glm::vec3(0.5, 0.5, 0.2));
    glm::mat4 SVentanal   = glm::scale(I, glm::vec3(0.2, 0.5, 0.5));
    glm::mat4 TVentana1a = glm::translate(I, glm::vec3(2.0 - 0.9, 1.9, 2.0 - 0.2));
    glm::mat4 TVentana2a = glm::translate(I, glm::vec3(-2.0 + 0.9, 1.9, 2.0 - 0.2));
    glm::mat4 TVentana3a = glm::translate(I, glm::vec3(2.0 - 0.9, 1.9, -2.0 + 0.2));
    glm::mat4 TVentana4a = glm::translate(I, glm::vec3(-2.0 + 0.9, 1.9, -2.0 + 0.2));
    glm::mat4 TVentana1l = glm::translate(I, glm::vec3(2.0 - 0.2, 1.9, 2.0 - 0.9));
    glm::mat4 TVentana2l = glm::translate(I, glm::vec3(-2.0 + 0.2, 1.9, 2.0 - 0.9));
    glm::mat4 TVentana3l = glm::translate(I, glm::vec3(2.0 - 0.2, 1.9, -2.0+ 0.9));
    glm::mat4 TVentana4l = glm::translate(I, glm::vec3(-2.0 + 0.2, 1.9, -2.0+ 0.9));
    glm::mat4 TVentana11l = glm::translate(I, glm::vec3(2.0 - 0.2, 0.7, 2.0 - 0.9));
    glm::mat4 TVentana22l = glm::translate(I, glm::vec3(-2.0 + 0.2, 0.7, 2.0 - 0.9));
    glm::mat4 TVentana33l = glm::translate(I, glm::vec3(2.0 - 0.2, 0.7, -2.0 + 0.9));
    glm::mat4 TVentana44l = glm::translate(I, glm::vec3(-2.0 + 0.2, 0.7, -2.0 + 0.9));
    glm::mat4 TVentana33a = glm::translate(I, glm::vec3(2.0 - 0.9, 0.7, -2.0 + 0.2));
    glm::mat4 TVentana44a = glm::translate(I, glm::vec3(-2.0 + 0.9, 0.7, -2.0 + 0.2));

    Material ventana = mluz;
    if (dia) {ventana.emissive *= glm::vec4(0.3);}
    Material materialesv[] = {ventana,mluzoff};

    drawObject(cube, materialesv[randomBooleanArray[0]] , P, V, M * TVentana1a * SVentanaa);
    drawObject(cube, materialesv[randomBooleanArray[1]] , P, V, M * TVentana2a * SVentanaa);
    drawObject(cube, materialesv[randomBooleanArray[2]] , P, V, M * TVentana3a * SVentanaa);
    drawObject(cube, materialesv[randomBooleanArray[3]] , P, V, M * TVentana4a * SVentanaa);
    drawObject(cube, materialesv[randomBooleanArray[0]] , P, V, M * TVentana1l * SVentanal);
    drawObject(cube, materialesv[randomBooleanArray[1]] , P, V, M * TVentana2l * SVentanal);
    drawObject(cube, materialesv[randomBooleanArray[2]] , P, V, M * TVentana3l * SVentanal);
    drawObject(cube, materialesv[randomBooleanArray[3]] , P, V, M * TVentana4l * SVentanal);
    drawObject(cube, materialesv[randomBooleanArray[4]] , P, V, M * TVentana11l * SVentanal);
    drawObject(cube, materialesv[randomBooleanArray[5]] , P, V, M * TVentana22l * SVentanal);
    drawObject(cube, materialesv[randomBooleanArray[6]], P, V, M * TVentana33l * SVentanal);
    drawObject(cube, materialesv[randomBooleanArray[7]], P, V, M * TVentana44l * SVentanal);
    drawObject(cube, materialesv[randomBooleanArray[6]], P, V, M * TVentana33a * SVentanaa);
    drawObject(cube, materialesv[randomBooleanArray[7]], P, V, M * TVentana44a * SVentanaa);
}

void drawEdificio(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S   = glm::scale(I, glm::vec3(2.0, 0.1, 2.0));
    glm::mat4 T   = glm::translate(I, glm::vec3(0, 0.1, 0));

    glm::mat4 TP1 = glm::translate(I, glm::vec3(0, 1.3, 0));
    glm::mat4 TP2 = glm::translate(I, glm::vec3(0, 2.6, 0));
    glm::mat4 TP3 = glm::translate(I, glm::vec3(0, 3.9, 0));
    glm::mat4 TP4 = glm::translate(I, glm::vec3(0, 5.2, 0));
    glm::mat4 TP5 = glm::translate(I, glm::vec3(0, 6.5, 0));

    drawObject(cube, ruby, P, V, M * T * S);

    drawObject(cube, ruby, P, V, M * TP1 * S);
    drawObject(cube, ruby, P, V, M * TP2 * S);
    drawObject(cube, ruby, P, V, M * TP3 * S);
    drawObject(cube, ruby, P, V, M * TP4 * S);
    drawObject(cube, ruby, P, V, M * TP5 * S);

    // Dibujar las columnas
    glm::mat4 Scol   = glm::scale(I, glm::vec3(0.2, 3.25, 0.2));

    // Esquinas
    glm::mat4 TColumna1 = glm::translate(I, glm::vec3(1.8, 3.25, 1.8 ));
    glm::mat4 TColumna2 = glm::translate(I, glm::vec3(-1.8, 3.25 , 1.8 ));
    glm::mat4 TColumna3 = glm::translate(I, glm::vec3(1.8, 3.25 , -1.8));
    glm::mat4 TColumna4 = glm::translate(I, glm::vec3(-1.8, 3.25 , -1.8));
    drawObject(cube, ruby, P, V, M * TColumna1 * Scol);
    drawObject(cube, ruby, P, V, M * TColumna2 * Scol);
    drawObject(cube, ruby, P, V, M * TColumna3 * Scol);
    drawObject(cube, ruby, P, V, M * TColumna4 * Scol);

    // Lados
    glm::mat4 TColumna11 = glm::translate(I, glm::vec3(1.8 - 1.0, 3.25 , 1.8));
    glm::mat4 TColumna22 = glm::translate(I, glm::vec3(-1.8, 3.25 , 1.8 - 1.0));
    glm::mat4 TColumna33 = glm::translate(I, glm::vec3(1.8, 3.25 , -1.8 + 1.0));
    glm::mat4 TColumna44 = glm::translate(I, glm::vec3(-1.8 + 1.0, 3.25 , -1.8));
    drawObject(cube, ruby, P, V, M * TColumna11 * Scol);
    drawObject(cube, ruby, P, V, M * TColumna22 * Scol);
    drawObject(cube, ruby, P, V, M * TColumna33 * Scol);
    drawObject(cube, ruby, P, V, M * TColumna44 * Scol);


    glm::mat4 TColumna111 = glm::translate(I, glm::vec3(1.8, 3.25 , 1.8 - 1.0));
    glm::mat4 TColumna222 = glm::translate(I, glm::vec3(-1.8 + 1.0, 3.25 , 1.8));
    glm::mat4 TColumna333 = glm::translate(I, glm::vec3(1.8 - 1.0, 3.25 , -1.8));
    glm::mat4 TColumna444 = glm::translate(I, glm::vec3(-1.8, 3.25 , -1.8 + 1.0));
    drawObject(cube, ruby, P, V, M * TColumna111 * Scol);
    drawObject(cube, ruby, P, V, M * TColumna222 * Scol);
    drawObject(cube, ruby, P, V, M * TColumna333 * Scol);
    drawObject(cube, ruby, P, V, M * TColumna444 * Scol);

    //Dibujar puerta y toldo
    glm::mat4 SPuerta1 = glm::scale(I, glm::vec3(0.3, 0.6 , 0.1));
    glm::mat4 TPuerta1 = glm::translate(I, glm::vec3(-0.3, 0.6, -1.95));
    glm::mat4 SPuerta2 = glm::scale(I, glm::vec3(0.3, 0.6 , 0.1));
    glm::mat4 TPuerta2 = glm::translate(I, glm::vec3(0.3, 0.6 , -1.95));
    drawObject(cube, obsidian, P, V, M * TPuerta1 * SPuerta1);
    drawObject(cube, pSilver, P, V, M * TPuerta2 * SPuerta2);
    glm::mat4 SToldo = glm::scale(I, glm::vec3(1.0, 0.05 , 1.0));
    glm::mat4 TToldo = glm::translate(I, glm::vec3(0, 1.3, -2.0));
    drawObject(cube, obsidian, P, V, M * TToldo * SToldo);
    glm::mat4 SMastil1 = glm::scale(I, glm::vec3(0.05, 0.65 , 0.05));
    glm::mat4 TMastil1 = glm::translate(I, glm::vec3(0.8, 0.65, -2.7));
    glm::mat4 SMastil2 = glm::scale(I, glm::vec3(0.05, 0.65 , 0.05));
    glm::mat4 TMastil2 = glm::translate(I, glm::vec3(-0.8, 0.65, -2.7));
    drawObject(cylinder, obsidian, P, V, M * TMastil1 * SMastil1);
    drawObject(cylinder, obsidian, P, V, M * TMastil2 * SMastil2);

    //tapar ventanas abajo
    glm::mat4 SVcT = glm::scale(I, glm::vec3(0.6, 0.6 , 0.1));
    glm::mat4 TVcT = glm::translate(I, glm::vec3(0, 0.6, 1.9));
    drawObject(cube, ruby, P, V, M * TVcT * SVcT);
    glm::mat4 SVcD = glm::scale(I, glm::vec3(0.1, 0.6 , 0.6));
    glm::mat4 TVcD = glm::translate(I, glm::vec3(1.9, 0.6, 0));
    drawObject(cube, ruby, P, V, M * TVcD * SVcD);
    glm::mat4 SVcI = glm::scale(I, glm::vec3(0.1, 0.6 , 0.6));
    glm::mat4 TVcI = glm::translate(I, glm::vec3(-1.9, 0.6, 0));
    drawObject(cube, ruby, P, V, M * TVcI * SVcI);


    //balcones
    glm::mat4 Sbbase1 = glm::scale(I, glm::vec3(0.4, 0.05 , 0.8));
    glm::mat4 Tbbase1 = glm::translate(I, glm::vec3(2.4, 1.4, 0));
    drawObject(cube, obsidian, P, V, M * Tbbase1 * Sbbase1);
    glm::mat4 Sblateral1 = glm::scale(I, glm::vec3(0.4, 0.3 , 0.05));
    glm::mat4 Tblateral1 = glm::translate(I, glm::vec3(2.4, 1.7, 0.75));
    drawObject(cube, obsidian, P, V, M * Tblateral1 * Sblateral1);
    glm::mat4 Sblateral11 = glm::scale(I, glm::vec3(0.4, 0.3 , 0.05));
    glm::mat4 Tblateral11 = glm::translate(I, glm::vec3(2.4, 1.7, -0.75));
    drawObject(cube, obsidian, P, V, M * Tblateral11 * Sblateral11);
    glm::mat4 Sblateral111 = glm::scale(I, glm::vec3(0.05, 0.3 ,0.8));
    glm::mat4 Tblateral111 = glm::translate(I, glm::vec3(2.75, 1.7, 0));
    drawObject(cube, obsidian, P, V, M * Tblateral111 * Sblateral111);

    glm::mat4 Sbbase2 = glm::scale(I, glm::vec3(0.4, 0.05 , 0.8));
    glm::mat4 Tbbase2 = glm::translate(I, glm::vec3(2.4, 2.7, 0));
    drawObject(cube, obsidian, P, V, M * Tbbase2 * Sbbase2);
    glm::mat4 Sblateral2 = glm::scale(I, glm::vec3(0.4, 0.3 , 0.05));
    glm::mat4 Tblateral2 = glm::translate(I, glm::vec3(2.4, 3.0, 0.75));
    drawObject(cube, obsidian, P, V, M * Tblateral2 * Sblateral2);
    glm::mat4 Sblateral22 = glm::scale(I, glm::vec3(0.4, 0.3 , 0.05));
    glm::mat4 Tblateral22 = glm::translate(I, glm::vec3(2.4, 3.0, -0.75));
    drawObject(cube, obsidian, P, V, M * Tblateral22 * Sblateral22);
    glm::mat4 Sblateral222 = glm::scale(I, glm::vec3(0.05, 0.3 ,0.8));
    glm::mat4 Tblateral222 = glm::translate(I, glm::vec3(2.75, 3.0, 0));
    drawObject(cube, obsidian, P, V, M * Tblateral222 * Sblateral222);

    glm::mat4 Sbbase3 = glm::scale(I, glm::vec3(0.4, 0.05 , 0.8));
    glm::mat4 Tbbase3 = glm::translate(I, glm::vec3(2.4, 4.0, 0));
    drawObject(cube, obsidian, P, V, M * Tbbase3 * Sbbase3);
    glm::mat4 Sblateral3 = glm::scale(I, glm::vec3(0.4, 0.3 , 0.05));
    glm::mat4 Tblateral3 = glm::translate(I, glm::vec3(2.4, 4.3, 0.75));
    drawObject(cube, obsidian, P, V, M * Tblateral3 * Sblateral3);
    glm::mat4 Sblateral33 = glm::scale(I, glm::vec3(0.4, 0.3 , 0.05));
    glm::mat4 Tblateral33 = glm::translate(I, glm::vec3(2.4, 4.3, -0.75));
    drawObject(cube, obsidian, P, V, M * Tblateral33 * Sblateral33);
    glm::mat4 Sblateral333 = glm::scale(I, glm::vec3(0.05, 0.3 ,0.8));
    glm::mat4 Tblateral333 = glm::translate(I, glm::vec3(2.75, 4.3, 0));
    drawObject(cube, obsidian, P, V, M * Tblateral333 * Sblateral333);

    glm::mat4 Sbbase4 = glm::scale(I, glm::vec3(0.4, 0.05 , 0.8));
    glm::mat4 Tbbase4 = glm::translate(I, glm::vec3(2.4, 5.3, 0));
    drawObject(cube, obsidian, P, V, M * Tbbase4 * Sbbase4);
    glm::mat4 Sblateral4 = glm::scale(I, glm::vec3(0.4, 0.3 , 0.05));
    glm::mat4 Tblateral4 = glm::translate(I, glm::vec3(2.4, 5.6, 0.75));
    drawObject(cube, obsidian, P, V, M * Tblateral4 * Sblateral4);
    glm::mat4 Sblateral44 = glm::scale(I, glm::vec3(0.4, 0.3 , 0.05));
    glm::mat4 Tblateral44 = glm::translate(I, glm::vec3(2.4, 5.6, -0.75));
    drawObject(cube, obsidian, P, V, M * Tblateral44 * Sblateral44);
    glm::mat4 Sblateral444 = glm::scale(I, glm::vec3(0.05, 0.3 ,0.8));
    glm::mat4 Tblateral444 = glm::translate(I, glm::vec3(2.75, 5.6, 0));
    drawObject(cube, obsidian, P, V, M * Tblateral444 * Sblateral444);
 //Dibjar ventanas
    glm::mat4 SInterior = glm::scale(I, glm::vec3(1.9, 3.3 , 1.9));
    glm::mat4 TInterior = glm::translate(I, glm::vec3(0, 3.25, 0));
    Material ventana = mluz;
    if (dia) {
        ventana.emissive = glm::vec4(0.3);
        drawObject(cube, ventana, P, V, M * TInterior * SInterior);
    } else {
        drawObject(cube, ventana, P, V, M * TInterior * SInterior);
    }

}
void drawFarola(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 Spie   = glm::scale(I, glm::vec3(0.07, 0.1, 0.07));
    glm::mat4 Tpie = glm::translate(I, glm::vec3(0, 0.1, 0));
    drawObject(cylinder,obsidian,P,V,M*Tpie*Spie);
    glm::mat4 Sbase   = glm::scale(I, glm::vec3(0.05, 0.6, 0.05));
    glm::mat4 Tbase  = glm::translate(I, glm::vec3(0, 0.6, 0));
    drawObject(cylinder,obsidian,P,V,M*Tbase*Sbase);
    glm::mat4 Stronco   = glm::scale(I, glm::vec3(0.025, 1.2, 0.025));
    glm::mat4 Ttronco  = glm::translate(I, glm::vec3(0, 1.2, 0));
    drawObject(cylinder,obsidian,P,V,M*Ttronco*Stronco);
    glm::mat4 SbaseF   = glm::scale(I, glm::vec3(0.05, 0.01, 0.05));
    glm::mat4 TbaseF  = glm::translate(I, glm::vec3(0, 2.41, 0));
    drawObject(cube,obsidian,P,V,M*TbaseF*SbaseF);
    glm::mat4 SbaseF2   = glm::scale(I, glm::vec3(0.07, 0.01, 0.07));
    glm::mat4 TbaseF2  = glm::translate(I, glm::vec3(0, 2.51, 0));
    drawObject(cube,obsidian,P,V,M*TbaseF2*SbaseF);
    glm::mat4 Sf  = glm::scale(I, glm::vec3(0.001, 0.05, 0.001));
    glm::mat4 Tf1  = glm::translate(I, glm::vec3(0.0495, 2.46, 0.0495));
    glm::mat4 Tf2  = glm::translate(I, glm::vec3(-0.0495, 2.46, 0.0495));
    glm::mat4 Tf3  = glm::translate(I, glm::vec3(0.0495, 2.46, -0.0495));
    glm::mat4 Tf4  = glm::translate(I, glm::vec3(-0.0495, 2.46, -0.0495));
    drawObject(cylinder,obsidian,P,V,M*Tf1*Sf);
    drawObject(cylinder,obsidian,P,V,M*Tf2*Sf);
    drawObject(cylinder,obsidian,P,V,M*Tf3*Sf);
    drawObject(cylinder,obsidian,P,V,M*Tf4*Sf);

    glm::mat4 Sfarol  = glm::scale(I, glm::vec3(0.049, 0.05, 0.049));
    glm::mat4 Tfarol  = glm::translate(I, glm::vec3(0.0, 2.46, 0.0));

    Material farola = mluz;
    if (dia) {farola.emissive *= 0.3;}
    drawObject(cube,farola,P,V,M*Tfarol*Sfarol);


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
    if (dia) {
        mFaro.emissive = glm::vec4(lightF[index].diffuse.x * 0.6, lightF[index].diffuse.y * 0.6, lightF[index].diffuse.z * 0.6, 1.0);
    } else {
        mFaro.emissive = glm::vec4(lightF[index].diffuse, 1.0);
    }

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
    glm::mat4 T = translate(I, glm::vec3(0,0.4,0));
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

    Material luzPos = mluzR;
    if (dia) {
        luzPos.emissive = glm::vec4(luzPos.emissive.x * 0.6, luzPos.emissive.y * 0.6, luzPos.emissive.z * 0.6, 1.0);
    }

    drawObject(cube, luzPos, P, V, M * Tf1 * Sf);
    drawObject(cube, luzPos, P, V, M * Tf2 * Sf);

}

void drawLucesPosicionDeportivo (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 Sf = scale(I, glm::vec3(0.1/2.0,0.08/2.0,0.01/2.0));

    //luces traseras deportivo
    glm::mat4 Tf1 =  translate(I, glm::vec3(0.3,0.18,0.73));
    glm::mat4 Tf2 =  translate(I, glm::vec3(-0.3,0.18,0.73));

    Material luzP = mluzR;
    if (dia) {
        luzP.emissive = glm::vec4(luzP.emissive.x * 0.6, luzP.emissive.y * 0.6, luzP.emissive.z * 0.6, 1.0);
    }

    drawObject(cube, luzP, P, V, M * Tf1 * Sf);
    drawObject(cube, luzP, P, V, M * Tf2 * Sf);

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
        case 'n': rotAstro = 180.0;
                  dia = false;
                  break;
        case 'm': rotAstro = 0.0;
                  dia = true;
                  break;
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

    npcTimer += 1.0;
    if (npcTimer > 7.0) {   // Toca girar
        npcTimer = 0.0;
        npcRot += 90.0;
        npcDir += 1;
        if (npcDir > 3) {   // Se ha hecho un giro completo
            npcDir = 0;
            // Reiniciar npcControl
            for (int i = 0; i < 4; i++) {
                npcControl[i] = 0.0;
            }
        }
    }
    npcControl[npcDir] = npcTimer;

    if (npcTimer == 0){
        for (int i = 0; i < NVENTANAS; i++) {
            randomBooleanArray[i] = rand() % 2;
        }
    }

    rotAstro += 5.0;
    if (rotAstro > 360.0) rotAstro = 0.0;

    if (rotAstro < 180.0) {
        dia = true;
    } else {
        dia = false;
    }

    glutPostRedisplay();
    glutTimerFunc(speed, funTimer, 0);

}
