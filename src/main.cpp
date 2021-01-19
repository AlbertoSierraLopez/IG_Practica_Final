#include <iostream>
#include <ogl/glew.h>
#include <ogl/freeglut.h>

#include "Shaders.h"
#include "Model.h"
#include "Texture.h"

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

void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);

// Shaders
Shaders shaders;

// Modelos
Model sphere;
Model plane;
Model cylinder;
Model cube;

// Texturas (imagenes)
Texture   geometric;
Texture   gravel;
Texture   marble;
Texture   none;
Texture   speckles;
Texture   tile;
Texture   tileNormal;
Texture   brick;
Texture   brickNormal;
Texture   wheel;
Texture   asphalt;
Texture   brick2;
Texture   brick2Normal;
Texture   moon;
Texture   moonNormal;
Texture   neon;
Texture   sign;
Texture   sun;
Texture   sunEmissive;
Texture   grass;
Texture   grassNormal;
Texture   road;
Texture   iron;
Texture   ironNormal;
Texture   ironSpecular;
Texture   door;
Texture   doorNormal;
Texture   doorSpecular;
Texture   white;
Texture   whiteNormal;
Texture   concrete;
Texture   concreteNormal;
Texture   stone;
Texture   stoneNormal;
Texture   wood;
Texture   woodNormal;
Texture   blueMetal;
Texture   decoratedIron;
Texture   tire;
Texture   tireNormal;

// Luces y materiales
#define   NCOCHES  2
#define   NFAROLAS 13

Light     lightG;
Light     lightP[1+NFAROLAS];
Light     lightF[NCOCHES*2];
Light     lightSol;
Light     lightLuna;
Light     lightFarola;

Material  mluz;
Material  mluzoff;
Material  mluzRed;
Material  mSol;
Material  mLuna;
Material  gold;
Material  ruby;
Material  cyan;
Material  polishedBronze;
Material  emmerald;
Material  xgreen;
Material  obsidian;
Material  wPlastic;
Material  pSilver;
Material  mCristal;

