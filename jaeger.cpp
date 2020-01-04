#include <Windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gl/glext.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#include <string>

#include <cstdio>
#include <cmath>

using namespace std;

// Particle engine
const int ParticleCount = 500;
const int JetPackParticleCount = 500;
float jetPackParticleSize = 1;

float speed = -90;
float perspectiveX = 0, perspectiveY = -3.3, perspectiveZ = -16;
float orthoX = 0, orthoY = 0.5, orthoZ = 0;
float bridgeDegree = 0, bridgeLineUp = 0;

float animationX = 0, animationY = -0.71, animationZ = 0;
float ionBlasterX = 0, ionBlasterY = 0, ionBlasterZ = 0;
float ionBlasterXDegree = 0, ionBlasterYDegree = 0, ionBlasterZDegree = 0;

bool rotate = false;
bool rotateLeft = false;
bool rotateRight = false;

bool isOrtho = false;
bool isTexture = false;

int mode = 0, moveFinger = 0;
int textureMode = 0;

float fingerDegree[10] = {}, handDegree[2] = {};
float armDegreeX[2] = {}, armDegreeY[2] = {};
float robotHeight = 0.5;
float calfDegree[2] = {}, thighDegree[2] = {};

double w = 1920;
double h = 1080;
double ar = w / h; // aspect ratio

float adjustSpeed1 = 0.05;
float adjustSpeed2 = 0.05;
float v = 0.5;
float v1 = 2.41;
float v2 = 4.92;
float v3 = 0;
float v4 = 0;
float v5 = 0;
float v6 = 0.14;
float v7 = 90;

int fingerNo = 0; // individual finger control on, 1 - 10
bool fingerControl = false;
int fingerCounter = 0;

int armNo = 0; // individual arm control on, 1 - 2
bool armControl = false;
bool armRotate = false;
int handCounter = 0;
int armCounter = 0;

int legNo = 0; // individual leg control on, 1 - 2
bool legControl = false;
int calfCounter = 0;
int thighCounter = 0;

bool animationControl = false;
bool walkControl = false;
bool runControl = false;
int walkingFlag[2] = {0, 1};
int direction = 0;
bool insultControl = false;
bool walkStationary = true;
float walkDirectionDegree = 0;

bool itemControl = false;
bool jetpack = false;
bool ionBlaster = false;
bool sword = false;

bool fightMode = false;
int fightModeFlag[2] = {0, 0};
bool shootFlag = false;
float ionEnergyZ = 0;
bool flyMode = false;

float ascendSpeed = 0.01;
void flyVertical(int direction);
void flyHorizontal(int direction);
float swingDistance = 0;
bool swingFlag = false;
int exponent = 1;
float flyForwardDegree = 0;

void shoot();
bool shooting = false;
//
int gluDrawStyles[4] = {100012, 100010, 100011, 100011};
int glDrawStyles[4] = {0x0007, 0x0000, 0x0002, 0x0001};
int glewDrawStyles[4] = {0x0009, 0x0000, 0x0002, 0x0001};

// drawStyle
int selectedGluDrawStyles = 100011;
int selectedGlDrawStyles = 0x0002;
int selectedGlewDrawStyles = 0x0002;
int stylesNo = 0;

// View mode

// texture
GLuint texture = 0;
BITMAP BMP;
HBITMAP hBMP = NULL;
// string textures[3] = {"Brick.bmp", "Wood.bmp", "Metal.bmp"};
int textureNo = 0;

// get dir path of project root
string file_path = __FILE__;
string dir_path = file_path.substr(0, file_path.rfind("\\"));

string projectRoot;
char temp[100];

void resetRobot();

bool spawn = false;
int frameDisplayCount = 0;

bool light = false;
float diffuseColor[] = {0.64f, 0.48f, 0.76f};
float diffusePosition[] = {-60.0f, -0.65f, 20.0f};
float objectColor[] = {0.64f, 0.48f, 0.76f};

// use dedicated GPU to run
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
extern "C"
{
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

string textureNames[50] = {};
int textureCount = 0;
GLuint *textures = new GLuint[50];

void initTexture(string textureName)
{
    if (!isTexture)
    {
        glDisable(GL_TEXTURE_2D);
    }
    else
    {
        bool found = false;
        int texturePointer = -1;
        for (int i = 0; i < textureCount + 1; i++)
        {
            if (textureNames[i] == textureName)
            {
                found = true;
                texturePointer = i;
                break;
            }
        }

        glEnable(GL_TEXTURE_2D);
        if (found)
        {
            // glActiveTexture(GL_TEXTURE0 + texturePointer); // Texture unit 0
            glBindTexture(GL_TEXTURE_2D, textures[texturePointer]);
        }
        else
        {
            strcpy(temp, dir_path.c_str());
            strcat(temp, "\\texture\\");
            strcat(temp, textureName.c_str());

            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), temp, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
            GetObject(hBMP, sizeof(BMP), &BMP);

            //glActiveTexture(GL_TEXTURE0 + textureCount); // Texture unit 0
            glBindTexture(GL_TEXTURE_2D, textures[textureCount]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                            GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth,
                         BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

            DeleteObject(hBMP);
            textureNames[textureCount] = textureName;
            textureCount++;
        }
    }
}

void removeTexture()
{
    // glDisable(GL_TEXTURE_2D);
    // glDeleteTextures(1, &texture);
}

void incrementArray(float *arr, float value, int size)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] += value;
        printf("%f\n", arr[i]);
    }
}

