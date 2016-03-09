#include "Stone.h"

bool g_isStop = false;

void Init(int argc, char ** argv)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);//设置背景颜色为黑色
    glShadeModel(GL_SMOOTH);//设置为光滑明暗模式
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
}

void InitWindow(int width, int height, int x, int y, const char* title) {
    glutInitWindowSize(width, height);
    glutInitWindowPosition(x, y);
    glutCreateWindow(title);//窗口标题
}

void InitMenu() {
    glutCreateMenu(OnMenu);
    glutAddMenuEntry("Clear Screen", CLEAR);
    glutAddMenuEntry("Create Rigidbody", CREATE);
    glutAddMenuEntry("Ray Casting", RAY_CAST);
    glutAddMenuEntry("Pick Rigidbody", PICK);
    glutAddMenuEntry("Add Rod", ROD);
    glutAddMenuEntry("Add Cable", CABLE);
    glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

void Reshade(GLsizei w,GLsizei h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, 0, HEIGHT, -10, 10);
}

int frame=0,elaspseTime,timebase=0;
void MainUpdate(int dt)
{
    //cal fps
    frame++;
    elaspseTime = glutGet(GLUT_ELAPSED_TIME);
    if (elaspseTime - timebase > 1000) {
        real fps = frame*1000.0/(elaspseTime-timebase);
        cout << "FPS: " << fps << endl;
        timebase = elaspseTime;
        frame = 0;
    }

    Update();
    glutPostRedisplay();
    if(!g_isStop)
        glutTimerFunc(FRAME_TIME,MainUpdate, dt);
}

void MainDraw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    OnDraw();
    glFlush();
    glutSwapBuffers();
} 

void MainSpecialKey(int key, int x, int y) {
    //Stop the frame
    if(key == GLUT_KEY_F1) {
        if(g_isStop) {
            g_isStop = false;
            glutTimerFunc(FRAME_TIME,MainUpdate, FRAME_TIME);
        }
        else
            g_isStop = true;
    }

    OnSpecialKey(key, x, y);
}

void MainMouse( int button, int state, int x, int y )
{
    OnMouse(button, state, x, y);
}

int main(int argc, char** argv)
{
    Init(argc, argv);
    InitWindow(WIDTH, HEIGHT, 500, 0, "Stone");
    InitMenu();
    glutReshapeFunc(Reshade);
    glutTimerFunc(FRAME_TIME,MainUpdate,FRAME_TIME);
    glutDisplayFunc(MainDraw);
    glutSpecialFunc(MainSpecialKey);
    glutMouseFunc(MainMouse);
    glutPassiveMotionFunc(OnPassiveMouse);

    OnCreate();

    glutMainLoop( );

    return 0;
}