Textures  texWheel;
Textures  texBrick;
Textures  texBrick2;
Textures  texAsphalt;
Textures  texRoad;
Textures  texGrass;
Textures  texIron;
Textures  texMoon;
Textures  texNeon;
Textures  texSign;
Textures  texSun;
Textures  texDoor;
Textures  texWhite;
Textures  texConcrete;
Textures  texStone;
Textures  texWood;
Textures  texBlueMetal;
Textures  texDecoratedIron;
Textures  texTire;

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
#define     NCOCHESTIPO 3
std::string coches[NCOCHESTIPO] = {"Utilitario", "Todoterreno", "Deportivo"};
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

    // Establecer el offset para quitar el z-fighting
    glPolygonOffset(-1.0,1.0);

    // Para mezclar colores con alpha y 1-alpha
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

 // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

 // Modelos
    sphere.initModel("resources/models/sphere.obj");
    plane.initModel("resources/models/plane.obj");
    cylinder.initModel("resources/models/cylinder.obj");
    cube.initModel("resources/models/cube.obj");

 // Texturas
    geometric.initTexture("resources/textures/geometric.png");
    gravel.initTexture("resources/textures/gravel.png");
    marble.initTexture("resources/textures/marble.png");
    none.initTexture("resources/textures/none.png");
    speckles.initTexture("resources/textures/speckles.png");
    tile.initTexture("resources/textures/tile.png");
    tileNormal.initTexture("resources/textures/tileNormal.png");
    brick.initTexture("resources/textures/brick.jpg");
    brickNormal.initTexture("resources/textures/brickNormal.jpg");
    wheel.initTexture("resources/textures/wheel.jpg");
    asphalt.initTexture("resources/textures/asphalt.jpg");
    brick2.initTexture("resources/textures/brick2.tif");
    brick2Normal.initTexture("resources/textures/brick2Normal.tif");
    grass.initTexture("resources/textures/grass.tif");
    grassNormal.initTexture("resources/textures/grassNormal.tif");
    iron.initTexture("resources/textures/iron.tif");
    ironNormal.initTexture("resources/textures/ironNormal.tif");
    ironSpecular.initTexture("resources/textures/ironSpecular.tif");
    moon.initTexture("resources/textures/moon.tif");
    moonNormal.initTexture("resources/textures/moonNormal.tif");
    neon.initTexture("resources/textures/neon.jpg");
    road.initTexture("resources/textures/road.tif");
    sign.initTexture("resources/textures/sign.png");
    sun.initTexture("resources/textures/sun.tif");
    sunEmissive.initTexture("resources/textures/sunEmissive.tif");
    door.initTexture("resources/textures/door.tif");
    doorNormal.initTexture("resources/textures/doorNormal.tif");
    doorSpecular.initTexture("resources/textures/doorSpecular.tif");
    white.initTexture("resources/textures/white.jpg");
    whiteNormal.initTexture("resources/textures/whiteNormal.tif");
    concrete.initTexture("resources/textures/concrete.tif");
    concreteNormal.initTexture("resources/textures/concreteNormal.tif");
    stone.initTexture("resources/textures/stone.tif");
    stoneNormal.initTexture("resources/textures/stoneNormal.tif");
    wood.initTexture("resources/textures/wood.tif");
    woodNormal.initTexture("resources/textures/woodNormal.tif");
    blueMetal.initTexture("resources/textures/blueMetal.tif");
    decoratedIron.initTexture("resources/textures/decoratedIron.tif");
    tire.initTexture("resources/textures/tire.tif");
    tireNormal.initTexture("resources/textures/tireNormal.tif");


    // Luces Globales
    lightG.ambient        = glm::vec3(0.9, 0.9, 0.9);

 // Luces Posicionales
    // Sol
    lightSol.position  = glm::vec3(16.0,0.0, 0.0);
    lightSol.ambient   = glm::vec3(2.53,2.04,1.08);
    lightSol.diffuse   = glm::vec3(2.53,2.04,1.08);
    lightSol.specular  = glm::vec3(0.7,0.7,  0.7);
    lightSol.c0 = 0.4;
    lightSol.c1 = 0.035;
    lightSol.c2 = 0.005;
    // Luna
    lightLuna.position  = glm::vec3(-16.0,0.0, 0.0);
    lightLuna.ambient   = glm::vec3(1.94, 1.97,2.04);
    lightLuna.diffuse   = glm::vec3(1.94, 1.97,2.04);
    lightLuna.specular  = glm::vec3(0.5,  0.5, 0.5);
    lightLuna.c0 = 1.0;
    lightLuna.c1 = 0.18;
    lightLuna.c2 = 0.08;

    // Farola
    lightFarola.position  = glm::vec3(0.0, 2.46, 0.0);
    lightFarola.ambient   = glm::vec3(1.23, 1.025, 0.695);
    lightFarola.diffuse   = glm::vec3(1.23, 1.025, 0.695);
    lightFarola.specular  = glm::vec3(0.7, 0.7, 0.7);
    lightFarola.c0 = 2.0;
    lightFarola.c1 = 1.0;
    lightFarola.c2 = 0.8;

    lightP[1] = lightFarola;
        lightP[1].position = glm::vec3(-1.0, 2.46, -1.0);
    lightP[2] = lightFarola;
        lightP[2].position = glm::vec3(-1.0, 2.46, -6.0);
    lightP[3] = lightFarola;
        lightP[3].position = glm::vec3(-6.0, 2.46, -1.0);
    lightP[4] = lightFarola;
        lightP[4].position = glm::vec3(1.0, 2.46, 1.0);
    lightP[5] = lightFarola;
        lightP[5].position = glm::vec3(1.0, 2.46, 6.0);
    lightP[6] = lightFarola;
        lightP[6].position = glm::vec3(6.0, 2.46, 1.0);
    lightP[7] = lightFarola;
        lightP[7].position = glm::vec3(1.0, 2.46, -1.0);
    lightP[8] = lightFarola;
        lightP[8].position = glm::vec3(1.0, 2.46, -6.0);
    lightP[9] = lightFarola;
        lightP[9].position = glm::vec3(6.0, 2.46, -1.0);
    lightP[10] = lightFarola;
        lightP[10].position = glm::vec3(3.5, 2.46, -3.5);
    lightP[11] = lightFarola;
        lightP[11].position = glm::vec3(-1.0, 2.46, 1.0);
    lightP[12] = lightFarola;
        lightP[12].position = glm::vec3(-1.0, 2.46, 6.0);
    lightP[13] = lightFarola;
        lightP[13].position = glm::vec3(-6.0, 2.46, 1.0);

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

    mluzoff.ambient   = glm::vec4(0.0, 0.0, 0.0, 0.3);
    mluzoff.diffuse   = glm::vec4(0.0, 0.0, 0.0, 0.3);
    mluzoff. specular  = glm::vec4(0.0, 0.0, 0.0, 0.3);
    mluzoff. emissive  = glm::vec4(0.3, 0.3, 0.3, 1.0);
    mluzoff. shininess = 1.0;

    mluzRed.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluzRed.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluzRed.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluzRed.emissive  = glm::vec4(1.0, 0.0, 0.0, 1.0);
    mluzRed.shininess = 1.0;

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

    ruby.ambient   = glm::vec4(0.174500, 0.011750, 0.011750, 0.55);
    ruby.diffuse   = glm::vec4(0.614240, 0.041360, 0.041360, 0.55);
    ruby.specular  = glm::vec4(0.727811, 0.626959, 0.626959, 0.55);
    ruby.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.0);
    ruby.shininess = 76.8;

    gold.ambient   = glm::vec4(0.247250, 0.199500, 0.074500, 1.00);
    gold.diffuse   = glm::vec4(0.751640, 0.606480, 0.226480, 1.00);
    gold.specular  = glm::vec4(0.628281, 0.555802, 0.366065, 1.00);
    gold.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    gold.shininess = 51.2;

    cyan.ambient   = glm::vec4(0.0f,0.05f,0.05f,1.0f);
    cyan.diffuse   = glm::vec4(0.4f,0.5f,0.5f,1.0f );
    cyan.specular  = glm::vec4(0.04f,0.7f,0.7f,1.0f );
    cyan.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    cyan.shininess = 10.0f;

    polishedBronze.ambient   = glm::vec4(0.25f, 0.148f, 0.06475f, 1.0f );
    polishedBronze.diffuse   = glm::vec4(0.4f, 0.2368f, 0.1036f, 1.0f);
    polishedBronze.specular  = glm::vec4(0.774597f, 0.458561f, 0.200621f, 1.0f );
    polishedBronze.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    polishedBronze.shininess =  76.8f;

    emmerald.ambient   = glm::vec4( 0.0215f, 0.1745f, 0.0215f, 0.55f );
    emmerald.diffuse   = glm::vec4(0.07568f, 0.61424f, 0.07568f, 0.55f);
    emmerald.specular  = glm::vec4(0.633f, 0.727811f, 0.633f, 0.55f );
    emmerald.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.00);
    emmerald.shininess =  76.8f;

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

    // Cristal
    mCristal.ambient   = glm::vec4(0.2f, 0.2f, 0.3f, 0.3f);
    mCristal.diffuse   = glm::vec4(0.2f, 0.2f, 0.3f, 0.3f);
    mCristal.specular  = glm::vec4(0.7f, 0.7f, 0.8f, 0.3f);
    mCristal.emissive  = glm::vec4(0.000000, 0.000000, 0.000000, 1.0f);
    mCristal.shininess = 89.6f ;

    // Inicializar array de booleanos aleatorios para las ventanas
    for (int i = 0; i < NVENTANAS; i++) {
        randomBooleanArray[i] = rand() % 2;
    }

 // Texturas
    texWheel.diffuse    = wheel.getTexture();
    texWheel.specular   = none.getTexture();
    texWheel.emissive   = none.getTexture();
    texWheel.normal     = 0;
    texWheel.shininess  = 10.0;

    texBrick.diffuse    = brick.getTexture();
    texBrick.specular   = none.getTexture();
    texBrick.emissive   = none.getTexture();
    texBrick.normal     = brickNormal.getTexture();
    texBrick.shininess  = 10.0;

    texBrick2.diffuse    = brick2.getTexture();
    texBrick2.specular   = none.getTexture();
    texBrick2.emissive   = none.getTexture();
    texBrick2.normal     = brick2Normal.getTexture();
    texBrick2.shininess  = 10.0;

    texAsphalt.diffuse  = asphalt.getTexture();
    texAsphalt.specular = none.getTexture();
    texAsphalt.emissive = none.getTexture();
    texAsphalt.normal   = 0;
    texAsphalt.shininess= 10.0;

    texRoad.diffuse  = road.getTexture();
    texRoad.specular = none.getTexture();
    texRoad.emissive = none.getTexture();
    texRoad.normal   = 0;
    texRoad.shininess= 10.0;

    texGrass.diffuse  = grass.getTexture();
    texGrass.specular = none.getTexture();
    texGrass.emissive = none.getTexture();
    texGrass.normal   = grassNormal.getTexture();
    texGrass.shininess= 10.0;

    texIron.diffuse  = iron.getTexture();
    texIron.specular = none.getTexture();
    texIron.emissive = none.getTexture();
    texIron.normal   = iron.getTexture();
    texIron.shininess= 10.0;

    texMoon.diffuse  = moon.getTexture();
    texMoon.specular = none.getTexture();
    texMoon.emissive = none.getTexture();
    texMoon.normal   = moonNormal.getTexture();
    texMoon.shininess= 10.0;

    texNeon.diffuse  = neon.getTexture();
    texNeon.specular = none.getTexture();
    texNeon.emissive = neon.getTexture();
    texNeon.normal   = 0;
    texNeon.shininess= 10.0;

    texSign.diffuse  = sign.getTexture();
    texSign.specular = none.getTexture();
    texSign.emissive = none.getTexture();
    texSign.normal   = 0;
    texSign.shininess= 10.0;

    texSun.diffuse  = sun.getTexture();
    texSun.specular = none.getTexture();
    texSun.emissive = sunEmissive.getTexture();
    texSun.normal   = 0;
    texSun.shininess= 10.0;

    texDoor.diffuse  = door.getTexture();
    texDoor.specular = doorSpecular.getTexture();
    texDoor.emissive = none.getTexture();
    texDoor.normal   = doorNormal.getTexture();
    texDoor.shininess= 10.0;

    texWhite.diffuse  = white.getTexture();
    texWhite.specular = none.getTexture();
    texWhite.emissive = none.getTexture();
    texWhite.normal   = whiteNormal.getTexture();
    texWhite.shininess= 10.0;

    texConcrete.diffuse  = concrete.getTexture();
    texConcrete.specular = none.getTexture();
    texConcrete.emissive = none.getTexture();
    texConcrete.normal   = concreteNormal.getTexture();
    texConcrete.shininess= 10.0;

    texStone.diffuse  = stone.getTexture();
    texStone.specular = none.getTexture();
    texStone.emissive = none.getTexture();
    texStone.normal   = stoneNormal.getTexture();
    texStone.shininess= 10.0;

    texWood.diffuse  = wood.getTexture();
    texWood.specular = none.getTexture();
    texWood.emissive = none.getTexture();
    texWood.normal   = wood.getTexture();
    texWood.shininess= 10.0;

    texBlueMetal.diffuse  = blueMetal.getTexture();
    texBlueMetal.specular = none.getTexture();
    texBlueMetal.emissive = none.getTexture();
    texBlueMetal.normal   = 0;
    texBlueMetal.shininess= 10.0;

    texDecoratedIron.diffuse  = decoratedIron.getTexture();
    texDecoratedIron.specular = none.getTexture();
    texDecoratedIron.emissive = none.getTexture();
    texDecoratedIron.normal   = 0;
    texDecoratedIron.shininess= 10.0;

    texTire.diffuse  = tire.getTexture();
    texTire.specular = none.getTexture();
    texTire.emissive = none.getTexture();
    texTire.normal   = tireNormal.getTexture();
    texTire.shininess= 10.0;

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

    // Dibujar Edificios
    glm::mat4 TEdificio   = glm::translate(I, glm::vec3(3.5, 0.0, 3.8));
    drawEdificio(P,V,I * TEdificio);
    glm::mat4 TOficina = glm::translate(I,glm::vec3(-3.5,0,-3.8));
    drawOficina(P,V,I * TOficina);
    glm::mat4 TParque = glm::translate(I,glm::vec3(3.5,0,-3.5));
    drawParque(P,V,I * TParque);

    glm::mat4 TRestaurante = glm::translate(I,glm::vec3(-3.5,0,3.8));
    drawRestaurante(P,V,I * TRestaurante);

    // Intercambiamos los buffers
    glutSwapBuffers();

}