void controls(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_ENTER:
            if (fightMode)
                shootFlag = true;
            break;

        case GLFW_KEY_ESCAPE:
            printf("v: %f\tv1: %f\tv2: %f\tv3: %f\n", v, v1, v2, v3);
            printf("v4: %f\tv5: %f\tv6: %f\tv7: %f\n", v4, v5, v6, v7);
            printf("ionBlasterX: %f\tionBlasterY: %f\tionBlasterZ: %f\n", ionBlasterX, ionBlasterY, ionBlasterZ, v7);

            // printf("orthoZ: %f\texponent: %d\n", orthoZ, exponent);
            printf("perspectiveX: %f\tperspectiveY: %f\tspeed: %f\t\n", perspectiveX, perspectiveY, speed);

            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_GRAVE_ACCENT: // key ` (~) change shape draw style
            stylesNo += 1;
            if (stylesNo % 4 == 0)
                stylesNo = 0;
            selectedGluDrawStyles = gluDrawStyles[stylesNo];
            selectedGlDrawStyles = glDrawStyles[stylesNo];
            selectedGlewDrawStyles = glewDrawStyles[stylesNo];
            break;
        case GLFW_KEY_F1: // viewport control
            mode = 0;
            if (isOrtho)
                isOrtho = false;
            else
                isOrtho = true;
            break;
        case GLFW_KEY_F2: // finger
            mode = 1;
            printf("mode = %d\n", mode);
            break;
        case GLFW_KEY_F3: // hand
            mode = 2;
            printf("mode = %d\n", mode);
            break;
        case GLFW_KEY_F4: // arm
            mode = 3;
            printf("mode = %d\n", mode);
            break;
        case GLFW_KEY_F5: // calf
            mode = 4;
            printf("mode = %d\n", mode);
            break;
        case GLFW_KEY_F6: // thigh
            mode = 5;
            printf("mode = %d\n", mode);
            break;
        case GLFW_KEY_F7: // finger control
            if (fingerControl)
            {
                fingerControl = false;
                mode = 0;
                printf("fingerControl = %s\tmode = %d\n", fingerControl, mode);
            }
            else
            {
                fingerControl = true;
                armControl = false;
                legControl = false;
                animationControl = false;
                mode = 1;
            }
            break;
        case GLFW_KEY_F8: // arm control
            if (armControl)
            {
                armControl = false;
                mode = 0;
            }
            else // user need to press F3 or F4
            {
                fingerControl = false;
                armControl = true;
                legControl = false;
                animationControl = false;
                itemControl = false;
            }
            break;
        case GLFW_KEY_F9: // leg control
            if (legControl)
            {
                legControl = false;
                mode = 0;
            }
            else // user need to press F5 or F6
            {
                fingerControl = false;
                armControl = false;
                legControl = true;
                animationControl = false;
                itemControl = false;
            }
            break;
        case GLFW_KEY_F10: // animation
            if (animationControl)
            {
                animationControl = false;
                mode = 0;
            }
            else // user need to press F5 or F6
            {
                fingerControl = false;
                armControl = false;
                legControl = false;
                animationControl = true;
                itemControl = false;
            }
            break;
        case GLFW_KEY_F11: // weapon, change shape
            if (itemControl)
            {
                itemControl = false;
                mode = 0;
            }
            else // user need to press F5 or F6
            {
                fingerControl = false;
                armControl = false;
                legControl = false;
                animationControl = false;
                itemControl = true;
            }
            break;
        case GLFW_KEY_F12: // enable texture
            if (isTexture)
            {
                isTexture = false;
                spawn = false;
            }
            else
            {
                spawn = true;
                isTexture = true;
            }
            break;
        case GLFW_KEY_SPACE: // rotate world
            if (rotate)
                rotate = false;
            else
                rotate = true;
            break;
        case GLFW_KEY_COMMA: // rotate world left / right
            speed -= 1;
            break;
        case GLFW_KEY_PERIOD: // rotate world left / right
            speed += 1;
            break;
        case GLFW_KEY_RIGHT:
            switch (mode)
            {
            case 0:
                perspectiveX += 0.1;
                orthoX += 0.1;
                break;
            case 3:
                if (armControl)
                    armDegreeY[armNo - 1] -= 2;
                break;
            default:
                break;
            }
            break;
        case GLFW_KEY_LEFT:
            switch (mode)
            {
            case 0:
                perspectiveX -= 0.1;
                orthoX -= 0.1;
                break;
            case 3:
                if (armControl)
                    armDegreeY[armNo - 1] += 2;
                break;
            default:
                break;
            }
            break;
        case GLFW_KEY_UP:
            switch (mode)
            {
            case 0:
                perspectiveY += 0.1;
                orthoY += 0.1;
                break;
            case 1:
                if (fingerControl)
                    fingerDegree[fingerNo - 1] += 2;
                else
                    incrementArray(fingerDegree, 2, 10);
                break;
            case 2:
                if (armControl)
                    handDegree[armNo - 1] += 1;
                else
                    incrementArray(handDegree, 1, 2);
                break;
            // case 3:
            //     robotHeight += 0.1;
            //     break;
            case 3:
                if (armControl)
                {
                    armDegreeX[armNo - 1] += 1;
                    // printf("armDegree1 = %f\tarmDegree2 = %f\tCurrent = %d\n",
                    //        armDegree[0], armDegree[1], armNo - 1);
                }
                else
                    incrementArray(armDegreeX, 1, 2);
                break;
            case 4:
                if (legControl)
                    calfDegree[legNo - 1] += 1;
                else
                    incrementArray(calfDegree, 1, 2);
                break;
            case 5:
                if (legControl)
                    thighDegree[legNo - 1] += 1;
                else
                    incrementArray(thighDegree, 1, 2);
                break;
            case 6:
                if (flyMode)
                {
                    if (direction == -3)
                        exponent = 1;
                    exponent++;
                    direction = 3;
                    flyVertical(1);
                    jetPackParticleSize = 1.5;
                }
                break;
            default:
                break;
            }
            break;
        case GLFW_KEY_DOWN:
            switch (mode)
            {
            case 0:
                perspectiveY -= 0.1;
                orthoY -= 0.1;
                break;
            case 1:
                if (fingerControl)
                    fingerDegree[fingerNo - 1] -= 2;
                else
                    incrementArray(fingerDegree, -2, 10);
                break;
            case 2:
                if (armControl)
                    handDegree[armNo - 1] -= 1;
                else
                    incrementArray(handDegree, -1, 2);
                break;
            // case 3:
            //     robotHeight -= 0.1;
            //     break;
            case 3:
                if (armControl)
                    armDegreeX[armNo - 1] -= 1;
                else
                    incrementArray(armDegreeX, -1, 2);
                break;
            case 4:
                if (legControl)
                    calfDegree[legNo - 1] -= 1;
                else
                    incrementArray(calfDegree, -1, 2);
                break;
            case 5:
                if (legControl)
                    thighDegree[legNo - 1] -= 1;
                else
                    incrementArray(thighDegree, -1, 2);
                break;
            case 6:
                if (flyMode)
                {
                    if (direction == 3)
                        exponent = 1;
                    // if (exponent > 50)
                    // {
                    //     exponent = 50;
                    // }
                    // else if (exponent > 50)
                    // {
                    //     exponent--;
                    // }
                    exponent++;

                    direction = -3;
                    flyVertical(-1);
                }
                break;
            default:
                break;
            }
            break;
        case GLFW_KEY_KP_8:
            perspectiveZ += 0.1;
            orthoZ += 0.05;
            break;
        case GLFW_KEY_KP_2:
            perspectiveZ -= 0.1;
            orthoZ -= 0.05;
            break;
        case GLFW_KEY_1:
            if (fingerControl)
                fingerNo = 1;
            else if (armControl)
                armNo = 1;
            else if (legControl)
                legNo = 1;
            else if (animationControl)
            { // walk
                if (walkControl)
                    walkControl = false;
                else
                {
                    walkControl = true;
                    runControl = false;
                    flyMode = false;
                }
            }
            else if (itemControl)
            {
                if (ionBlaster)
                    ionBlaster = false;
                else
                {
                    ionBlaster = true;
                    jetpack = false;
                }
            }
            printf("fingerNo: %d\tarmNo: %d\tlegNo: %d\t\n",
                   fingerNo, armNo, legNo);
            break;
        case GLFW_KEY_2:
            if (fingerControl)
                fingerNo = 2;
            else if (armControl)
                armNo = 2;
            else if (legControl)
                legNo = 2;
            else if (animationControl)
            {
                if (runControl)
                    runControl = false;
                else
                {
                    runControl = true;
                    walkControl = false;
                    flyMode = false;
                }

                if (!runControl)
                {
                    resetRobot();
                }
            }
            else if (itemControl)
            {
                if (fightMode)
                    fightMode = false;
                else if (!fightMode && ionBlaster)
                {
                    walkStationary = false;
                    walkControl = false;
                    runControl = false;
                    flyMode = false;
                    fightMode = true;
                    printf("fightMode: %d\n", fightMode);
                }
                else
                {
                    fightMode = false;
                }

                if (!fightMode)
                {
                    resetRobot();
                }
            }
            printf("fingerNo: %d\tarmNo: %d\tlegNo: %d\t\n",
                   fingerNo, armNo, legNo);
            break;
        case GLFW_KEY_3: // real walk / run
            if (fingerControl)
                fingerNo = 3;
            else if (animationControl)
            {
                if (walkStationary)
                    walkStationary = false;
                else
                    walkStationary = true;
            }
            else if (itemControl)
            {
                if (jetpack)
                {
                    jetpack = false;
                    flyMode = false;
                }
                else
                {
                    ionBlaster = false;
                    jetpack = true;
                }
            }
            break;
        case GLFW_KEY_4:
            if (fingerControl)
                fingerNo = 4;
            else if (itemControl)
            {
                if (flyMode)
                    flyMode = false;
                else if (!flyMode && jetpack)
                {
                    walkStationary = false;
                    walkControl = false;
                    runControl = false;
                    fightMode = false;
                    flyMode = true;
                    mode = 6;
                }
                else
                {
                    walkStationary = false;
                    flyMode = false;
                }

                if (!flyMode)
                {
                    resetRobot();
                }
            }
            break;
        case GLFW_KEY_5:
            if (fingerControl)
                fingerNo = 5;
            else if (itemControl)
            {
                if (sword)
                    sword = false;
                else
                {
                    sword = true;
                    jetpack = false;
                    ionBlaster = false;
                    flyMode = false;
                    fightMode = false;
                }
            }
            break;
        case GLFW_KEY_6:
            if (fingerControl)
                fingerNo = 6;
            break;
        case GLFW_KEY_7:
            if (fingerControl)
                fingerNo = 7;
            break;
        case GLFW_KEY_8:
            if (fingerControl)
                fingerNo = 8;
            break;
        case GLFW_KEY_9:
            if (fingerControl)
                fingerNo = 9;
            break;
        case GLFW_KEY_0: // insult animation
            if (fingerControl)
                fingerNo = 10;
            else if (animationControl)
            {
                if (insultControl)
                    insultControl = false;
                else
                    insultControl = true;
            }
            break;

        case GLFW_KEY_W:
            if (walkControl || runControl || flyMode)
            {
                if (direction != 1)
                {
                    walkingFlag[0] = 0;
                    walkingFlag[1] = 1;
                }
                if (flyMode)
                    jetPackParticleSize = 1.5;
                walkDirectionDegree = 0;
                direction = 1;
            }
            break;
        case GLFW_KEY_S:
            if (walkControl || runControl || flyMode)
            {
                if (direction != -1)
                {
                    walkingFlag[1] = 0;
                    walkingFlag[0] = 1;
                }
                if (flyMode)
                    walkDirectionDegree = 0;
                else
                    walkDirectionDegree = 180;
                direction = -1;
            }
            break;
        case GLFW_KEY_A:
            if (walkControl || runControl || flyMode)
            {
                if (direction != 2)
                {
                    walkingFlag[0] = 0;
                    walkingFlag[1] = 2;
                }
                walkDirectionDegree = -90;
                if (flyMode)
                    jetPackParticleSize = 1.5;
                direction = 2;
            }
            break;
        case GLFW_KEY_D:
            if (walkControl || runControl || flyMode)
            {
                if (direction != -2)
                {
                    walkingFlag[1] = 0;
                    walkingFlag[0] = 1;
                }
                walkDirectionDegree = 90;
                if (flyMode)
                    jetPackParticleSize = 1.5;
                direction = -2;
            }
            break;
        case GLFW_KEY_V: // RPG or first person mode
                         // reserved

            break;
        case GLFW_KEY_Z: // reset robot
            resetRobot();
            break;
        case GLFW_KEY_C:
            if (textureMode == 0)
            {
                textureMode = textureMode + 1;
            }
            else
            {
                textureMode = 0;
            }
            break;
        case GLFW_KEY_Q:
            if (light)
            {
                light = false;
            }else 
            {
                light = true;
            }
            break;
        case GLFW_KEY_T:
            v += adjustSpeed1;
            break;
        case GLFW_KEY_G:
            v -= adjustSpeed1;
            break;
        case GLFW_KEY_Y:
            v1 += adjustSpeed1;
            break;
        case GLFW_KEY_H:
            v1 -= adjustSpeed1;
            break;
        case GLFW_KEY_U:
            v2 += adjustSpeed1;
            break;
        case GLFW_KEY_J:
            v2 -= adjustSpeed1;
            break;
        case GLFW_KEY_I:
            v3 += adjustSpeed2;
            break;
        case GLFW_KEY_K:
            v3 -= adjustSpeed2;
            break;
        case GLFW_KEY_O:
            v4 += adjustSpeed2;
            break;
        case GLFW_KEY_L:
            v4 -= adjustSpeed2;
            break;
        case GLFW_KEY_P:
            v5 += adjustSpeed2;
            break;
        case GLFW_KEY_SEMICOLON:
            v5 -= adjustSpeed2;
            break;
        case GLFW_KEY_LEFT_BRACKET:
            v6 += 0.01;
            break;
        case GLFW_KEY_APOSTROPHE:
            v6 -= 0.01;
            break;
        case GLFW_KEY_RIGHT_BRACKET:
            v7 += 0.01;
            break;
        case GLFW_KEY_BACKSLASH:
            v7 -= 0.01;
            break;
        default:
            break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        switch (key)
        {
        case GLFW_KEY_W:
        case GLFW_KEY_A:
        case GLFW_KEY_D:
        case GLFW_KEY_S:
            if (walkControl || runControl || flyMode)
            {
                direction = 0;
                jetPackParticleSize = 1;
            }
            break;
        case GLFW_KEY_UP:
            if (flyMode)
            {
                if (direction == -3)
                    exponent = 1;
                exponent++;
                direction = 3;
                flyVertical(1);
                jetPackParticleSize = 1;
            }
            break;

        default:
            break;
        }
    }
}

GLFWwindow *initWindow(const int resX, const int resY)
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    // Open a window and create its OpenGL context
    GLFWwindow *window = glfwCreateWindow(resX, resY, "Jaeger", NULL, NULL);

    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, controls);

    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST); // Depth Testing
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    return window;
}

// Primitive shape
void drawCube()
{
    GLfloat vertices[] =
        {
            -1, -1, -1, -1, -1, 1, -1, 1, 1, -1, 1, -1,
            1, -1, -1, 1, -1, 1, 1, 1, 1, 1, 1, -1,
            -1, -1, -1, -1, -1, 1, 1, -1, 1, 1, -1, -1,
            -1, 1, -1, -1, 1, 1, 1, 1, 1, 1, 1, -1,
            -1, -1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1,
            -1, -1, 1, -1, 1, 1, 1, 1, 1, 1, -1, 1};

    GLfloat colors[] =
        {
            0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0,
            1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0,
            0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0,
            0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1};

    static float alpha = 0;
    //attempt to rotate cube
    glRotatef(alpha, 0, 1, 0);

    /* We have a color array and a vertex array */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, colors);

    /* Send data : 24 vertices */
    glDrawArrays(GL_QUADS, 0, 24);

    /* Cleanup states */
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    alpha += 1;
}

void drawCirle(float x1, float y1, float radius)
{
    float x2 = x1, y2 = y1;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x1, y1);
    for (float angle = 0; angle <= 360; angle += 10)
    {
        x2 = x1 + cos(angle) * radius;
        y2 = y1 + sin(angle) * radius;
        glVertex2f(x2, y2);
    }
    glEnd();
}

void drawCylinder(double baseRadius, double topRadius, double height, int slices, int stacks)
{
    GLUquadricObj *cylinder = NULL;
    cylinder = gluNewQuadric();
    gluQuadricTexture(cylinder, GLU_TRUE);
    gluQuadricDrawStyle(cylinder, selectedGluDrawStyles);
    gluCylinder(cylinder, baseRadius, topRadius, height, slices, stacks);
    gluDeleteQuadric(cylinder);
}

void drawSphere(float radius, int slice, int slack)
{
    GLUquadricObj *sphere = NULL;
    sphere = gluNewQuadric();
    gluQuadricTexture(sphere, GLU_TRUE);
    gluQuadricDrawStyle(sphere, selectedGluDrawStyles);
    gluSphere(sphere, radius, slice, slack);
    gluDeleteQuadric(sphere);
}

void drawCone()
{
    glPushMatrix();
    GLUquadricObj *cylinder = NULL;
    cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, selectedGluDrawStyles);
    gluCylinder(cylinder, 0.01, 0.3, 1, 20, 20);
    gluDeleteQuadric(cylinder);
    glPopMatrix();

    // glPushMatrix();
    // cylinder = gluNewQuadric();
    // glColor3ub(98, 66, 40);
    // gluQuadricDrawStyle(cylinder, GLU_LINE);
    // gluCylinder(cylinder, 0.011, 0.31, 1, 5, 5);
    // gluDeleteQuadric(cylinder);
    // glPopMatrix();
}