void setLights(glm::mat4 P, glm::mat4 V) {

 // Luz Globales
    shaders.setLight("ulightG",lightG);

 // Luces Posicionales
    for (int i = 0; i < 1; i++) {   // Primero los astros
        // Luz
        Light lAstro;
        if (dia) {
            lAstro = lightSol;
        } else {
            lAstro = lightLuna;
        }
        // Posicion
        glm::mat4 rotLuz = glm::rotate(I, glm::radians(rotAstro), glm::vec3(0, 0, 1));
        glm::vec4 posLuz = glm::vec4(lAstro.position, 1.0);
        lAstro.position = rotLuz * posLuz;

        shaders.setLight("ulightP["+toString(i)+"]", lAstro);
    }
    for (int i = 1; i < NFAROLAS+1; i++) {  // Ahora las farolas
        // Luz
        Light lFarola = lightP[i];
        if (dia) {
            lFarola.ambient = glm::vec3(0.0);
            lFarola.diffuse = glm::vec3(0.0);
            lFarola.specular = glm::vec3(0.0);
        }

        shaders.setLight("ulightP["+toString(i)+"]", lFarola);
    }

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
    Textures tAstro;
    float scale = 0.0;

    if (dia) {
        lAstro = lightSol;
        tAstro = texSun;
        scale = 1.0;
    } else {
        lAstro = lightLuna;
        tAstro = texMoon;
        scale = 0.25;
    }

    glm::mat4 S = glm::scale(I, glm::vec3(scale));
    glm::mat4 T = glm::translate(I, lAstro.position);
    glm::mat4 R = glm::rotate(I, glm::radians(rotAstro), glm::vec3(0, 0, 1));
    drawObjectTex(sphere, tAstro, P, V, M*R*T*S);
}

void drawCarretera(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(8.0, 0.025, 1.0));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 0.025, 0.0));
    glm::mat4 R = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 1, 0));

    glm::mat4 Tizquierda = glm::translate(I, glm::vec3(7.0, 0.0, 0.0));
    glm::mat4 Tderecha    = glm::translate(I, glm::vec3(-7.0, 0.0, 0.0));
    glm::mat4 Tarriba     = glm::translate(I, glm::vec3(0.0, 0.0, 7.0));
    glm::mat4 Tabajo      = glm::translate(I, glm::vec3(0.0, 0.0, -7.0));

    drawObjectTex(cube, texRoad, P, V, M * T * S);
    drawObjectTex(cube, texRoad, P, V, M * Tarriba * T * S);
    drawObjectTex(cube, texRoad, P, V, M * Tabajo * T * S);

    drawObjectTex(cube, texRoad, P, V, M * R * T * S);
    drawObjectTex(cube, texRoad, P, V, M * Tizquierda * R * T * S);
    drawObjectTex(cube, texRoad, P, V, M * Tderecha * R * T * S);

}

void drawParque(glm::mat4 P, glm::mat4 V, glm::mat4 M){

    glm::mat4 S = glm::scale(I, glm::vec3(2.5, 0.025, 2.5));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 0.025, 0.0));
    drawObjectTex(cube,texGrass,P,V,M*T*S);

    // Parada de bus
    glm::mat4 RTecho = glm::rotate(I, glm::radians(180.0f), glm::vec3(1, 0, 0));
    glm::mat4 STecho = glm::scale(I, glm::vec3(1.0, 0.05 , 0.6));
    glm::mat4 TTecho = glm::translate(I, glm::vec3(0, 1.3, -2.0));
    drawObjectTex(cube, texBlueMetal, P, V, M * TTecho * STecho * RTecho);

    glm::mat4 SMastil1 = glm::scale(I, glm::vec3(0.05, 0.65 , 0.05));
    glm::mat4 TMastil1 = glm::translate(I, glm::vec3(0.8, 0.65, -2.0));
    glm::mat4 SMastil2 = glm::scale(I, glm::vec3(0.05, 0.65 , 0.05));
    glm::mat4 TMastil2 = glm::translate(I, glm::vec3(-0.8, 0.65, -2.0));
    drawObjectTex(cylinder, texBlueMetal, P, V, M * TMastil1 * SMastil1 * RTecho);
    drawObjectTex(cylinder, texBlueMetal, P, V, M * TMastil2 * SMastil2 * RTecho);

    glm::mat4 SBanco = glm::scale(I, glm::vec3(0.8, 0.03 , 0.2));
    glm::mat4 TBanco = glm::translate(I, glm::vec3(0, 0.43, -2));
    drawObjectMat(cube, wPlastic, P, V, M * TBanco * SBanco * RTecho);

    // Papelera
    glm::mat4 SPapelera = glm::scale(I, glm::vec3(0.1, 0.3 , 0.1));
    glm::mat4 TPapelera = glm::translate(I, glm::vec3(1, 0.4, -2));
    drawObjectTex(cube, texDecoratedIron, P, V, M * TPapelera * SPapelera * RTecho);
    glm::mat4 SPapelerab = glm::scale(I, glm::vec3(0.07, 0.1 , 0.07));
    glm::mat4 TPapelerab = glm::translate(I, glm::vec3(1, 0.1, -2));
    drawObjectTex(cube, texIron, P, V, M * TPapelerab * SPapelerab * RTecho);

    // Cristales
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    glEnable(GL_CULL_FACE);
        glm::mat4 SCristalt = glm::scale(I, glm::vec3(0.8, 0.65 , 0.05));
        glm::mat4 TCristalt = glm::translate(I, glm::vec3(0.0, 0.65, -1.55));
        glm::mat4 SCristali = glm::scale(I, glm::vec3(0.05, 0.65 , 0.25));
        glm::mat4 TCristali = glm::translate(I, glm::vec3(-0.8, 0.65, -1.75));
        glm::mat4 SCristald = glm::scale(I, glm::vec3(0.05, 0.65 , 0.25));
        glm::mat4 TCristald = glm::translate(I, glm::vec3(0.8, 0.65, -1.75));
        drawObjectMat(cube, mCristal, P, V, M * TCristalt * SCristalt * RTecho);
        drawObjectMat(cube, mCristal, P, V, M * TCristali * SCristali * RTecho);
        drawObjectMat(cube, mCristal, P, V, M * TCristald * SCristald * RTecho);
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

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

void drawFarola(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 Spie   = glm::scale(I, glm::vec3(0.07, 0.1, 0.07));
    glm::mat4 Tpie = glm::translate(I, glm::vec3(0, 0.1, 0));
    drawObjectTex(cylinder,texIron,P,V,M*Tpie*Spie);
    glm::mat4 Sbase   = glm::scale(I, glm::vec3(0.05, 0.6, 0.05));
    glm::mat4 Tbase  = glm::translate(I, glm::vec3(0, 0.6, 0));
    drawObjectTex(cylinder,texIron,P,V,M*Tbase*Sbase);
    glm::mat4 Stronco   = glm::scale(I, glm::vec3(0.025, 1.2, 0.025));
    glm::mat4 Ttronco  = glm::translate(I, glm::vec3(0, 1.2, 0));
    drawObjectTex(cylinder,texIron,P,V,M*Ttronco*Stronco);
    glm::mat4 SbaseF   = glm::scale(I, glm::vec3(0.05, 0.01, 0.05));
    glm::mat4 TbaseF  = glm::translate(I, glm::vec3(0, 2.41, 0));
    drawObjectTex(cube,texIron,P,V,M*TbaseF*SbaseF);
    glm::mat4 SbaseF2   = glm::scale(I, glm::vec3(0.07, 0.01, 0.07));
    glm::mat4 TbaseF2  = glm::translate(I, glm::vec3(0, 2.51, 0));
    drawObjectTex(cube,texIron,P,V,M*TbaseF2*SbaseF);
    glm::mat4 Sf  = glm::scale(I, glm::vec3(0.005, 0.05, 0.005));
    glm::mat4 Tf1  = glm::translate(I, glm::vec3(0.0495, 2.46, 0.0495));
    glm::mat4 Tf2  = glm::translate(I, glm::vec3(-0.0495, 2.46, 0.0495));
    glm::mat4 Tf3  = glm::translate(I, glm::vec3(0.0495, 2.46, -0.0495));
    glm::mat4 Tf4  = glm::translate(I, glm::vec3(-0.0495, 2.46, -0.0495));
    drawObjectTex(cylinder,texIron,P,V,M*Tf1*Sf);
    drawObjectTex(cylinder,texIron,P,V,M*Tf2*Sf);
    drawObjectTex(cylinder,texIron,P,V,M*Tf3*Sf);
    drawObjectTex(cylinder,texIron,P,V,M*Tf4*Sf);

    glm::mat4 Sfarol  = glm::scale(I, glm::vec3(0.045, 0.05, 0.045));
    glm::mat4 Tfarol  = glm::translate(I, glm::vec3(0.0, 2.46, 0.0));

    Material farola = mluz;
    if (dia) {farola.emissive *= 0.3;}
    drawObjectMat(cube,farola,P,V,M*Tfarol*Sfarol);

}

void drawAsfalto(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
        glm::mat4 S = glm::scale(I, glm::vec3(6.0, 1.0, 6.0));
        drawObjectMat(plane, obsidian, P, V, M * S);

        glEnable(GL_POLYGON_OFFSET_FILL);
            glm::mat4 R = glm::rotate   (I, glm::radians(180.0f), glm::vec3(1, 0, 0));
            drawObjectMat(plane, obsidian, P, V, M * S * R);
        glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_CULL_FACE);

}

void drawOficina(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S   = glm::scale(I, glm::vec3(2.0, 0.1, 2.0));
    glm::mat4 T   = glm::translate(I, glm::vec3(0, 0.1, 0));
    glm::mat4 TP1 = glm::translate(I, glm::vec3(0, 1.3, 0));
    glm::mat4 TP2 = glm::translate(I, glm::vec3(0, 2.5, 0));
    drawObjectTex(cube, texWood, P, V, M * T * S);
    drawObjectTex(cube, texWood, P, V, M * TP1 * S);
    drawObjectTex(cube, texWood, P, V, M * TP2 * S);

    glm::mat4 SColumna   = glm::scale(I, glm::vec3(0.2, 1.2, 0.2));
    glm::mat4 TColumna1 = glm::translate(I, glm::vec3(2.0 - 0.2, 1.2, 2.0 - 0.2));
    glm::mat4 TColumna2 = glm::translate(I, glm::vec3(-2.0 + 0.2, 1.2, 2.0 - 0.2));
    glm::mat4 TColumna3 = glm::translate(I, glm::vec3(2.0 - 0.2, 1.2, -2.0+ 0.2));
    glm::mat4 TColumna4 = glm::translate(I, glm::vec3(-2.0 + 0.2, 1.2, -2.0+ 0.2));

    drawObjectTex(cube, texStone, P, V, M * TColumna1 * SColumna);
    drawObjectTex(cube, texStone, P, V, M * TColumna2 * SColumna);
    drawObjectTex(cube, texStone, P, V, M * TColumna3 * SColumna);
    drawObjectTex(cube, texStone, P, V, M * TColumna4 * SColumna);

    glm::mat4 TColumna11 = glm::translate(I, glm::vec3(2.0 - 0.2, 1.2, 2.0 - 1.6));
    glm::mat4 TColumna22 = glm::translate(I, glm::vec3(-2.0 + 0.2, 1.2, 2.0 - 1.6));
    glm::mat4 TColumna33 = glm::translate(I, glm::vec3(2.0 - 0.2, 1.2, -2.0+ 1.6));
    glm::mat4 TColumna44 = glm::translate(I, glm::vec3(-2.0 + 0.2, 1.2, -2.0+ 1.6));
    glm::mat4 TColumna33c = glm::translate(I, glm::vec3(2.0 - 0.2, 1.2, -2.0+ 1.8));
    glm::mat4 TColumna44c = glm::translate(I, glm::vec3(-2.0 + 0.2, 1.2, -2.0+ 1.8));
    glm::mat4 SColumnac   = glm::scale(I, glm::vec3(0.2, 1.2, 0.4));

    drawObjectTex(cube, texStone, P, V, M * TColumna11 * SColumna);
    drawObjectTex(cube, texStone, P, V, M * TColumna22 * SColumna);
    drawObjectTex(cube, texStone, P, V, M * TColumna33 * SColumna);
    drawObjectTex(cube, texStone, P, V, M * TColumna44 * SColumna);
    drawObjectTex(cube, texStone, P, V, M * TColumna33c * SColumnac);
    drawObjectTex(cube, texStone, P, V, M * TColumna44c * SColumnac);

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

    drawObjectTex(cube, texStone, P, V, M * TColumna111 * SColumnaA);
    drawObjectTex(cube, texStone, P, V, M * TColumna222 * SColumnaA);
    drawObjectTex(cube, texStone, P, V, M * TColumna333 * SColumna);
    drawObjectTex(cube, texStone, P, V, M * TColumna444 * SColumna);
    drawObjectTex(cube, texStone, P, V, M * TColumna333c * SColumnac2);
    drawObjectTex(cube, texStone, P, V, M * TColumna111ac * SColumnaAc);
    drawObjectTex(cube, texStone, P, V, M * TColumna111D * SColumnaD);
    drawObjectTex(cube, texStone, P, V, M * TColumna222D * SColumnaD);

    // Decoracion techo
    glm::mat4 SCartel   = glm::scale(I, glm::vec3(1.5, 1.0, 0.05));
    glm::mat4 TCartel = glm::translate(I, glm::vec3(2.0 - 2.0, 3.8, 2.0 - 0.5));
    drawObjectTex(cube, texNeon, P, V, M * TCartel * SCartel);

    glm::mat4 Sbarrad  = glm::scale(I, glm::vec3(0.05, 1.0, 0.05));
    glm::mat4 Tbarrad = glm::translate(I, glm::vec3(2.0 - 0.9, 3.5, 2.0 - 0.6));
    drawObjectMat(cylinder, obsidian, P, V, M * Tbarrad  * Sbarrad);
    glm::mat4 Sbarrai  = glm::scale(I, glm::vec3(0.05, 1.0, 0.05));
    glm::mat4 Tbarrai = glm::translate(I, glm::vec3(-2.0 + 0.9, 3.5, 2.0 - 0.6));
    drawObjectMat(cylinder, obsidian, P, V, M * Tbarrai * Sbarrai);
    glm::mat4 Rbarras  = glm::rotate(I, glm::radians(30.0f),glm::vec3(1,0,0));
    glm::mat4 Tbarraii = glm::translate(I, glm::vec3(-2.0 + 0.9, 3.5, 2.0 - 1.1));
    glm::mat4 Tbarradi = glm::translate(I, glm::vec3(2.0 - 0.9, 3.5, 2.0 - 1.1));
    drawObjectMat(cylinder, obsidian, P, V, M * Tbarraii *Rbarras * Sbarrai);
    drawObjectMat(cylinder, obsidian, P, V, M * Tbarradi *Rbarras  * Sbarrad);

    // Puerta
    glm::mat4 SPuerta   = glm::scale(I, glm::vec3(0.4, 0.5, 0.2));
    glm::mat4 TPuerta = glm::translate(I, glm::vec3(2.0 - 2.0, 0.7, 2.0 - 0.2));
    drawObjectTex(cube, texDoor, P, V, M * TPuerta * SPuerta);

    // Ventanas
    //glenable
    glm::mat4 SVentanaa   = glm::scale(I, glm::vec3(0.5, 0.5, 0.2));
    glm::mat4 SVentanal   = glm::scale(I, glm::vec3(0.2, 0.5, 0.5));
    glm::mat4 TVentana1a = glm::translate(I, glm::vec3(2.0 - 0.9, 1.9, 2.0 - 0.3));
    glm::mat4 TVentana2a = glm::translate(I, glm::vec3(-2.0 + 0.9, 1.9, 2.0 - 0.3));
    glm::mat4 TVentana3a = glm::translate(I, glm::vec3(2.0 - 0.9, 1.9, -2.0 + 0.3));
    glm::mat4 TVentana4a = glm::translate(I, glm::vec3(-2.0 + 0.9, 1.9, -2.0 + 0.3));
    glm::mat4 TVentana1l = glm::translate(I, glm::vec3(2.0 - 0.3, 1.9, 2.0 - 0.9));
    glm::mat4 TVentana2l = glm::translate(I, glm::vec3(-2.0 + 0.3, 1.9, 2.0 - 0.9));
    glm::mat4 TVentana3l = glm::translate(I, glm::vec3(2.0 - 0.3, 1.9, -2.0+ 0.9));
    glm::mat4 TVentana4l = glm::translate(I, glm::vec3(-2.0 + 0.3, 1.9, -2.0+ 0.9));
    glm::mat4 TVentana11l = glm::translate(I, glm::vec3(2.0 - 0.3, 0.7, 2.0 - 0.9));
    glm::mat4 TVentana22l = glm::translate(I, glm::vec3(-2.0 + 0.3, 0.7, 2.0 - 0.9));
    glm::mat4 TVentana33l = glm::translate(I, glm::vec3(2.0 - 0.3, 0.7, -2.0 + 0.9));
    glm::mat4 TVentana44l = glm::translate(I, glm::vec3(-2.0 + 0.3, 0.7, -2.0 + 0.9));
    glm::mat4 TVentana33a = glm::translate(I, glm::vec3(2.0 - 0.9, 0.7, -2.0 + 0.3));
    glm::mat4 TVentana44a = glm::translate(I, glm::vec3(-2.0 + 0.9, 0.7, -2.0 + 0.3));

    Material ventana = mluz;
    if (dia) {ventana.emissive *= glm::vec4(0.3);}
    Material materialesv[] = {ventana,mluzoff};

    drawObjectMat(cube, materialesv[randomBooleanArray[0]] , P, V, M * TVentana1a * SVentanaa);
    drawObjectMat(cube, materialesv[randomBooleanArray[1]] , P, V, M * TVentana2a * SVentanaa);
    drawObjectMat(cube, materialesv[randomBooleanArray[2]] , P, V, M * TVentana3a * SVentanaa);
    drawObjectMat(cube, materialesv[randomBooleanArray[3]] , P, V, M * TVentana4a * SVentanaa);
    drawObjectMat(cube, materialesv[randomBooleanArray[0]] , P, V, M * TVentana1l * SVentanal);
    drawObjectMat(cube, materialesv[randomBooleanArray[1]] , P, V, M * TVentana2l * SVentanal);
    drawObjectMat(cube, materialesv[randomBooleanArray[2]] , P, V, M * TVentana3l * SVentanal);
    drawObjectMat(cube, materialesv[randomBooleanArray[3]] , P, V, M * TVentana4l * SVentanal);
    drawObjectMat(cube, materialesv[randomBooleanArray[4]] , P, V, M * TVentana11l * SVentanal);
    drawObjectMat(cube, materialesv[randomBooleanArray[5]] , P, V, M * TVentana22l * SVentanal);
    drawObjectMat(cube, materialesv[randomBooleanArray[6]], P, V, M * TVentana33l * SVentanal);
    drawObjectMat(cube, materialesv[randomBooleanArray[7]], P, V, M * TVentana44l * SVentanal);
    drawObjectMat(cube, materialesv[randomBooleanArray[6]], P, V, M * TVentana33a * SVentanaa);
    drawObjectMat(cube, materialesv[randomBooleanArray[7]], P, V, M * TVentana44a * SVentanaa);
}