void drawCuboid(float size, float widthScale)
{
    glBegin(selectedGlDrawStyles);
    // front
    glTexCoord2f(0.0f, 1);
    glVertex3f(0, 0, size);
    glTexCoord2f(1, 1);
    glVertex3f(size * widthScale, 0, size);
    glTexCoord2f(1, 0.0f);
    glVertex3f(size * widthScale, 0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, 0, 0);

    // left
    // glColor3ub(223, 120, 239);
    glTexCoord2f(0.0f, 1);

    glVertex3f(0, size, size);
    glTexCoord2f(1, 1);
    glVertex3f(0, 0, size);
    glTexCoord2f(1, 0.0f);
    glVertex3f(0, 0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, size, 0);

    // bottom
    // glColor3ub(128, 226, 126);
    glTexCoord2f(0.0f, 1);
    glVertex3f(0, size, 0);
    glTexCoord2f(1, 1);
    glVertex3f(size * widthScale, size, 0);
    glTexCoord2f(1, 0.0f);
    glVertex3f(size * widthScale, 0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, 0, 0);

    // right
    // glColor3ub(255, 255, 114);
    glTexCoord2f(0.0f, 1);
    glVertex3f(size * widthScale, 0, size);
    glTexCoord2f(1, 1);
    glVertex3f(size * widthScale, size, size);
    glTexCoord2f(1, 0.0f);
    glVertex3f(size * widthScale, size, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(size * widthScale, 0, 0);

    // behind
    // glColor3ub(255, 201, 71);
    glTexCoord2f(0.0f, 1);
    glVertex3f(size * widthScale, size, size);
    glTexCoord2f(1, 1);
    glVertex3f(0, size, size);
    glTexCoord2f(1, 0.0f);
    glVertex3f(0, size, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(size * widthScale, size, 0);

    // top
    glTexCoord2f(0.0f, 1);
    glVertex3f(0, size, size);
    glTexCoord2f(1, 1);
    glVertex3f(size * widthScale, size, size);
    glTexCoord2f(1, 0.0f);
    glVertex3f(size * widthScale, 0, size);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0, 0, size);

    glEnd();
}

void drawPyramid()
{
    glPushMatrix();
    glTranslatef(0, -0.5f, 0);
    //glRotatef(rotateDeg, 1, 1, 1);
    glBegin(selectedGlewDrawStyles);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, -0.5);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(-0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(0.5, 0, 0);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, -0.5);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(-0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(-0.5, 0, -1.0);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, -0.5);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(0.5, 0, -1.0);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, -0.5);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(0.5, 0, -1.0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(-0.5, 0, -1.0);
    }
    glEnd();

    glBegin(selectedGlewDrawStyles);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(-0.5, 0, -1);

        glTexCoord2f(1, 1);
        glVertex3f(-0.5, 0, 0);

        glTexCoord2f(1, 0.0f);
        glVertex3f(0.5, 0, 0);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.5, 0, -1);
    }
    glEnd();

    glPopMatrix();
}

void drawRightPyramid(float length)
{
    glPushMatrix();
    glTranslatef(0, -0.5f, 0);
    //glRotatef(rotateDeg, 1, 1, 1);
    glBegin(selectedGlewDrawStyles);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, 0);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(-0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(0.5, 0, 0);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, 0);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(-0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(-0.5, 0, length);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, 0);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(0.5, 0, length);

        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, 0);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(0.5, 0, length);
        glTexCoord2f(0.5f, 0);
        glVertex3f(-0.5, 0, length);
    }
    glEnd();

    glBegin(selectedGlewDrawStyles);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(-0.5, 0, length);

        glTexCoord2f(1, 1);
        glVertex3f(-0.5, 0, 0);

        glTexCoord2f(1, 0.0f);
        glVertex3f(0.5, 0, 0);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.5, 0, length);
    }
    glEnd();

    glPopMatrix();
}

void drawTrianglePyramid(float length, float rightTopHeight)
{
    glDisable(GL_SMOOTH);
    glPushMatrix();
    glTranslatef(0, -0.5f, 0);
    // back and front
    glBegin(selectedGlewDrawStyles);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(0, 1, 0);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(-0.5, 0, 0);
        glTexCoord2f(0.5f, 0);
        glVertex3f(0.5, 0, 0);
    }
    glEnd();

    glBegin(selectedGlewDrawStyles);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(0, rightTopHeight, length);
        glTexCoord2f(-0.5f, 0);
        glVertex3f(0.5, 0, length);
        glTexCoord2f(0.5f, 0);
        glVertex3f(-0.5, 0, length);
    }
    glEnd();

    //  two sides
    glBegin(selectedGlewDrawStyles);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(0, rightTopHeight, length);
        glTexCoord2f(1, 1);
        glVertex3f(0, 1, 0);
        glTexCoord2f(1, 0.0f);
        glVertex3f(0.5, 0, 0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.5, 0, length);
    }
    glEnd();

    glBegin(selectedGlewDrawStyles);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(0, rightTopHeight, length);
        glTexCoord2f(1, 1);
        glVertex3f(0, 1, 0);
        glTexCoord2f(1, 0.0f);
        glVertex3f(-0.5, 0, 0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-0.5, 0, length);
    }
    glEnd();

    // base
    glBegin(selectedGlewDrawStyles);
    {
        glTexCoord2f(0.0f, 1);
        glVertex3f(-0.5, 0, length);
        glTexCoord2f(1, 1);
        glVertex3f(-0.5, 0, 0);
        glTexCoord2f(1, 0.0f);
        glVertex3f(0.5, 0, 0);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.5, 0, length);
    }
    glEnd();

    glPopMatrix();
}

void drawCapsule(float width, float height)
{
    glPushMatrix();
    {
        // one cylinder and two sphere
        glPushMatrix();
        {
            // glTranslatef(0.25, 1.88 + calfHeight * 1.45, 0.04);
            glScalef(1, 1, 1);
            drawCylinder(height, height, width, 20, 10);
        }
        glPopMatrix();

        glPushMatrix();
        {
            int jointSliceStack = 10;
            float jointRadius = height;
            // glTranslatef(0.25, 1.88 + calfHeight * 1.45, 0.33);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
            glTranslatef(0, 0, width);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        }
        glPopMatrix();
    }
    glPopMatrix();
}

// animation

typedef struct
{
    double Xpos;
    double Ypos;
    double Zpos;
    double Xmov;
    double Zmov;
    double Red;
    double Green;
    double Blue;
    double Direction;
    double Acceleration;
    double Deceleration;
    double Scalez;
    bool Visible;
} PARTICLES;

PARTICLES Particle[ParticleCount];

PARTICLES JetPackParticle1[JetPackParticleCount];
PARTICLES JetPackParticle2[JetPackParticleCount];

void glCreateParticles(void)
{
    int i;
    for (i = 1; i < ParticleCount; i++)
    {
        Particle[i].Xpos = 0;
        Particle[i].Ypos = -5;
        Particle[i].Zpos = -5;
        Particle[i].Xmov = (((((((2 - 1 + 1) * rand() % 11) + 1) - 1 + 1) *
                              rand() % 11) +
                             1) *
                            0.005) -
                           (((((((2 - 1 + 1) * rand() % 11) + 1) - 1 + 1) * rand() % 11) + 1) * 0.005);
        Particle[i].Zmov = (((((((2 - 1 + 1) * rand() % 11) + 1) - 1 + 1) *
                              rand() % 11) +
                             1) *
                            0.005) -
                           (((((((2 - 1 + 1) * rand() % 11) + 1) - 1 + 1) * rand() % 11) + 1) * 0.005);
        Particle[i].Red = 1;
        Particle[i].Green = 1;
        Particle[i].Blue = 1;
        Particle[i].Scalez = 0.1;
        Particle[i].Direction = 0;
        Particle[i].Acceleration = ((((((8 - 5 + 2) * rand() % 11) + 5) - 1 + 1) * rand() % 11) + 1) * 0.02;
        Particle[i].Deceleration = 0.0025;
    }
}

void glUpdateParticles(void)
{
    int i;
    for (i = 1; i < ParticleCount; i++)
    {
        Particle[i].Ypos = Particle[i].Ypos + Particle[i].Acceleration - Particle[i].Deceleration;
        Particle[i].Deceleration = Particle[i].Deceleration +
                                   0.0025;
        Particle[i].Xpos = Particle[i].Xpos + Particle[i].Xmov;
        Particle[i].Zpos = Particle[i].Zpos + Particle[i].Zmov;
        Particle[i].Direction = Particle[i].Direction + ((((((int)(0.5 - 0.1 + 0.1) * rand() % 11) + 1) - 1 + 1) * rand() % 11) + 1);

        if (spawn)
            if (Particle[i].Ypos < -5)
            {
                Particle[i].Xpos = 0;
                Particle[i].Ypos = -5;
                Particle[i].Zpos = -5;
                Particle[i].Red = 1;
                Particle[i].Green = 0;
                Particle[i].Blue = 0;
                Particle[i].Direction = 0;
                Particle[i].Acceleration = ((((((8 - 5 + 2) * rand() % 11) + 5) - 1 + 1) * rand() % 11) + 1) * 0.02;
                Particle[i].Deceleration = 0.0025;
            }
    }
}

void glDrawParticles(void)
{
    int i;
    for (i = 1; i < ParticleCount; i++)
    {
        glPushMatrix();
        glTranslatef(Particle[i].Xpos, Particle[i].Ypos, Particle[i].Zpos);
        glRotatef(Particle[i].Direction - 90, 0, 0, 1);
        glScalef(Particle[i].Scalez, Particle[i].Scalez, Particle[i].Scalez);

        int jointSliceStack = 3;
        float jointRadius = 0.8;
        if (i % 2 == 0)
            initTexture("gold_texture.bmp");
        else if (i % 3 == 0)
            initTexture("blue_metal_texture.bmp");
        else
            initTexture("galaxy_texture.bmp");

        drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        glPopMatrix();
    }
}

// jetpack effect

void glCreateJetPackParticles1(void)
{
    int i;
    for (i = 1; i < JetPackParticleCount; i++)
    {
        JetPackParticle1[i].Xpos = 0;
        JetPackParticle1[i].Ypos = -5;
        JetPackParticle1[i].Zpos = -5;
        JetPackParticle1[i].Xmov = (((((((2 - 1 + 1) * rand() % 11) + 1) - 1 + 1) *
                                      rand() % 11) +
                                     1) *
                                    0.005) -
                                   (((((((2 - 1 + 1) * rand() % 11) + 1) - 1 + 1) * rand() % 11) + 1) * 0.005);
        JetPackParticle1[i].Zmov = (((((((2 - 1 + 1) * rand() % 11) + 1) - 1 + 1) *
                                      rand() % 11) +
                                     1) *
                                    0.005) -
                                   (((((((2 - 1 + 1) * rand() % 11) + 1) - 1 + 1) * rand() % 11) + 1) * 0.005);
        JetPackParticle1[i].Red = 1;
        JetPackParticle1[i].Green = 1;
        JetPackParticle1[i].Blue = 1;
        JetPackParticle1[i].Scalez = 0.1;
        JetPackParticle1[i].Direction = 0;
        JetPackParticle1[i].Acceleration = ((((((8 - 5 + 2) * rand() % 11) + 5) - 1 + 1) * rand() % 11) + 1) * 0.02;
        JetPackParticle1[i].Deceleration = 0.0025;
    }
}

void glUpdateJetPackParticles1(void)
{
    int i;
    for (i = 1; i < JetPackParticleCount; i++)
    {
        JetPackParticle1[i].Ypos = JetPackParticle1[i].Ypos + JetPackParticle1[i].Acceleration - JetPackParticle1[i].Deceleration - 0.30;
        JetPackParticle1[i].Deceleration = JetPackParticle1[i].Deceleration + 0.0025;
        JetPackParticle1[i].Xpos = JetPackParticle1[i].Xpos + JetPackParticle1[i].Xmov;
        JetPackParticle1[i].Zpos = JetPackParticle1[i].Zpos + JetPackParticle1[i].Zmov;
        JetPackParticle1[i].Direction = JetPackParticle1[i].Direction + ((((((int)(0.5 - 0.1 + 0.1) * rand() % 11) + 1) - 1 + 1) * rand() % 11) + 1);

        if (JetPackParticle1[i].Ypos < -5)
        {
            JetPackParticle1[i].Xpos = 0;
            JetPackParticle1[i].Ypos = 4.0;
            JetPackParticle1[i].Zpos = -5;
            JetPackParticle1[i].Red = 1;
            JetPackParticle1[i].Green = 0;
            JetPackParticle1[i].Blue = 0;
            JetPackParticle1[i].Direction = 0;
            JetPackParticle1[i].Acceleration = ((((((8 - 5 + 2) * rand() % 11) + 5) - 1 + 1) * rand() % 11) + 1) * 0.02;
            JetPackParticle1[i].Deceleration = 0.0025;
        }
    }
}

void glDrawJetPackParticles1(void)
{
    int i;
    for (i = 1; i < JetPackParticleCount; i++)
    {
        glPushMatrix();
        glTranslatef(JetPackParticle1[i].Xpos, JetPackParticle1[i].Ypos, JetPackParticle1[i].Zpos);
        glRotatef(JetPackParticle1[i].Direction - 90, 0, 0, 1);
        glScalef(JetPackParticle1[i].Scalez, JetPackParticle1[i].Scalez, JetPackParticle1[i].Scalez);

        int jointSliceStack = 3;
        float jointRadius = 0.8;
        initTexture("fire.bmp");
        glScalef(jetPackParticleSize, jetPackParticleSize, jetPackParticleSize);
        drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        glPopMatrix();
    }
}