void drawEdificio(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S   = glm::scale(I, glm::vec3(2.0, 0.1, 2.0));
    glm::mat4 T   = glm::translate(I, glm::vec3(0, 0.1, 0));

    glm::mat4 TP1 = glm::translate(I, glm::vec3(0, 1.3, 0));
    glm::mat4 TP2 = glm::translate(I, glm::vec3(0, 2.6, 0));
    glm::mat4 TP3 = glm::translate(I, glm::vec3(0, 3.9, 0));
    glm::mat4 TP4 = glm::translate(I, glm::vec3(0, 5.2, 0));
    glm::mat4 TP5 = glm::translate(I, glm::vec3(0, 6.5, 0));

    drawObjectTex(cube, texWhite, P, V, M * T * S);

    drawObjectTex(cube, texWhite, P, V, M * TP1 * S);
    drawObjectTex(cube, texWhite, P, V, M * TP2 * S);
    drawObjectTex(cube, texWhite, P, V, M * TP3 * S);
    drawObjectTex(cube, texWhite, P, V, M * TP4 * S);
    drawObjectTex(cube, texWhite, P, V, M * TP5 * S);

    // Dibujar las columnas
    glm::mat4 Scol   = glm::scale(I, glm::vec3(0.2, 3.25, 0.2));

    // Esquinas
    glm::mat4 TColumna1 = glm::translate(I, glm::vec3(1.8, 3.25, 1.8 ));
    glm::mat4 TColumna2 = glm::translate(I, glm::vec3(-1.8, 3.25 , 1.8 ));
    glm::mat4 TColumna3 = glm::translate(I, glm::vec3(1.8, 3.25 , -1.8));
    glm::mat4 TColumna4 = glm::translate(I, glm::vec3(-1.8, 3.25 , -1.8));
    drawObjectTex(cube, texWhite, P, V, M * TColumna1 * Scol);
    drawObjectTex(cube, texWhite, P, V, M * TColumna2 * Scol);
    drawObjectTex(cube, texWhite, P, V, M * TColumna3 * Scol);
    drawObjectTex(cube, texWhite, P, V, M * TColumna4 * Scol);

    // Lados
    glm::mat4 TColumna11 = glm::translate(I, glm::vec3(1.8 - 1.0, 3.25 , 1.8));
    glm::mat4 TColumna22 = glm::translate(I, glm::vec3(-1.8, 3.25 , 1.8 - 1.0));
    glm::mat4 TColumna33 = glm::translate(I, glm::vec3(1.8, 3.25 , -1.8 + 1.0));
    glm::mat4 TColumna44 = glm::translate(I, glm::vec3(-1.8 + 1.0, 3.25 , -1.8));
    drawObjectTex(cube, texWhite, P, V, M * TColumna11 * Scol);
    drawObjectTex(cube, texWhite, P, V, M * TColumna22 * Scol);
    drawObjectTex(cube, texWhite, P, V, M * TColumna33 * Scol);
    drawObjectTex(cube, texWhite, P, V, M * TColumna44 * Scol);

    glm::mat4 TColumna111 = glm::translate(I, glm::vec3(1.8, 3.25 , 1.8 - 1.0));
    glm::mat4 TColumna222 = glm::translate(I, glm::vec3(-1.8 + 1.0, 3.25 , 1.8));
    glm::mat4 TColumna333 = glm::translate(I, glm::vec3(1.8 - 1.0, 3.25 , -1.8));
    glm::mat4 TColumna444 = glm::translate(I, glm::vec3(-1.8, 3.25 , -1.8 + 1.0));
    drawObjectTex(cube, texWhite, P, V, M * TColumna111 * Scol);
    drawObjectTex(cube, texWhite, P, V, M * TColumna222 * Scol);
    drawObjectTex(cube, texWhite, P, V, M * TColumna333 * Scol);
    drawObjectTex(cube, texWhite, P, V, M * TColumna444 * Scol);

    // Dibujar puerta y toldo
    glm::mat4 SPuerta1 = glm::scale(I, glm::vec3(0.3, 0.6 , 0.1));
    glm::mat4 TPuerta1 = glm::translate(I, glm::vec3(-0.3, 0.6, -1.95));
    glm::mat4 SPuerta2 = glm::scale(I, glm::vec3(0.3, 0.6 , 0.1));
    glm::mat4 TPuerta2 = glm::translate(I, glm::vec3(0.3, 0.6 , -1.95));
    drawObjectTex(cube, texDoor, P, V, M * TPuerta1 * SPuerta1);
    drawObjectTex(cube, texDoor, P, V, M * TPuerta2 * SPuerta2);

    glm::mat4 SToldo = glm::scale(I, glm::vec3(1.0, 0.05 , 1.0));
    glm::mat4 TToldo = glm::translate(I, glm::vec3(0, 1.3, -2.0));
    drawObjectMat(cube, obsidian, P, V, M * TToldo * SToldo);
    glm::mat4 SMastil1 = glm::scale(I, glm::vec3(0.05, 0.65 , 0.05));
    glm::mat4 TMastil1 = glm::translate(I, glm::vec3(0.8, 0.65, -2.7));
    glm::mat4 SMastil2 = glm::scale(I, glm::vec3(0.05, 0.65 , 0.05));
    glm::mat4 TMastil2 = glm::translate(I, glm::vec3(-0.8, 0.65, -2.7));
    drawObjectMat(cylinder, obsidian, P, V, M * TMastil1 * SMastil1);
    drawObjectMat(cylinder, obsidian, P, V, M * TMastil2 * SMastil2);

    // Tapar ventanas abajo
    glm::mat4 SVcT = glm::scale(I, glm::vec3(0.6, 0.6 , 0.1));
    glm::mat4 TVcT = glm::translate(I, glm::vec3(0, 0.6, 1.9));
    drawObjectTex(cube, texWhite, P, V, M * TVcT * SVcT);
    glm::mat4 SVcD = glm::scale(I, glm::vec3(0.1, 0.6 , 0.6));
    glm::mat4 TVcD = glm::translate(I, glm::vec3(1.9, 0.6, 0));
    drawObjectTex(cube, texWhite, P, V, M * TVcD * SVcD);
    glm::mat4 SVcI = glm::scale(I, glm::vec3(0.1, 0.6 , 0.6));
    glm::mat4 TVcI = glm::translate(I, glm::vec3(-1.9, 0.6, 0));
    drawObjectTex(cube, texWhite, P, V, M * TVcI * SVcI);

    // Balcones
    glm::mat4 Sbbase1 = glm::scale(I, glm::vec3(0.4, 0.05 , 0.8));
    glm::mat4 Tbbase1 = glm::translate(I, glm::vec3(2.4, 1.4, 0));
    drawObjectMat(cube, obsidian, P, V, M * Tbbase1 * Sbbase1);
    glm::mat4 Sblateral1 = glm::scale(I, glm::vec3(0.4, 0.3 , 0.05));
    glm::mat4 Tblateral1 = glm::translate(I, glm::vec3(2.4, 1.7, 0.75));
    drawObjectMat(cube, obsidian, P, V, M * Tblateral1 * Sblateral1);
    glm::mat4 Sblateral11 = glm::scale(I, glm::vec3(0.4, 0.3 , 0.05));
    glm::mat4 Tblateral11 = glm::translate(I, glm::vec3(2.4, 1.7, -0.75));
    drawObjectMat(cube, obsidian, P, V, M * Tblateral11 * Sblateral11);
    glm::mat4 Sblateral111 = glm::scale(I, glm::vec3(0.05, 0.3 ,0.8));
    glm::mat4 Tblateral111 = glm::translate(I, glm::vec3(2.75, 1.7, 0));
    drawObjectMat(cube, obsidian, P, V, M * Tblateral111 * Sblateral111);

    glm::mat4 Sbbase2 = glm::scale(I, glm::vec3(0.4, 0.05 , 0.8));
    glm::mat4 Tbbase2 = glm::translate(I, glm::vec3(2.4, 2.7, 0));
    drawObjectMat(cube, obsidian, P, V, M * Tbbase2 * Sbbase2);
    glm::mat4 Sblateral2 = glm::scale(I, glm::vec3(0.4, 0.3 , 0.05));
    glm::mat4 Tblateral2 = glm::translate(I, glm::vec3(2.4, 3.0, 0.75));
    drawObjectMat(cube, obsidian, P, V, M * Tblateral2 * Sblateral2);
    glm::mat4 Sblateral22 = glm::scale(I, glm::vec3(0.4, 0.3 , 0.05));
    glm::mat4 Tblateral22 = glm::translate(I, glm::vec3(2.4, 3.0, -0.75));
    drawObjectMat(cube, obsidian, P, V, M * Tblateral22 * Sblateral22);
    glm::mat4 Sblateral222 = glm::scale(I, glm::vec3(0.05, 0.3 ,0.8));
    glm::mat4 Tblateral222 = glm::translate(I, glm::vec3(2.75, 3.0, 0));
    drawObjectMat(cube, obsidian, P, V, M * Tblateral222 * Sblateral222);

    glm::mat4 Sbbase3 = glm::scale(I, glm::vec3(0.4, 0.05 , 0.8));
    glm::mat4 Tbbase3 = glm::translate(I, glm::vec3(2.4, 4.0, 0));
    drawObjectMat(cube, obsidian, P, V, M * Tbbase3 * Sbbase3);
    glm::mat4 Sblateral3 = glm::scale(I, glm::vec3(0.4, 0.3 , 0.05));
    glm::mat4 Tblateral3 = glm::translate(I, glm::vec3(2.4, 4.3, 0.75));
    drawObjectMat(cube, obsidian, P, V, M * Tblateral3 * Sblateral3);
    glm::mat4 Sblateral33 = glm::scale(I, glm::vec3(0.4, 0.3 , 0.05));
    glm::mat4 Tblateral33 = glm::translate(I, glm::vec3(2.4, 4.3, -0.75));
    drawObjectMat(cube, obsidian, P, V, M * Tblateral33 * Sblateral33);
    glm::mat4 Sblateral333 = glm::scale(I, glm::vec3(0.05, 0.3 ,0.8));
    glm::mat4 Tblateral333 = glm::translate(I, glm::vec3(2.75, 4.3, 0));
    drawObjectMat(cube, obsidian, P, V, M * Tblateral333 * Sblateral333);

    glm::mat4 Sbbase4 = glm::scale(I, glm::vec3(0.4, 0.05 , 0.8));
    glm::mat4 Tbbase4 = glm::translate(I, glm::vec3(2.4, 5.3, 0));
    drawObjectMat(cube, obsidian, P, V, M * Tbbase4 * Sbbase4);
    glm::mat4 Sblateral4 = glm::scale(I, glm::vec3(0.4, 0.3 , 0.05));
    glm::mat4 Tblateral4 = glm::translate(I, glm::vec3(2.4, 5.6, 0.75));
    drawObjectMat(cube, obsidian, P, V, M * Tblateral4 * Sblateral4);
    glm::mat4 Sblateral44 = glm::scale(I, glm::vec3(0.4, 0.3 , 0.05));
    glm::mat4 Tblateral44 = glm::translate(I, glm::vec3(2.4, 5.6, -0.75));
    drawObjectMat(cube, obsidian, P, V, M * Tblateral44 * Sblateral44);
    glm::mat4 Sblateral444 = glm::scale(I, glm::vec3(0.05, 0.3 ,0.8));
    glm::mat4 Tblateral444 = glm::translate(I, glm::vec3(2.75, 5.6, 0));
    drawObjectMat(cube, obsidian, P, V, M * Tblateral444 * Sblateral444);

 //Dibujar ventanas
    glm::mat4 SInterior = glm::scale(I, glm::vec3(1.9, 3.2 , 1.9));
    glm::mat4 TInterior = glm::translate(I, glm::vec3(0, 3.2, 0));
    Material ventana = mluz;
    if (dia) {
        ventana.emissive = glm::vec4(0.3);
        drawObjectMat(cube, ventana, P, V, M * TInterior * SInterior);
    } else {
        drawObjectMat(cube, ventana, P, V, M * TInterior * SInterior);
    }

}

void drawRestaurante(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 Sbase   = glm::scale(I, glm::vec3(2.0, 0.1, 2.0));
    glm::mat4 Tbase   = glm::translate(I, glm::vec3(0, 0.1, 0));
    drawObjectMat(cube,ruby,P,V,M*Tbase*Sbase);

    glm::mat4 Stecho  = glm::scale(I, glm::vec3(2.0, 0.1, 1.0));
    glm::mat4 Ttecho   = glm::translate(I, glm::vec3(0, 2.1, 1.0));
    drawObjectMat(cube,ruby,P,V,M*Ttecho*Stecho);

    //barra
    glm::mat4 Sbarra   = glm::scale(I, glm::vec3(1.5, 0.4, 0.1));
    glm::mat4 Tbarra   = glm::translate(I, glm::vec3(0.5, 0.6, 1.3));
    drawObjectMat(cube,ruby,P,V,M*Tbarra*Sbarra);
    glm::mat4 Sasiento   = glm::scale(I, glm::vec3(0.08, 0.05, 0.08));
    glm::mat4 Tasiento   = glm::translate(I, glm::vec3(1.0, 0.65, 1.0));
    drawObjectMat(cylinder,obsidian,P,V,M*Tasiento*Sasiento);
    glm::mat4 Sasoporte   = glm::scale(I, glm::vec3(0.03, 0.2, 0.03));
    glm::mat4 Tasoporte   = glm::translate(I, glm::vec3(1.0, 0.4, 1.0));
    drawObjectMat(cylinder,obsidian,P,V,M*Tasoporte*Sasoporte);
    glm::mat4 Tasiento2   = glm::translate(I, glm::vec3(1.5, 0.65, 1.0));
    glm::mat4 Tasoporte2   = glm::translate(I, glm::vec3(1.5, 0.4, 1.0));
    glm::mat4 Tasiento3   = glm::translate(I, glm::vec3(0.5, 0.65, 1.0));
    glm::mat4 Tasoporte3  = glm::translate(I, glm::vec3(0.5, 0.4, 1.0));
    drawObjectMat(cylinder,obsidian,P,V,M*Tasiento2*Sasiento);
    drawObjectMat(cylinder,obsidian,P,V,M*Tasiento3*Sasiento);
    drawObjectMat(cylinder,obsidian,P,V,M*Tasoporte2*Sasoporte);
    drawObjectMat(cylinder,obsidian,P,V,M*Tasoporte3*Sasoporte);

    // Cristalera
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    glEnable(GL_CULL_FACE);
        glm::mat4 Smurot   = glm::scale(I, glm::vec3(2.0, 1.0, 0.1));
        glm::mat4 Tmurot   = glm::translate(I, glm::vec3(0, 1.2, 1.9));
        drawObjectMat(cube,mCristal,P,V,M*Tmurot*Smurot);
        glm::mat4 Smurod   = glm::scale(I, glm::vec3(2.0, 1.0, 0.1));
        glm::mat4 Tmurod   = glm::translate(I, glm::vec3(0, 1.2, 0.1));
        drawObjectMat(cube,mCristal,P,V,M*Tmurod*Smurod);
        glm::mat4 Smurold   = glm::scale(I, glm::vec3(0.1, 1.0, 1.0));
        glm::mat4 Tmurold   = glm::translate(I, glm::vec3(1.9, 1.2, 1.0));
        drawObjectMat(cube,mCristal,P,V,M*Tmurold*Smurold);
        glm::mat4 Smuroli   = glm::scale(I, glm::vec3(0.1, 1.0, 1.0));
        glm::mat4 Tmuroli   = glm::translate(I, glm::vec3(-1.9, 1.2, 1.0));
        drawObjectMat(cube,mCristal,P,V,M*Tmuroli*Smuroli);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);

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

    drawObjectMat(cylinder,mFaro,P,V,M*T*S*Rini);

}