void glCreateJetPackParticles2(void)
{
    int i;
    for (i = 1; i < JetPackParticleCount; i++)
    {
        JetPackParticle2[i].Xpos = 0;
        JetPackParticle2[i].Ypos = -5;
        JetPackParticle2[i].Zpos = -5;
        JetPackParticle2[i].Xmov = (((((((2 - 1 + 1) * rand() % 11) + 1) - 1 + 1) *
                                      rand() % 11) +
                                     1) *
                                    0.005) -
                                   (((((((2 - 1 + 1) * rand() % 11) + 1) - 1 + 1) * rand() % 11) + 1) * 0.005);
        JetPackParticle2[i].Zmov = (((((((2 - 1 + 1) * rand() % 11) + 1) - 1 + 1) *
                                      rand() % 11) +
                                     1) *
                                    0.005) -
                                   (((((((2 - 1 + 1) * rand() % 11) + 1) - 1 + 1) * rand() % 11) + 1) * 0.005);
        JetPackParticle2[i].Red = 1;
        JetPackParticle2[i].Green = 1;
        JetPackParticle2[i].Blue = 1;
        JetPackParticle2[i].Scalez = 0.1;
        JetPackParticle2[i].Direction = 0;
        JetPackParticle2[i].Acceleration = ((((((8 - 5 + 2) * rand() % 11) + 5) - 1 + 1) * rand() % 11) + 1) * 0.02;
        JetPackParticle2[i].Deceleration = 0.0025;
    }
}

void glUpdateJetPackParticles2(void)
{
    int i;
    for (i = 1; i < JetPackParticleCount; i++)
    {
        JetPackParticle2[i].Ypos = JetPackParticle2[i].Ypos + JetPackParticle2[i].Acceleration - JetPackParticle2[i].Deceleration - 0.30;
        JetPackParticle2[i].Deceleration = JetPackParticle2[i].Deceleration + 0.0025;
        JetPackParticle2[i].Xpos = JetPackParticle2[i].Xpos + JetPackParticle2[i].Xmov;
        JetPackParticle2[i].Zpos = JetPackParticle2[i].Zpos + JetPackParticle2[i].Zmov;
        JetPackParticle2[i].Direction = JetPackParticle2[i].Direction + ((((((int)(0.5 - 0.1 + 0.1) * rand() % 11) + 1) - 1 + 1) * rand() % 11) + 1);

        if (JetPackParticle2[i].Ypos < -5)
        {
            JetPackParticle2[i].Xpos = 0;
            JetPackParticle2[i].Ypos = 4.0;
            JetPackParticle2[i].Zpos = -5;
            JetPackParticle2[i].Red = 1;
            JetPackParticle2[i].Green = 0;
            JetPackParticle2[i].Blue = 0;
            JetPackParticle2[i].Direction = 0;
            JetPackParticle2[i].Acceleration = ((((((8 - 5 + 2) * rand() % 11) + 5) - 1 + 1) * rand() % 11) + 1) * 0.02;
            JetPackParticle2[i].Deceleration = 0.0025;
        }
    }
}

void glDrawJetPackParticles2(void)
{
    int i;
    for (i = 1; i < JetPackParticleCount; i++)
    {
        glPushMatrix();
        glTranslatef(JetPackParticle2[i].Xpos, JetPackParticle2[i].Ypos, JetPackParticle2[i].Zpos);
        glRotatef(JetPackParticle2[i].Direction - 90, 0, 0, 1);
        glScalef(JetPackParticle2[i].Scalez, JetPackParticle2[i].Scalez, JetPackParticle2[i].Scalez);

        int jointSliceStack = 3;
        float jointRadius = 0.8;
        initTexture("fire.bmp");
        glScalef(jetPackParticleSize, jetPackParticleSize, jetPackParticleSize);
        drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        glPopMatrix();
    }
}

void resetRobot()
{
    fill(fingerDegree, fingerDegree + 10, 0);
    fill(handDegree, handDegree + 2, 0);
    fill(armDegreeX, armDegreeX + 2, 0);
    fill(armDegreeY, armDegreeY + 2, 0);
    fill(calfDegree, calfDegree + 2, 0);
    fill(thighDegree, thighDegree + 2, 0);
    animationX = 0;
    animationY = -0.71;
    animationZ = 0;
    ionBlasterX = 0;
    ionBlasterY = 0;
    ionBlasterZ = 0;
    ionBlasterXDegree = 0;
    ionBlasterYDegree = 0;
    ionBlasterZDegree = 0;
    flyForwardDegree = 0;
    walkDirectionDegree = 0;
    fill(fightModeFlag, fightModeFlag + 2, 0);
    fill(walkingFlag, walkingFlag + 2, 0);
    perspectiveX = 0;
    perspectiveY = -3.3;
    perspectiveZ = -16;
    orthoX = 0;
    orthoY = 0.5;
    orthoZ = 0;
}

void shoot()
{
    if (shootFlag)
    {
        // shoot ion energy
        glPushMatrix();
        {
            // long capsule of energy
            glTranslatef(0, 0, -1.5 + ionEnergyZ);
            drawCapsule(2.82, 0.09);

            if (ionEnergyZ < 50)
            {
                ionEnergyZ += 2;
            }
            if (ionEnergyZ >= 50)
            {
                shootFlag = false;
                ionEnergyZ = 0;
            }
        }
        glPopMatrix();
    }
}

void walk(int speed)
{
    float legSpeed = 1 * speed;
    float armSpeed = 0.2 * speed;
    float armMoveDegree = 15;
    if (speed > 1 || speed < -1)
    {
        // run
        armMoveDegree = 25;
    }
    if (walkingFlag[1])
    {
        // leg movement
        if (thighDegree[0] < 46)
        {
            thighDegree[0] += legSpeed;
        }
        if (calfDegree[0] > -36)
        {
            calfDegree[0] -= legSpeed;
        }
        if (thighDegree[1] > -36)
        {
            thighDegree[1] -= legSpeed;
        }
        if (calfDegree[1] < 0)
        {
            calfDegree[1] += legSpeed;
        }

        // hand movement
        if (!insultControl && !fightMode)
        {
            if (armDegreeX[1] < armMoveDegree)
            {
                armDegreeX[1] += armSpeed;
            }
        }

        if (armDegreeX[0] > -armMoveDegree)
        {
            armDegreeX[0] -= armSpeed;
        }
        if (thighDegree[0] > 45)
        {
            walkingFlag[0] = 1;
            walkingFlag[1] = 0;
        }
    }
    if (walkingFlag[0])
    {
        // leg movement
        if (thighDegree[0] > -36)
        {
            thighDegree[0] -= legSpeed;
        }
        if (calfDegree[0] < 0)
        {
            calfDegree[0] += legSpeed;
        }
        if (thighDegree[1] < 46)
        {
            thighDegree[1] += legSpeed;
        }
        if (calfDegree[1] > -36)
        {
            calfDegree[1] -= legSpeed;
        }

        // hand movement
        if (!insultControl && !fightMode)
        {
            if (armDegreeX[1] > -armMoveDegree)
            {
                armDegreeX[1] -= armSpeed;
            }
        }
        if (armDegreeX[0] < armMoveDegree)
        {
            armDegreeX[0] += armSpeed;
        }

        if (thighDegree[1] > 45)
        {
            walkingFlag[0] = 0;
            walkingFlag[1] = 1;
        }
    }

    //  walk with move X
    if (!walkStationary)
    {
        if (direction == 1)
        {
            if (walkControl)
                animationX += 0.1;
            else if (runControl)
                animationX += 0.2;
        }
        else if (direction == -1)
        {
            if (walkControl)
                animationX -= 0.1;
            else if (runControl)
                animationX -= 0.2;
        }
        if (direction == 2)
        {
            if (walkControl)
                animationZ += 0.1;
            else if (runControl)
                animationZ += 0.2;
        }
        else if (direction == -2)
        {
            if (walkControl)
                animationZ -= 0.1;
            else if (runControl)
                animationZ -= 0.2;
        }
    }
}

void insult()
{
    float armSpeed = 1;
    if (armDegreeX[1] < 80)
    {
        armDegreeX[1] += armSpeed;
    }
    if (handDegree[1] < 80)
    {
        handDegree[1] += armSpeed;
    }
    if (fingerDegree[5] < 100)
    {
        fingerDegree[5] += armSpeed;
    }
    if (fingerDegree[6] < 100)
    {
        fingerDegree[6] += armSpeed;
    }
    if (fingerDegree[8] < 100)
    {
        fingerDegree[8] += armSpeed;
    }
    if (fingerDegree[9] < 100)
    {
        fingerDegree[9] += armSpeed;
    }
}

void flyVertical(int direction)
{
    // ascend speed increase exponentially
    ascendSpeed = pow(1.1, exponent) / 100;

    if (ascendSpeed > 1.0)
    {
        ascendSpeed = 1.0;
    }

    if (direction == 1)
    {
        animationY += ascendSpeed;
    }
    else if (direction == -1)
    {
        animationY -= ascendSpeed;
    }

    // move up and down by 0.5
    if (swingDistance < 0.5 && !swingFlag)
        animationY += 0.001;
    else if (swingDistance > 0 && swingFlag)
        animationY -= 0.001;

    if (swingDistance == 0.5)
        swingFlag = true;
    else if (swingDistance == 0)
        swingFlag = false;
}

void flyHorizontal()
{
    if (direction == 1)
    {
        animationX += 0.1;
        animationY += 0.1;

        if (flyForwardDegree > -45)
        {
            flyForwardDegree -= 1;
        }

        if (armDegreeX[1] < 160)
        {
            armDegreeX[1] += 5;
        }
        if (armDegreeX[0] < 160)
        {
            armDegreeX[0] += 5;
        }
    }
    else if (direction == -1)
    {
        {
            animationX -= 0.1;
            animationY -= 0.1;
        }
        if (flyForwardDegree < 0)
        {
            flyForwardDegree += 1;
        }
        if (armDegreeX[1] > 0)
        {
            armDegreeX[1] -= 5;
        }
        if (armDegreeX[0] > 0)
        {
            armDegreeX[0] -= 5;
        }
    }
    else if (direction == 2)
    {
        animationZ += 0.1;
        animationY += 0.1;

        if (flyForwardDegree > -45)
        {
            flyForwardDegree -= 1;
        }

        if (armDegreeX[1] < 160)
        {
            armDegreeX[1] += 5;
        }
        if (armDegreeX[0] < 160)
        {
            armDegreeX[0] += 5;
        }
    }
    else if (direction == -2)
    {
        animationZ -= 0.1;
        animationY += 0.1;

        if (flyForwardDegree > -45)
        {
            flyForwardDegree -= 1;
        }

        if (armDegreeX[1] < 160)
        {
            armDegreeX[1] += 5;
        }
        if (armDegreeX[0] < 160)
        {
            armDegreeX[0] += 5;
        }
    }
}

void activateFightMode()
{
    // makes gun go to hand
    // first the gun fly up and rotate
    // resetRobot();
    // printf("ionBlasterXDegree: %f, ionBlasterYDegree: %f\n", ionBlasterXDegree, ionBlasterYDegree);

    // step 1

    if (!fightModeFlag[0])
    {
        if (ionBlasterX < 6.9)
        {
            ionBlasterX += 0.2;
        }
        if (ionBlasterY < 4.7)
        {
            ionBlasterY += 0.2;
        }
        if (ionBlasterYDegree < 90)
        {
            ionBlasterYDegree += 2;
        }
        if (ionBlasterXDegree > -45)
        {
            ionBlasterXDegree -= 2;
        }

        if (ionBlasterX >= 6.9 && ionBlasterYDegree >= 90)
        {
            fightModeFlag[0] = 1;
        }
    }

    // if(fightModeFlag[0]){
    // ionBlasterX = ionBlasterX + v;
    // ionBlasterY = ionBlasterY + v1;
    // ionBlasterZ = ionBlasterZ + v2;
    // }

    else if (!fightModeFlag[1])
    {
        // -0.200000    v1: -1.800000   v2: -2.7

        // if (ionBlasterX > 7)
        // {
        //     ionBlasterX -= 0.1;
        // }
        if (ionBlasterY > 3)
        {
            ionBlasterY -= 0.1;
        }
        if (ionBlasterZ > -2.6)
        {
            ionBlasterZ -= 0.1;
        }

        float armSpeed = 2;
        if (armDegreeX[1] < 90)
        {
            armDegreeX[1] += armSpeed;
        }
        if (armDegreeY[1] < 90)
        {
            armDegreeY[1] += armSpeed;
        }

        if (fingerDegree[5] > -90)
        {
            fingerDegree[5] -= armSpeed;
        }
        if (fingerDegree[6] > -90)
        {
            fingerDegree[6] -= armSpeed;
        }
        if (fingerDegree[7] > -90)
        {
            fingerDegree[7] -= armSpeed;
        }
        if (fingerDegree[8] > -90)
        {
            fingerDegree[8] -= armSpeed;
        }
        if (fingerDegree[9] > -90)
        {
            fingerDegree[9] -= armSpeed;
        }

        if (ionBlasterY <= 3 && ionBlasterZ <= -2.6 && fingerDegree[5] <= -90)
        {
            fightModeFlag[1] = 1;
        }
    }
}