void drawChasisUtilitario (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    //base turismo y todoterreno
    glm::mat4 S = scale(I, glm::vec3(0.4,0.15,0.8));
    glm::mat4 T = translate(I, glm::vec3(0,0.3,0));
    drawObjectMat(cube,pSilver,P,V,M*T*S);

    //coche normal familiar
    glm::mat4 Scab = scale(I, glm::vec3(0.38,0.15,0.55));
    glm::mat4 Tcab = translate(I, glm::vec3(0,0.6,0.2));
    drawObjectMat(cube,pSilver,P,V,M*Tcab*Scab);
}

void drawChasisTodoterreno (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    //base turismo y todoterreno
    glm::mat4 S = scale(I, glm::vec3(0.4,0.2,0.8));
    glm::mat4 T = translate(I, glm::vec3(0,0.4,0));
    drawObjectMat(cube,pSilver,P,V,M*T*S);

    //coche todoterreno
    glm::mat4 Scab = scale(I, glm::vec3(0.4,0.30,0.55));
    glm::mat4 Tcab = translate(I, glm::vec3(0,0.6,0.25));
    drawObjectMat(cube,pSilver,P,V,M*Tcab*Scab);

    glm::mat4 SR = scale(I, glm::vec3(0.4/2.0,0.15/2.0,0.4/2.0));
    glm::mat4 RR = rotate(I, glm::radians(90.0f), glm::vec3(1, 0, 0));
    glm::mat4 TR = translate(I, glm::vec3(0,0.55,0.9));
    drawObjectMat(cylinder, obsidian, P, V, M * TR * RR * SR);
}

void drawChasisDeportivo (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    //base coche deportivo
    glm::mat4 S1 = scale(I, glm::vec3(0.4,0.02,0.78));
    glm::mat4 T1 = translate(I, glm::vec3(0,0.10,-0.15));
    drawObjectMat(cube,pSilver,P,V,M*T1*S1);

    glm::mat4 S2 = scale(I, glm::vec3(0.4,0.02,0.82));
    glm::mat4 T2 = translate(I, glm::vec3(0,0.12,-0.15));
    drawObjectMat(cube,pSilver,P,V,M*T2*S2);

    glm::mat4 S3 = scale(I, glm::vec3(0.4,0.02,0.84));
    glm::mat4 T3 = translate(I, glm::vec3(0,0.14,-0.15));
    drawObjectMat(cube,pSilver,P,V,M*T3*S3);

    glm::mat4 S4 = scale(I, glm::vec3(0.4,0.02,0.88));
    glm::mat4 T4 = translate(I, glm::vec3(0,0.16,-0.15));
    drawObjectMat(cube,pSilver,P,V,M*T4*S4);

    glm::mat4 S5 = scale(I, glm::vec3(0.4,0.02,0.88));
    glm::mat4 T5 = translate(I, glm::vec3(0,0.18,-0.15));
    drawObjectMat(cube,pSilver,P,V,M*T5*S5);

    glm::mat4 S6 = scale(I, glm::vec3(0.4,0.02,0.88));
    glm::mat4 T6 = translate(I, glm::vec3(0,0.2,-0.15));
    drawObjectMat(cube,pSilver,P,V,M*T6*S6);

    glm::mat4 S7 = scale(I, glm::vec3(0.4,0.02,0.84));
    glm::mat4 T7 = translate(I, glm::vec3(0,0.22,-0.15));
    drawObjectMat(cube,pSilver,P,V,M*T7*S7);

    glm::mat4 S8 = scale(I, glm::vec3(0.4,0.02,0.80));
    glm::mat4 T8 = translate(I, glm::vec3(0,0.24,-0.15));
    drawObjectMat(cube,pSilver,P,V,M*T8*S8);

    glm::mat4 SFaro = scale(I, glm::vec3(0.05,0.05,0.05));
    glm::mat4 TFaro1 = translate(I, glm::vec3(0.3,0.3,-0.75));
    drawObjectMat(cube,pSilver,P,V,M*TFaro1*SFaro);
    glm::mat4 TFaro2 = translate(I, glm::vec3(-0.3,0.3,-0.75));
    drawObjectMat(cube,pSilver,P,V,M*TFaro2*SFaro);

    //cabina base
    glm::mat4 SCB1 = scale(I, glm::vec3(0.4,0.02,0.6));
    glm::mat4 TCB1 = translate(I, glm::vec3(0,0.26,0.02));
    drawObjectMat(cube,pSilver,P,V,M*TCB1*SCB1);

    glm::mat4 SCB2 = scale(I, glm::vec3(0.4,0.02,0.58));
    glm::mat4 TCB2 = translate(I, glm::vec3(0,0.28,0.03));
    drawObjectMat(cube,pSilver,P,V,M*TCB2*SCB2);

    //cabina
    glm::mat4 SC1 = scale(I, glm::vec3(0.4,0.02,0.55));
    glm::mat4 TC1 = translate(I, glm::vec3(0,0.3,0.04));
    drawObjectMat(cube,pSilver,P,V,M*TC1*SC1);

    glm::mat4 SC2 = scale(I, glm::vec3(0.38,0.02,0.53));
    glm::mat4 TC2 = translate(I, glm::vec3(0,0.32,0.04));
    drawObjectMat(cube,pSilver,P,V,M*TC2*SC2);

    glm::mat4 SC3 = scale(I, glm::vec3(0.36,0.02,0.51));
    glm::mat4 TC3 = translate(I, glm::vec3(0,0.34,0.04));
    drawObjectMat(cube,pSilver,P,V,M*TC3*SC3);

    glm::mat4 SC4 = scale(I, glm::vec3(0.34,0.02,0.49));
    glm::mat4 TC4 = translate(I, glm::vec3(0,0.36,0.04));
    drawObjectMat(cube,pSilver,P,V,M*TC4*SC4);

    glm::mat4 SC5 = scale(I, glm::vec3(0.32,0.02,0.47));
    glm::mat4 TC5 = translate(I, glm::vec3(0,0.38,0.04));
    drawObjectMat(cube,pSilver,P,V,M*TC5*SC5);

    glm::mat4 SC6 = scale(I, glm::vec3(0.3,0.02,0.45));
    glm::mat4 TC6 = translate(I, glm::vec3(0,0.4,0.04));
    drawObjectMat(cube,pSilver,P,V,M*TC6*SC6);

    //aleron base
    glm::mat4 SA = scale(I, glm::vec3(0.42,0.005,0.05));
    glm::mat4 TA = translate(I, glm::vec3(0,0.45,0.75));
    drawObjectMat(cube,pSilver,P,V,M*TA*SA);

    //aleron lados
    glm::mat4 SAT = scale(I, glm::vec3(0.005,0.02,0.04));
    glm::mat4 TAD1 = translate(I, glm::vec3(0.3,0.43,0.74));
    drawObjectMat(cube,pSilver,P,V,M*TAD1*SAT);
    glm::mat4 TAD2 = translate(I, glm::vec3(0.3,0.41,0.72));
    drawObjectMat(cube,pSilver,P,V,M*TAD2*SAT);
    glm::mat4 TAD3 = translate(I, glm::vec3(0.3,0.39,0.70));
    drawObjectMat(cube,pSilver,P,V,M*TAD3*SAT);
    glm::mat4 TAD4 = translate(I, glm::vec3(0.3,0.37,0.68));
    drawObjectMat(cube,pSilver,P,V,M*TAD4*SAT);
    glm::mat4 TAD5 = translate(I, glm::vec3(0.3,0.35,0.66));
    drawObjectMat(cube,pSilver,P,V,M*TAD5*SAT);
    glm::mat4 TAD6 = translate(I, glm::vec3(0.3,0.33,0.64));
    drawObjectMat(cube,pSilver,P,V,M*TAD6*SAT);
    glm::mat4 TAD7 = translate(I, glm::vec3(0.3,0.31,0.62));
    drawObjectMat(cube,pSilver,P,V,M*TAD7*SAT);
    glm::mat4 TAD8 = translate(I, glm::vec3(0.3,0.29,0.6));
    drawObjectMat(cube,pSilver,P,V,M*TAD8*SAT);
    glm::mat4 TAI1 = translate(I, glm::vec3(-0.3,0.43,0.74));
    drawObjectMat(cube,pSilver,P,V,M*TAI1*SAT);
    glm::mat4 TAI2 = translate(I, glm::vec3(-0.3,0.41,0.72));
    drawObjectMat(cube,pSilver,P,V,M*TAI2*SAT);
    glm::mat4 TAI3 = translate(I, glm::vec3(-0.3,0.39,0.70));
    drawObjectMat(cube,pSilver,P,V,M*TAI3*SAT);
    glm::mat4 TAI4 = translate(I, glm::vec3(-0.3,0.37,0.68));
    drawObjectMat(cube,pSilver,P,V,M*TAI4*SAT);
    glm::mat4 TAI5 = translate(I, glm::vec3(-0.3,0.35,0.66));
    drawObjectMat(cube,pSilver,P,V,M*TAI5*SAT);
    glm::mat4 TAI6 = translate(I, glm::vec3(-0.3,0.33,0.64));
    drawObjectMat(cube,pSilver,P,V,M*TAI6*SAT);
    glm::mat4 TAI7 = translate(I, glm::vec3(-0.3,0.31,0.62));
    drawObjectMat(cube,pSilver,P,V,M*TAI7*SAT);
    glm::mat4 TAI8 = translate(I, glm::vec3(-0.3,0.29,0.6));
    drawObjectMat(cube,pSilver,P,V,M*TAI8*SAT);

}

void drawLucesPosicion (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 Sf = scale(I, glm::vec3(0.1/2.0,0.08/2.0,0.01/2.0));

    //luces traseras turismo
    glm::mat4 Tf1 =  translate(I, glm::vec3(0.3,0.35,0.8));
    glm::mat4 Tf2 =  translate(I, glm::vec3(-0.3,0.35,0.8));

    Material luzPos = mluzRed;
    if (dia) {
        luzPos.emissive = glm::vec4(luzPos.emissive.x * 0.6, luzPos.emissive.y * 0.6, luzPos.emissive.z * 0.6, 1.0);
    }

    drawObjectMat(cube, luzPos, P, V, M * Tf1 * Sf);
    drawObjectMat(cube, luzPos, P, V, M * Tf2 * Sf);

}

void drawLucesPosicionDeportivo (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 Sf = scale(I, glm::vec3(0.1/2.0,0.08/2.0,0.01/2.0));

    //luces traseras deportivo
    glm::mat4 Tf1 =  translate(I, glm::vec3(0.3,0.18,0.73));
    glm::mat4 Tf2 =  translate(I, glm::vec3(-0.3,0.18,0.73));

    Material luzP = mluzRed;
    if (dia) {
        luzP.emissive = glm::vec4(luzP.emissive.x * 0.6, luzP.emissive.y * 0.6, luzP.emissive.z * 0.6, 1.0);
    }

    drawObjectMat(cube, luzP, P, V, M * Tf1 * Sf);
    drawObjectMat(cube, luzP, P, V, M * Tf2 * Sf);

}

void drawRuedasUtilitario (glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    //ruedas turismo
    glm::mat4 Tru1 = translate(I, glm::vec3(0.4,0.15,0.6));
    glm::mat4 Tru2 = translate(I, glm::vec3(-0.4,0.15,0.6));
    glm::mat4 Tru3 = translate(I, glm::vec3(-0.4,0.15,-0.4));
    glm::mat4 Tru4 = translate(I, glm::vec3(0.4,0.15,-0.4));

    // Hacer girar las ruedas al moverse el coche
    glm::mat4 R = glm::rotate(I, glm::radians(rotRueda), glm::vec3(-1, 0, 0));

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
    drawObjectTex(cylinder, texTire, P, V, M * R * S);

}

void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",true);
    shaders.setMaterial("umaterial", material);
    model.renderModel(GL_FILL);

}

void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials", false);
    shaders.setTextures("utextures",textures);
    if(textures.normal!=0) shaders.setBool("uWithNormals",true);
    else                   shaders.setBool("uWithNormals",false);
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

    if (faroX >  7.0) faroX =  7.0;
    if (faroX < -7.0) faroX = -7.0;
    if (faroZ >  7.0) faroZ =  7.0;
    if (faroZ < -7.0) faroZ = -7.0;

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