// model item
void drawMoon()
{
    float x = 0, y = 0, z = -15, GL_PI = 3.142, radius = 0.4;
    glPushMatrix();
    {
        // Call only once for all remaining points
        glBegin(selectedGlDrawStyles);

        // All lines lie in the xy plane.
        for (float angle = 0.0f; angle <= GL_PI; angle += (GL_PI / 50.0f))
        {
            // Top half of the circle
            x = radius * sin(angle);
            y = radius * cos(angle);
            glVertex3f(x + 1, y + 2, z); // First endpoint of line

            // Bottom half of the circle
            x = radius * sin(angle + GL_PI);
            y = radius * cos(angle + GL_PI);
            glVertex3f(x + 1, y + 2, z); // Second endpoint of line
        }
        // Done drawing points
        glEnd();
    }
    glPopMatrix();
}

void drawCloud(float size)
{
    drawCirle(0, 0, size);
    drawCirle(0.2, 0, size);

    drawCirle(0.35, 0.1, size);

    drawCirle(0.2, 0.2, size);
    drawCirle(0, 0.2, size);

    drawCirle(-0.15, 0.1, size);
}

void drawClouds()
{
    glPushMatrix();
    {
        glTranslatef(-5, 1.5, -15);
        drawCloud(0.15);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-7, 1.2, -15);
        drawCloud(0.15);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-2, 1.5, -15);
        drawCloud(0.15);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(4, 1.2, -15);
        drawCloud(0.15);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(8, 1.5, -15);
        drawCloud(0.15);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(13, 1.2, -15);
        drawCloud(0.15);
    }
    glPopMatrix();
}

void drawBracers()
{
    // hand
    glPushMatrix();
    {
        if (textureMode == 0)
        {
            initTexture("blue_texture.bmp");
        }
        else
        {
            initTexture("galvanized_blue.bmp");
        }
        glRotatef(180, 0, 1, 1);
        glTranslatef(0, 0, -0.45);
        drawCylinder(0.1, 0.2, 0.45, 20, 10);
    }
    removeTexture();
    glPopMatrix();

    glPushMatrix();
    {
        if (textureMode == 0)
        {
            initTexture("white_texture.bmp");
        }
        else
        {
            initTexture("galvanized.bmp");
        }
        glTranslatef(0.1, -0.43, 0);
        glRotatef(180, 0, 1, 1);
        glRotatef(90, 0, 1, 0);
        drawCylinder(0.05, 0.05, 0.21, 20, 10);
    }
    removeTexture();
    glPopMatrix();

    glPushMatrix();
    {
        int jointSliceStack = 10;
        float jointRadius = 0.22;
        if (textureMode == 0)
        {
            initTexture("yellow_texture.bmp");
        }
        else
        {
            initTexture("galvanized_blue.bmp");
        }

        glTranslatef(0, 1.04, 0);
        drawSphere(jointRadius, jointSliceStack, jointSliceStack);
    }
    removeTexture();
    glPopMatrix();
}

void drawFinger(float fingerSize, float fingerWidthScale, int fingerNo)
{
    // float fingerSize = 0.3;
    // float fingerWidthScale = 2.5;

    int jointSliceStack = 10;
    float jointRadius = 0.15;

    float fingerGap = fingerSize * fingerWidthScale + jointRadius;
    float jointGap = fingerGap - (jointRadius * 0.5);

    glScalef(0.7, 0.5, 0.5);
    glPushMatrix();
    {
        if (textureMode == 0)
        {
            initTexture("red2_texture.bmp");
        }
        else
        {
            initTexture("grey2_texture.bmp");
        }
        drawCuboid(fingerSize, fingerWidthScale);
    }
    removeTexture();
    glPopMatrix();
    glPushMatrix();
    {
        initTexture("white_texture.bmp");
        glTranslatef(jointGap, fingerSize / 2, fingerSize / 2);
        drawSphere(jointRadius, jointSliceStack, jointSliceStack);
    }
    removeTexture();
    glPopMatrix();
    glPushMatrix();
    {
        if (textureMode == 0)
        {
            initTexture("red2_texture.bmp");
        }
        else
        {
            initTexture("grey2_texture.bmp");
        }
        glTranslatef(fingerGap, 0, 0);
        glRotatef(0 + fingerDegree[fingerNo], 0, 0, 1);
        drawCuboid(fingerSize, fingerWidthScale);
    }
    removeTexture();
    glPopMatrix();
}

void drawFingers()
{
    // // longness of finger

    // float indexRingFinger = 0;
    float middleFinger = 0.01;
    // float littleFinger = -0.02;
    // float thumb = -0.02;
    // float secondY = -0.04, secondZ = 0.2;

    glRotatef(-90, 0, 0, 1);
    glRotatef(-90, 1, 0, 0);

    glPushMatrix();
    {
        glTranslatef(0, 0, -0.2);
        drawFinger(0.3, 2, fingerCounter++);
    }
    glPopMatrix();

    glPushMatrix();
    {
        drawFinger(0.3, 2.5, fingerCounter++);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(0, 0, 0.2);
        drawFinger(0.3, 3, fingerCounter++);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(0, 0, 0.4);
        drawFinger(0.3, 2.5, fingerCounter++);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(0, 0, 0.6);
        drawFinger(0.3, 2, fingerCounter++);
    }
    glPopMatrix();
}

void drawArm(int armNo)
{
    glPushMatrix();
    {
        // rotate horizontally
        glRotatef(0 + armDegreeY[armNo], 0, 1, 0);

        // draw arm
        glPushMatrix();
        {
            if (textureMode == 0)
            {
                initTexture("grey_texture.bmp");
            }
            else
            {
                initTexture("galvanized.bmp");
            }

            glRotatef(180, 0, 1, 1);
            drawCylinder(0.15, 0.22, 1, 20, 10);
        }
        removeTexture();
        glPopMatrix();

        glPushMatrix();
        {
            drawBracers();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0, -0.44, 0);
            glRotatef(0 + handDegree[handCounter++], 1, 0, 0);
            glTranslatef(0, 0.49, 0);
            glPushMatrix();
            {
                if (textureMode == 0)
                {
                    initTexture("red2_texture.bmp");
                }
                else
                {
                    initTexture("dark_texture.bmp");
                }

                glRotatef(90, 0, 1, 0);
                glTranslatef(-0.04, -0.78, -0.18);
                drawCuboid(0.35, 0.21);
            }
            removeTexture();
            glPopMatrix();

            glPushMatrix();
            {
                glTranslatef(-0.1, -0.71, 0.03);
                glScalef(0.37, 0.37, 0.37);
                drawFingers();
            }
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void drawCalf(int thighNo)
{
    glTranslatef(0, 2.04, 0);
    glRotatef(0 + thighDegree[thighNo], 0, 0, 1);
    glTranslatef(0, -2.04, 0); //-1.84
    glPushMatrix();
    {
        glTranslatef(0.16, 0.2, 0);
        glRotatef(0 + calfDegree[thighNo], 0, 0, 1);
        glTranslatef(-0.31, -2.62, 0); //-1.84
        // TODO: limit calfHeight to reasonable value
        float calfHeight = robotHeight;
        glPushMatrix();
        {
            if (textureMode == 0)
            {
                initTexture("blue_texture.bmp");
            }
            else
            {
                initTexture("galvanized_blue.bmp");
            }
            glTranslatef(0.1, 0.45, 0);
            glScalef(0.5, 2.42 + calfHeight * 3, 0.8);
            drawCuboid(0.5, 1.51);
        }
        removeTexture();
        glPopMatrix();

        glPushMatrix();
        {
            float cylinderHeight = 1.5;
            if (textureMode == 0)
            {
                initTexture("yellow_texture.bmp");
            }
            else
            {
                initTexture("galvanized.bmp");
            }

            glRotatef(180, 0, 1, 1);
            glTranslatef(-0.06, 0, 0.16);
            glScalef(1, 1, 1 + calfHeight);
            drawCylinder(0.05, 0.05, cylinderHeight, 20, 10);
            glTranslatef(0, 0.19, 0);
            drawCylinder(0.05, 0.05, cylinderHeight, 20, 10);
            glTranslatef(0, 0.19, 0);
            drawCylinder(0.05, 0.05, cylinderHeight, 20, 10);
        }
        removeTexture();
        glPopMatrix();

        ///////////////////////
        // draw calf joint
        // one cylinder and two sphere
        glPushMatrix();
        {
            float cylinderHeight = 0.31;
            if (textureMode == 0)
            {
                initTexture("yellow_texture.bmp");
            }
            else
            {
                initTexture("galvanized.bmp");
            }

            glTranslatef(0.25, 1.88 + calfHeight * 1.45, 0.04);
            glScalef(1, 1, 1);
            drawCylinder(0.16, 0.16, cylinderHeight, 20, 10);
        }
        removeTexture();
        glPopMatrix();

        glPushMatrix();
        {
            int jointSliceStack = 10;
            float jointRadius = 0.16;
            if (textureMode == 0)
            {
                initTexture("blue_texture.bmp");
            }
            else
            {
                initTexture("galvanized_blue.bmp");
            }
            glTranslatef(0.25, 1.88 + calfHeight * 1.45, 0.33);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
            glTranslatef(0, 0, -0.27);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        }
        removeTexture();
        glPopMatrix();

        glPushMatrix();
        {
            // triangle cover the cylinder joint
            float pyramidSize = 0.1;
            if (textureMode == 0)
            {
                initTexture("red2_texture.bmp");
            }
            else
            {
                initTexture("grey_texture.bmp");
            }
            glRotatef(270, 0, 1, 0);
            glScalef(0.39, 0.58, 0.48);
            glTranslatef(0.50, 3.4 + calfHeight * 2.5, -0.04);
            drawRightPyramid(-0.82);

            glRotatef(180, 0, 1, 0);
            glTranslatef(-0.07, 0, 0.92);
            drawRightPyramid(-0.82);
        }
        removeTexture();
        glPopMatrix();
        ///////////////////////

        // triangle shoe slope
        glPushMatrix(); // front shoe pyramid
        {
            float pyramidSize = 0.1;
            if (textureMode == 0)
            {
                initTexture("red2_texture.bmp");
            }
            else
            {
                initTexture("grey_texture.bmp");
            }
            glRotatef(270, 0, 1, 0);
            glScalef(0.39, 0.58, 0.48);
            glTranslatef(0.48, 1.27, -1.0);
            drawRightPyramid(-0.67);
        }
        removeTexture();
        glPopMatrix();

        glPushMatrix();
        {
            int jointSliceStack = 10;
            float jointRadius = 0.1;
            if (textureMode == 0)
            {
                initTexture("blue_texture.bmp");
            }
            else
            {
                initTexture("galvanized_blue.bmp");
            }

            glTranslatef(0.09, 0.210000, 0.39);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
            glTranslatef(0, 0, -0.2);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
            glTranslatef(0, 0, -0.2);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        }
        removeTexture();
        glPopMatrix();

        glPushMatrix();
        {
            if (textureMode == 0)
            {
                initTexture("yellow_texture.bmp");
            }
            else
            {
                initTexture("galvanized.bmp");
            }
            glScalef(1, 0.3, 1);
            glTranslatef(0, 0, -0.05);
            drawCuboid(0.5, 2);
        }
        removeTexture();
        glPopMatrix();
        glPushMatrix();
        {
            if (textureMode == 0)
            {
                initTexture("red2_texture.bmp");
            }
            else
            {
                initTexture("grey2_texture.bmp");
            }

            glTranslatef(0.1, 0.3 * 0.5, 0);
            glScalef(0.7, 0.6, 0.8);
            drawCuboid(0.5, 2);
        }
        removeTexture();
        glPopMatrix();

        glPushMatrix();
        {
            float pyramidSize = 0.1;
            if (textureMode == 0)
            {
                initTexture("red2_texture.bmp");
            }
            else
            {
                initTexture("galvanized_dark.bmp");
            }
            glRotatef(90, 0, 0, 1);
            glScalef(0.2, 0.2, 0.15);
            glTranslatef(0.52, 0.46, 0.85);
            drawPyramid();
            glTranslatef(0, 0, 1.0);
            drawPyramid();
            glTranslatef(0, 0, 1.0);
            drawPyramid();
        }
        removeTexture();
        glPopMatrix();
        glPushMatrix();
        {
            float pyramidSize = 0.1;
            if (textureMode == 0)
            {
                initTexture("red2_texture.bmp");
            }
            else
            {
                initTexture("galvanized_dark.bmp");
            }
            glRotatef(90, 1, 0, 0);
            glScalef(0.2, 0.2, 0.15);
            glTranslatef(0.5, 2.74, 0.02);
            drawPyramid();
            glTranslatef(1, 0, 0);
            drawPyramid();
            glTranslatef(1, 0, 0);
            drawPyramid();
            glTranslatef(1, 0, 0);
            drawPyramid();
            glTranslatef(1, 0, 0);
            drawPyramid();
        }
        removeTexture();
        glPopMatrix();
        glPushMatrix();
        {
            float pyramidSize = 0.1;
            if (textureMode == 0)
            {
                initTexture("red2_texture.bmp");
            }
            else
            {
                initTexture("galvanized_dark.bmp");
            }
            glRotatef(-90, 1, 0, 0);
            glScalef(0.2, 0.2, 0.15);
            glTranslatef(0.57, 0.65, 0.98);
            drawPyramid();
            glTranslatef(1, 0, 0);
            drawPyramid();
            glTranslatef(1, 0, 0);
            drawPyramid();
            glTranslatef(1, 0, 0);
            drawPyramid();
            glTranslatef(1, 0, 0);
            drawPyramid();
        }
        removeTexture();
        glPopMatrix();
        glPushMatrix(); // front shoe pyramid
        {
            float pyramidSize = 0.1;
            if (textureMode == 0)
            {
                initTexture("red2_texture.bmp");
            }
            else
            {
                initTexture("galvanized_dark.bmp");
            }
            glRotatef(-90, 0, 0, 1);
            glScalef(0.15, 0.13, 0.48);
            glTranslatef(-0.49, 8.21, 0.95);
            drawPyramid();
        }
        removeTexture();
        glPopMatrix();
    }
    glPopMatrix();
}

void drawBackground()
{
    // draw fence
    glPushMatrix();
    {
        float cylinderHeight = 0.6;
        glRotatef(-90, 1, 0, 0);
        glScalef(0.2, 0.2, 0.15);
        glTranslatef(0.57, 0.65, 0.98);
        drawPyramid();
    }
    glPopMatrix();
}

void drawThigh(int thighNo)
{
    glPushMatrix();
    {
        glTranslatef(0, 2.04, 0);
        glRotatef(0 + thighDegree[thighNo], 0, 0, 1);
        glTranslatef(0, -2.04, 0); //-1.84

        // glRotatef(0 + thighDegree, 0, 0, 1);
        // glTranslatef(0, -1.81, 0);

        glPushMatrix();
        {
            if (textureMode == 0)
            {
                initTexture("grey_texture.bmp");
            }
            else
            {
                initTexture("dark_texture.bmp");
            }
            glTranslatef(-0.1, -0.43 + robotHeight * 1.5, -0.02);
            glScalef(0.5, 3.18, 0.8);
            drawCuboid(0.5, 1.51);
        }
        removeTexture();
        glPopMatrix();

        glPushMatrix();
        {
            // glRotatef(90, 0, 1, 0);
            if (textureMode == 0)
            {
                initTexture("yellow_texture.bmp");
            }
            else
            {
                initTexture("red2_texture.bmp");
            }

            glTranslatef(0.09, 1.26, 0.05);
            drawCapsule(0.27, 0.12);
            glTranslatef(0., -0.28, 0);
            drawCapsule(0.27, 0.12);
            glTranslatef(0., -0.28, 0);
            drawCapsule(0.27, 0.12);
        }
        removeTexture();
        glPopMatrix();
    }
    glPopMatrix();
}

void drawHipJoint()
{
    glPushMatrix();
    {
        glTranslatef(0, -0.17, 0.04);
        glPushMatrix();
        {
            float cylinderHeight = 1.46;
            if (textureMode == 0)
            {
                initTexture("red2_texture.bmp");
            }
            else
            {
                initTexture("grey2_texture.bmp");
            }

            glRotatef(90, 0, 1, 0);
            glTranslatef(0.12, 1.48 + robotHeight * 1.45, -0.47);
            glScalef(1, 1, 1);
            drawCylinder(0.22, 0.22, cylinderHeight, 20, 10);
        }
        removeTexture();
        glPopMatrix();

        glPushMatrix();
        {
            int jointSliceStack = 10;
            float jointRadius = 0.22;
            if (textureMode == 0)
            {
                initTexture("yellow_texture.bmp");
            }
            else
            {
                initTexture("red2_texture.bmp");
            }

            glTranslatef(0.95, 1.48 + robotHeight * 1.45, -0.12);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
            glTranslatef(-1.37, 0, 0);
            drawSphere(jointRadius, jointSliceStack, jointSliceStack);
        }
        removeTexture();
        glPopMatrix();
    }
    glPopMatrix();
}

void drawBody()
{
    // hip
    glPushMatrix();
    {
        if (textureMode == 0)
        {
            initTexture("blue_metal_texture.bmp");
        }
        else
        {
            initTexture("red2_texture.bmp");
        }

        glRotatef(180, 0, 0, 1);
        glTranslatef(-0.17, -1.93, 0.42);
        glScalef(1.28, 1.32, 1);
        drawTrianglePyramid(-1, 1);
    }
    removeTexture();
    glPopMatrix();

    glPushMatrix();
    {
        if (textureMode == 0)
        {
            initTexture("red2_texture.bmp");
        }
        else
        {
            initTexture("dark_texture.bmp");
        }
        glTranslatef(-0.59, 2.49, -0.62);
        glScalef(0.65, 2.21, 1.23);
        drawCuboid(0.88, 2.64);
    }
    removeTexture();
    glPopMatrix();

    glPushMatrix();
    {
        if (textureMode == 0)
        {
            initTexture("grey_texture.bmp");
        }
        else
        {
            initTexture("2020_texture.bmp");
        }
        glTranslatef(-1.23, 4.44, -0.62);
        glScalef(0.65, 1.82, 1.26);
        drawCuboid(0.89, 4.83);
    }
    removeTexture();
    glPopMatrix();

    glPushMatrix();
    {
        if (textureMode == 0)
        {
            initTexture("blue_metal_texture.bmp");
        }
        else
        {
            initTexture("grey2_texture.bmp");
        }

        glRotatef(90, 1, 0, 0);
        glRotatef(90, 0, 0, 1);
        glTranslatef(-0.08, 0.91, -4.44);
        glScalef(1.08, 0.64, 1.25);
        drawRightPyramid(0.5);

        glRotatef(180, 0, 0, 1);
        glTranslatef(0, 3.36, 0);
        drawRightPyramid(0.5);
    }
    removeTexture();
    glPopMatrix();

    glPushMatrix();
    {
        if (textureMode == 0)
        {
            initTexture("yellow_texture.bmp");
        }
        else
        {
            initTexture("red2_texture.bmp");
        }
        glRotatef(90, 0, 1, 0);
        glTranslatef(0.09, 3.52, -0.61);
        drawCapsule(1.55, 0.12);
        glTranslatef(0, -0.31, 0);
        drawCapsule(1.55, 0.12);
        glTranslatef(0, -0.31, 0);
        drawCapsule(1.55, 0.12);
    }
    removeTexture();
    glPopMatrix();
}

void drawShoulderJoint()
{
    glPushMatrix();
    {
        if (textureMode == 0)
        {
            initTexture("yellow_texture.bmp");
        }
        else
        {
            initTexture("red2_texture.bmp");
        }
        glRotatef(90, 0, 1, 0);
        glTranslatef(0.02, 5.67, 1.32);
        drawCapsule(0.48, 0.16);
        glTranslatef(0, 0, -2.79);
        drawCapsule(0.48, 0.16);
    }
    removeTexture();
    glPopMatrix();
}

void drawLegs()
{
    //  leg
    glPushMatrix();
    {
        glPushMatrix();
        {
            glRotatef(90, 0, 1, 0);
            glTranslatef(0, 0, 0.7);
            drawThigh(0);

            glPushMatrix();
            {
                drawCalf(0);
            }
            glPopMatrix();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glRotatef(90, 0, 1, 0);
            glTranslatef(0, 0, -0.7);
            drawThigh(1);

            glPushMatrix();
            {
                drawCalf(1);
            }
            glPopMatrix();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-0.12, -0.12, 0);
            glScalef(1.11, 1, 1);
            drawHipJoint();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void drawArms()
{
    fingerCounter = 0;
    handCounter = 0;
    armCounter = 0;
    /////////////////////////// arm
    glPushMatrix();
    {
        glPushMatrix();
        {
            glTranslatef(0, 5.67, 0);
            glRotatef(0 + armDegreeX[0], 1, 0, 0);
            glTranslatef(0, -5.68, 0);

            float handDistance = 1.64;
            glTranslatef(0.23, 3.97, 0);
            glScalef(1.15, 1.39, 1.16);

            glPushMatrix();
            {
                // glRotatef(90, 0, 1, 0);
                glTranslatef(handDistance, 0, 0);
                drawArm(0);

                glPushMatrix();
                {
                    float pyramidSize = 0.1;
                    if (textureMode == 0)
                    {
                        initTexture("red2_texture.bmp");
                    }
                    else
                    {
                        initTexture("black_texture.bmp");
                    }

                    glRotatef(90, 0, 1, 0);
                    glRotatef(180, 0, 0, 1);
                    glScalef(0.80, 0.58, 0.48);
                    glTranslatef(-0.02, -1.95, 0.02);
                    drawRightPyramid(-1.29);
                }
                removeTexture();
                glPopMatrix();
            }
            glPopMatrix();
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0, 5.67, 0);
            glRotatef(0 + armDegreeX[1], 1, 0, 0);
            glTranslatef(0, -5.68, 0);

            float handDistance = 1.64;
            glTranslatef(0.23, 3.97, 0);
            glScalef(1.15, 1.39, 1.16);

            glPushMatrix();
            {
                // glRotatef(90, 0, 1, 0);
                glTranslatef(-handDistance, 0, 0);
                drawArm(1);

                glPushMatrix();
                {
                    float pyramidSize = 0.1;
                    if (textureMode == 0)
                    {
                        initTexture("red2_texture.bmp");
                    }
                    else
                    {
                        initTexture("black_texture.bmp");
                    }
                    glRotatef(270, 0, 1, 0);
                    glRotatef(180, 0, 0, 1);
                    glScalef(0.80, 0.58, 0.48);
                    glTranslatef(0.02, -1.95, 0.07);
                    drawRightPyramid(-1.29);
                }
                removeTexture();
                glPopMatrix();
            }
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void drawHead()
{
    glPushMatrix();
    {
        glRotatef(90, 1, 0, 0);
        glScalef(1.53, 1.64, 1.21);
        glTranslatef(0.12, -0.05, -5.51);
        float height = 0.16;
        float width = 0.5;
        glPushMatrix();
        {
            if (textureMode == 0)
            {
                initTexture("yellow_texture.bmp");
            }
            else
            {
                initTexture("darth_vader_texture.bmp");
            }

            // glTranslatef(0.25, 1.88 + calfHeight * 1.45, 0.04);
            glScalef(1, 1, 1);
            drawCylinder(height, height * 1.5, width, 20, 10);
        }
        removeTexture();
        glPopMatrix();

        glPushMatrix();
        {
            int jointSliceStack = 10;
            // float jointRadius = 0.12;
            // glTranslatef(0.25, 1.88 + calfHeight * 1.45, 0.33);
            if (textureMode == 0)
            {
                initTexture("green_texture.bmp");
            }
            else
            {
                initTexture("grey2_texture.bmp");
            }

            drawSphere(height, jointSliceStack, jointSliceStack);
            glTranslatef(0, 0, width);
            drawSphere(height, jointSliceStack, jointSliceStack);
        }
        removeTexture();
        glPopMatrix();
    }
    glPopMatrix();
}

void drawWingLeaf()
{
    // leaf
    glPushMatrix();
    {
        glPushMatrix();
        {
            glRotatef(90, 0, 1, 0);
            glTranslatef(-0.65, 5.39, -0.52);
            glScalef(0.12, 0.88, 1.97);
            drawTrianglePyramid(-1.0, 0.65);
            glTranslatef(0, -0.19, -1.05);
            glScalef(1, 0.63, 1.13);
            drawTrianglePyramid(-1.0, 0.5);

            // leaf connector
            glPushMatrix();
            {
                glTranslatef(0, -0.03, -0.07);
                glScalef(0.06, 2.54, 0.98);
                drawCuboid(0.18, 2.64);
            }
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void drawJetpack()
{
    glPushMatrix();
    { // joint between the two side of leaf

        glPushMatrix();
        {
            if (textureMode == 0)
            {
                initTexture("red2_texture.bmp");
            }
            else
            {
                initTexture("orange_texture.bmp");
            }
            glRotatef(90, 1, 0, 0);
            glTranslatef(0.15, 0.66, -5.7);
            drawCapsule(0.98, 0.16);
        }
        removeTexture();
        glPopMatrix();

        // rocket booster
        glPushMatrix();
        {
            if (textureMode == 0)
            {
                initTexture("white_blue_texture.bmp");
            }
            else
            {
                initTexture("Metal.bmp");
            }
            glRotatef(90, 1, 0, 0);
            glTranslatef(0.56, 0.73, -5.88);
            drawCylinder(0.25, 0.25, 1.8, 20, 10);
            glTranslatef(-0.83, 0, 0);
            drawCylinder(0.25, 0.25, 1.8, 20, 10);
            removeTexture();
            glPushMatrix();
            {
                if (textureMode == 0)
                {
                    initTexture("orange_texture.bmp");
                }
                else
                {
                    initTexture("grey_texture.bmp");
                }
                glTranslatef(0, 0, 0.14);
                int jointSliceStack = 10;
                float jointRadius = 0.25;
                // glTranslatef(0.25, 1.88 + calfHeight * 1.45, 0.33);
                drawSphere(jointRadius, jointSliceStack, jointSliceStack);
                glTranslatef(0.83, 0, 0);
                drawSphere(jointRadius, jointSliceStack, jointSliceStack);
            }
            removeTexture();
            glPopMatrix();

            glPushMatrix();
            {
                if (textureMode == 0)
                {
                    initTexture("orange_texture.bmp");
                }
                else
                {
                    initTexture("grey_texture.bmp");
                }
                glTranslatef(0.83, 0, 1.64);
                drawCylinder(0.18, 0.35, 0.45, 20, 10);
                glTranslatef(-0.83, 0, 0);
                drawCylinder(0.18, 0.35, 0.45, 20, 10);
            }
            removeTexture();
            glPopMatrix();
        }
        glPopMatrix();

        // leaf level 1
        glPushMatrix();
        {
            if (textureMode == 0)
            {
                initTexture("rainbow_texture.bmp");
            }
            else
            {
                initTexture("grey2_texture.bmp");
            }

            glTranslatef(0, -0.76, 0);
            glPushMatrix();
            {
                drawWingLeaf();
            }
            glPopMatrix();

            glPushMatrix();
            {
                glRotatef(180, 0, 1, 0);
                glTranslatef(-0.29, 0, -1.23);
                drawWingLeaf();
            }
            removeTexture();
            glPopMatrix();
        }
        glPopMatrix();

        // leaf level 2
        glPushMatrix();
        {
            // glTranslatef(v, v1, -0.07);
            if (textureMode == 0)
            {
                initTexture("green2_texture.bmp");
            }
            else
            {
                initTexture("bunker_galvanized.bmp");
            }

            glPushMatrix();
            {
                glRotatef(10, 0, 1, 0);
                glScalef(0.76, 1, 1);
                glTranslatef(-0.15, -0.39, 0.06);
                drawWingLeaf();
            }
            glPopMatrix();

            glPushMatrix();
            {

                glRotatef(170, 0, 1, 0);
                glScalef(0.76, 1, 1);
                glTranslatef(-0.53, -0.39, -1.23);
                drawWingLeaf();
            }
            removeTexture();
            glPopMatrix();
        }
        glPopMatrix();

        // leaf level 3
        glPushMatrix();
        {
            glPushMatrix();
            {
                if (textureMode == 0)
                {
                    initTexture("purple_texture.bmp");
                }
                else
                {
                    initTexture("galvanized_blue.bmp");
                }

                glRotatef(20, 0, 1, 0);
                glScalef(0.59, 0.63, 1.14);
                glTranslatef(-0.94, 3.03, 0.02);
                drawWingLeaf();
            }
            removeTexture();
            glPopMatrix();

            // leaf 3 connector
            glPushMatrix();
            {
                initTexture("orange_texture.bmp");
                glRotatef(90, 1, 0, 0);
                glTranslatef(0.85, 0.96, -5.58);
                drawCapsule(0.520, 0.14);

                glTranslatef(-1.33, 0.02, 0);
                drawCapsule(0.520, 0.14);
            }
            glPopMatrix();

            glPushMatrix();
            {
                if (textureMode == 0)
                {
                    initTexture("purple_texture.bmp");
                }
                else
                {
                    initTexture("galvanized_blue.bmp");
                }
                glRotatef(160, 0, 1, 0);
                glScalef(0.59, 0.63, 1.14);
                glTranslatef(-1.46, 3.02, -1.22);
                drawWingLeaf();
            }
            removeTexture();
            glPopMatrix();
        }
        glPopMatrix();

        // effect
        glPushMatrix();
        {
            glTranslatef(-0.25, 0.21, 5.72);
            glUpdateJetPackParticles1();
            glDrawJetPackParticles1();
            glTranslatef(0.85, -0.05, 0);
            glUpdateJetPackParticles2();
            glDrawJetPackParticles2();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void drawSword()
{
    glPushMatrix();
    {
        if (textureMode == 0)
        {
            initTexture("brown_texture.bmp");
        }
        else
        {
            initTexture("black_texture.bmp");
        }
        glRotatef(90, 0, 1, 0);
        glTranslatef(-0.65, 2.5, 2.0);
        glScalef(1, 1, 0.34);
        drawCuboid(0.5, 2.5);
    }
    glPopMatrix();

    glPushMatrix();
    {
        if (textureMode == 0)
        {
            initTexture("black_texture.bmp");
        }
        else
        {
            initTexture("black_texture.bmp");
        }
        glTranslatef(1.3, 2.5, -0.75);
        glScalef(1, 1, 0.34);
        drawCuboid(0.6, 2.5);
    }
    glPopMatrix();

    glPushMatrix();
    {
        if (textureMode == 0)
        {
            initTexture("Wood.bmp");
        }
        else
        {
            initTexture("dragon_texture.bmp");
        }
        glRotatef(90, 0, 1, 0);
        glTranslatef(0.75, 2.5, 2.0);
        glScalef(1, 1, 0.34);
        drawCuboid(0.5, 8.0);
    }
    glPopMatrix();

    glPushMatrix();
    {
        if (textureMode == 0)
        {
            initTexture("Wood.bmp");
        }
        else
        {
            initTexture("dragon_texture.bmp");
        }
        glRotatef(270, 1, 0, 0);
        glRotatef(90, 0, 1, 0);
        glTranslatef(-2.75, 5.0, 2.2); //z=zuoyou y=qianhou
        glScalef(0.5, 0.5, 0.2);
        drawTrianglePyramid(-1, 1);
    }
    removeTexture();
    glPopMatrix();
}

void drawIonBlaster()
{
    // 7 cylinder main barrel: http://bit.ly/2ZMGhZl
    glPushMatrix();
    {
        glRotatef(90, 0, 1, 0);
        glTranslatef(-0.85, 3.47, 1.24);

        // ionBlasterX = v;
        // ionBlasterY = v1;
        // ionBlasterZ = v2;

        // ionBlasterXDegree = v3;
        // ionBlasterYDegree = v4;
        // ionBlasterZDegree = v5;
        // if (fightMode)
        // {
        glTranslatef(ionBlasterX, ionBlasterY, ionBlasterZ);
        // }

        glRotatef(45, 1, 0, 0);
        glScalef(0.93, 0.93, 0.93);

        glRotatef(ionBlasterXDegree, 1, 0, 0);
        glRotatef(ionBlasterYDegree, 0, 1, 0);
        glRotatef(ionBlasterZDegree, 0, 0, 1);

        glPushMatrix();
        {
            initTexture("tiger_texture.bmp");
            // 1st and 2nd cylinder: muzzle
            glTranslatef(0, 0, -0.59);
            drawCylinder(0.25, 0.25, 1.8, 20, 10);
            glTranslatef(0, 0, 1.8);
            drawCylinder(0.25, 0.18, 0.25, 20, 10);
            glTranslatef(0, 0, -2.05);
            drawCylinder(0.18, 0.25, 0.25, 20, 10);
            removeTexture();
            // four cuboid decorate muzzle
            glPushMatrix();
            {
                // up and down
                glPushMatrix();
                {
                    initTexture("yellow_texture.bmp");
                    glRotatef(-90, 0, 1, 0);
                    glTranslatef(2.05, 0.1, -0.07);
                    drawCuboid(0.15, 1.66);
                    glRotatef(90, 1, 0, 0);
                    glTranslatef(0, 0, 0.2);
                    drawCuboid(0.15, 1.66);
                }
                removeTexture();
                glPopMatrix();

                // left and right
                glPushMatrix();
                {
                    initTexture("yellow_texture.bmp");
                    glRotatef(-90, 0, 1, 0);
                    glTranslatef(2.05, -0.08, 0.1);
                    drawCuboid(0.15, 1.66);
                    glRotatef(180, 1, 0, 0);
                    glTranslatef(0, -0.15, 0.2);
                    drawCuboid(0.15, 1.66);
                }
                removeTexture();
                glPopMatrix();
            }
            glPopMatrix();
        }
        glPopMatrix();

        glPushMatrix();
        {
            // second part
            initTexture("zebra_texture.bmp");
            glTranslatef(0, 0, -2.17);
            drawCylinder(0.25, 0.25, 1.25, 20, 10);
            glTranslatef(0, 0, -0.31);
            removeTexture();
            // middle part cylinder cover cap
            glPushMatrix();
            {
                initTexture("galvanized_blue.bmp");
                glScalef(1, 1, 0.57);
                glTranslatef(0, 0, 2.74);
                drawSphere(0.25, 10, 10);
                glTranslatef(0, 0, -2.19);
                drawSphere(0.25, 10, 10);
                removeTexture();
            }
            glPopMatrix();

            // up power light
            // 2 triangle pyramid and 1 cuboid
            glPushMatrix();
            {
                glTranslatef(-0.07, -0.12, 0);
                glPushMatrix();
                {
                    initTexture("galvanized_blue.bmp");
                    glRotatef(-90, 0, 1, 0);
                    glTranslatef(0.690, 0.25, -0.18);
                    drawCuboid(0.28, 1.66);
                    glScalef(0.55, 0.28, 1.54);
                    glTranslatef(0, 0.5, 0.21);
                    drawTrianglePyramid(-0.28, 1);
                    glTranslatef(0.85, 0, 0);
                    drawTrianglePyramid(-0.28, 1);
                    removeTexture();
                }
                glPopMatrix();
            }
            glPopMatrix();

            // down power light
            // 2 triangle pyramid and 1 cuboid
            glPushMatrix();
            {
                glRotatef(180, 1, 0, 0);
                glTranslatef(-0.06, -0.080, -1.61);
                glScalef(0.92, 0.78, 0.75);
                glPushMatrix();
                {
                    initTexture("galvanized_blue.bmp");
                    glRotatef(-90, 0, 1, 0);
                    glTranslatef(0.690, 0.25, -0.18);
                    drawCuboid(0.28, 1.66);
                    glScalef(0.55, 0.28, 1.54);
                    glTranslatef(0, 0.5, 0.21);
                    drawTrianglePyramid(-0.28, 1);
                    glTranslatef(0.85, 0, 0);
                    drawTrianglePyramid(-0.28, 1);
                }
                removeTexture();
                glPopMatrix();
            }
            glPopMatrix();

            glPushMatrix();
            {
                glTranslatef(0, 0.04, 0);
                // pointer light 1 cuboid, 1 cylinder
                glPushMatrix();
                {
                    initTexture("galvanized_blue.bmp");
                    glTranslatef(-0.2, -0.55, 0.92);
                    glScalef(0.94, 0.71, 1.07);
                    drawCuboid(0.26, 1.61);
                }
                glPopMatrix();

                glPushMatrix();
                {
                    initTexture("galvanized_blue.bmp");
                    glTranslatef(0.02, -0.46, 1.16);
                    drawCylinder(0.04, 0.04, 0.35, 20, 10);
                }
                removeTexture();
                glPopMatrix();
            }
            glPopMatrix();

            // four cuboid decorate muzzle
            glPushMatrix();
            {
                glScalef(1.06, 0.89, 0.6);
                glTranslatef(0, 0, 0.63);
                // up and down
                glPushMatrix();
                {
                    initTexture("yellow_texture.bmp");
                    glRotatef(-90, 0, 1, 0);
                    glTranslatef(2.05, 0.1, -0.07);
                    drawCuboid(0.15, 1.66);
                    glRotatef(90, 1, 0, 0);
                    glTranslatef(0, 0, 0.2);
                    drawCuboid(0.15, 1.66);
                }
                removeTexture();
                glPopMatrix();

                // left and right
                glPushMatrix();
                {
                    initTexture("yellow_texture.bmp");
                    glRotatef(-90, 0, 1, 0);
                    glTranslatef(2.05, -0.08, 0.1);
                    drawCuboid(0.15, 1.66);
                    glRotatef(180, 1, 0, 0);
                    glTranslatef(0, -0.15, 0.2);
                    drawCuboid(0.15, 1.66);
                }
                removeTexture();
                glPopMatrix();
            }
            glPopMatrix();
        }
        glPopMatrix();

        glPushMatrix();
        {
            // third part
            initTexture("blue_texture.bmp");
            glTranslatef(0, 0, -3.43);
            drawCylinder(0.32, 0.32, 0.92, 20, 10);

            // third part cylinder cover cap and connector
            glPushMatrix();
            {
                initTexture("grey_texture.bmp");
                glTranslatef(0, 0, 0.96);
                drawCylinder(0.21, 0.13, 0.3, 20, 10);
            }
            removeTexture();
            glPopMatrix();

            glPushMatrix();
            {
                initTexture("red2_texture.bmp");
                glScalef(1, 1, 0.44);
                glTranslatef(0, 0, 2.090);
                drawSphere(0.32, 10, 10);
            }
            glPopMatrix();

            // up power light
            // 2 triangle pyramid and 1 cuboid
            glPushMatrix();
            {
                glScalef(1, 1, 1);
                glTranslatef(-0.07, 0, -0.7);
                glPushMatrix();
                {
                    initTexture("blue_texture2.bmp");
                    glRotatef(-90, 0, 1, 0);
                    glTranslatef(0.690, 0.25, -0.18);
                    drawCuboid(0.28, 1.66);
                    glScalef(0.55, 0.28, 1.54);
                    glTranslatef(0, 0.5, 0.21);
                    drawTrianglePyramid(-0.28, 1);
                    glTranslatef(0.85, 0, 0);
                    drawTrianglePyramid(-0.28, 1);
                }
                removeTexture();
                glPopMatrix();
            }
            glPopMatrix();

            // down magazine
            glPushMatrix();
            {
                initTexture("blue_texture2.bmp");
                // 2 cuboid, 1 triangle pyramid
                glScalef(1.02, 3.03, 2.42);
                glTranslatef(-0.12, -0.23, 0.05);
                drawCuboid(0.14, 1.69);

                glScalef(1, 0.7, 0.37);
                glTranslatef(-0.01, 0.05, 0.38);
                drawCuboid(0.15, 1.66);

                glRotatef(90, 0, 1, 0);
                glRotatef(90, 0, 0, 1);
                glScalef(0.11, 0.15, 0.85);
                glTranslatef(0.03, 0.51, 0.29);
                drawTrianglePyramid(-0.28, 1);
            }
            removeTexture();
            glPopMatrix();
        }
        glPopMatrix();

        glPushMatrix();
        {
            // forth part
            initTexture("tiger_texture.bmp");
            glTranslatef(0, 0, -4.21);
            drawCylinder(0.39, 0.39, 0.67, 20, 10);

            // cap
            glPushMatrix();
            {
                initTexture("yellow_texture.bmp");
                glScalef(1, 1, 0.34);
                glTranslatef(0, 0, 2.07);
                drawSphere(0.41, 10, 10);
                glTranslatef(0, 0, -2.07);
                drawSphere(0.41, 10, 10);
            }
            removeTexture();
            glPopMatrix();

            // grip
            glPushMatrix();
            {
                glPushMatrix();
                {
                    initTexture("black_texture.bmp");
                    glRotatef(12, 1, 0, 0);
                    glScalef(1.35, 1.36, 0.53);
                    glTranslatef(-0.15, -0.78, 0.65);
                    drawCuboid(0.61, 0.48);
                }
                glPopMatrix();

                glPushMatrix();
                {
                    initTexture("black_texture.bmp");
                    glRotatef(90, 0, 1, 0);
                    glRotatef(122, 0, 0, 1);
                    glScalef(0.14, 0.19, 1.25);
                    glTranslatef(-5.13, 5.45, -0.17);
                    drawTrianglePyramid(0.32, 1);
                }
                glPopMatrix();
            }
            glPopMatrix();

            // four cuboid decorate muzzle
            glPushMatrix();
            {
                glScalef(1.06, 0.89, 0.6);
                glTranslatef(0, 0, 0.93);
                // up and down
                glPushMatrix();
                {
                    initTexture("yellow_texture.bmp");
                    glRotatef(-90, 0, 1, 0);
                    glTranslatef(2.05, 0.1, -0.07);
                    drawCuboid(0.15, 1.66);
                    glRotatef(90, 1, 0, 0);
                    glTranslatef(0, 0, 0.2);
                    drawCuboid(0.15, 1.66);
                }
                removeTexture();
                glPopMatrix();

                // left and right
                glPushMatrix();
                {
                    glRotatef(-90, 0, 1, 0);
                    glTranslatef(2.05, -0.08, 0.1);
                    drawCuboid(0.15, 1.66);
                    glRotatef(180, 1, 0, 0);
                    glTranslatef(0, -0.15, 0.2);
                    drawCuboid(0.15, 1.66);
                }
                glPopMatrix();
            }
            glPopMatrix();
        }
        glPopMatrix();

        // shoot energy
        glPushMatrix();
        {
            shoot();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void drawSky()
{
    glPushMatrix();
    {
        if (textureMode == 0)
        {
            initTexture("sky.bmp");
        }
        else
        {
            initTexture("night_sky.bmp");
        }
        glRotatef(-90, 1, 0, 0);
        glTranslatef(0.14, 1.4, 0);
        drawSphere(155, 50, 50);
        removeTexture();
    }
    glPopMatrix();
}

void drawGround()
{
    glPushMatrix();
    {
        float size = 11;
        int noOfGrass = 50;
        float resetValue = size * noOfGrass;
        if (textureMode == 0)
        {
            initTexture("grass.bmp");
        }
        else
        {
            initTexture("moon_texture.bmp");
        }

        //  nested loop draw ground
        glTranslatef(-400, -2.44, -200);
        for (int j = 0; j < noOfGrass; j++)
        {
            glTranslatef(size, 0, 0);
            // draw column
            for (int i = 0; i < noOfGrass; i++)
            {
                glTranslatef(0, 0, size);
                glPushMatrix();
                {
                    glBegin(GL_QUADS);
                    {
                        glMaterialfv(GL_FRONT, GL_AMBIENT, diffuseColor);
                        glTexCoord2f(0.0f, 1);
                        glVertex3f(-0.5 * size, 0, -1 * size);

                        glTexCoord2f(1, 1);
                        glVertex3f(-0.5 * size, 0, 0);

                        glTexCoord2f(1, 0.0f);
                        glVertex3f(0.5 * size, 0, 0);

                        glTexCoord2f(0.0f, 0.0f);
                        glVertex3f(0.5 * size, 0, -1 * size);
                    }
                    glEnd();
                }
                glPopMatrix();
            }
            glTranslatef(0, 0, -resetValue);
        }
        removeTexture();
    }
    glPopMatrix();
}

void display()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if (isOrtho)
    {
        glScalef(1 + orthoZ, 1 + orthoZ, 1 + orthoZ);
    }

    glPushMatrix();
    {
        glTranslatef(-0.1, 2.41, 4.92);
        glUpdateParticles();
        glDrawParticles();
    }
    glPopMatrix();

    if (frameDisplayCount == 100)
    {
        spawn = false;
        frameDisplayCount = 0;
    }

    if (isTexture)
    {
        selectedGluDrawStyles = gluDrawStyles[0];
        selectedGlDrawStyles = glDrawStyles[0];
        selectedGlewDrawStyles = glewDrawStyles[0];
    }


    glPushMatrix();
    {
        if (animationY <= -0.71)
        {
            animationY = -0.71;
        }
        glTranslatef(animationX, animationY, animationZ);
        glRotatef(-90 + walkDirectionDegree, 0, 1, 0);
        glRotatef(flyForwardDegree, 1, 0, 0);
        glScalef(0.7, 0.7, 0.7);
        drawLegs();
        drawArms();
        drawBody();
        drawHead();
        drawShoulderJoint();
        // item
        if (jetpack)
            drawJetpack();
        else if (ionBlaster)
            drawIonBlaster();
        else if (sword)
            drawSword();
    }
    glPopMatrix();

    if (isTexture)
    {
        drawSky();
        drawGround();
    }

    if(light){
        glPushMatrix();
        {
            glEnable(GL_LIGHTING);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
            glLightfv(GL_LIGHT0, GL_POSITION, diffusePosition);
            glEnable(GL_LIGHT0);
        }
        glPopMatrix();
    }else{
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
    }
}

int main(int argc, char **argv)
{
    GLFWwindow *window = initWindow(1920, 1080);
    glCreateParticles();
    glCreateJetPackParticles1();
    glCreateJetPackParticles2();

    if (NULL != window)
    {
        glGenTextures(50, textures);

        while (!glfwWindowShouldClose(window))
        {
            // Scale to window size
            GLint windowWidth, windowHeight;
            glfwGetWindowSize(window, &windowWidth, &windowHeight);
            glViewport(0, 0, windowWidth, windowHeight);

            // setup the camera and
            // toggle different view mode
            if (isOrtho)
            {
                glLoadIdentity();
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(-2 * ar, 2 * ar, -2, 2, -2, 2000);
                glTranslatef(orthoX, orthoY, 0);
                glRotatef(speed, 0, 1, 0);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
            }
            else
            {
                glLoadIdentity();
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                gluPerspective(45, ar, 1, 2000);
                glTranslatef(perspectiveX, perspectiveY, perspectiveZ);
                glRotatef(speed, 0, 1, 0);

                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
            }

            // walk animation
            if (walkControl)
            {
                if (direction != 0)
                    walk(1);
            }
            else if (runControl)
            {
                if (direction != 0)
                    walk(3);
            } // fly animation
            else if (flyMode)
            {
                flyHorizontal();
            }
            else if (fightMode)
            {
                activateFightMode();
            }

            if (insultControl)
            {
                insult();
            }

            if (spawn)
            {
                frameDisplayCount++;
            }

            display();

            if (rotate)
                speed += 1;
            else
                speed = speed;

            // Update Screen
            glfwSwapBuffers(window);

            // Check for any input, or window movement
            glfwPollEvents();
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
