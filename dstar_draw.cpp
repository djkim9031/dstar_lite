#include "dstar.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;

int window;
int mbutton = 0;
int mstate = 0;
const int win_width = 1000;
const int win_height = 800;

int dim_x;
int dim_y;

float size_x;
float size_y;

int sweep_range;

vector<vector<bool>> walls;
vector<vector<float>> g_vals;
pair<int, int> start_coord;
pair<int, int> terminal_coord;
pair<int, int> robot_coord;
pair<int, int> changed_cell;

bool QPressed, leftClicked, rightClicked;

void DrawScene()
{

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);

    glColor3f(1, 1, 0);
    //-1 ~ +1 range
    //Dstar_Lite method stores coordinates in (row, col), and the draw method needs to transform to (x,y) coord


    //1. Draws rectangular cells. Start grid will be painted green, Terminal grid will be painted red, etc. 
    for (int i = 0; i < dim_x; ++i) {
        const float i_curr = -1.0f + (i * 2.0f / dim_x);
        for (int j = 0; j < dim_y; ++j) {
            const float j_curr = -1.0f + (j * 2.0f / dim_y);
            if (i == start_coord.second && dim_y - j - 1 == start_coord.first) {
                glColor3f(0, 1, 0);
            }
            else if (i == terminal_coord.second && dim_y - j - 1 == terminal_coord.first) {
                glColor3f(1, 0, 0);
            }
            else if (walls[i][dim_y - j - 1]) {
                glColor3f(0.3, 0.3, 0.3);
            }
            else {
                glColor3f(0.7, 0.7, 0.7);
            }

            glVertex2f(i_curr, j_curr);
            glVertex2f(i_curr + size_x, j_curr);
            glVertex2f(i_curr + size_x, j_curr + size_y);
            glVertex2f(i_curr, j_curr + size_y);

        }
    }
    glEnd();

    //2. Draws grids that visually separate the cells drawn above.
    glBegin(GL_LINES);
    glColor3f(0.6, 0.3, 0.1);
    for (int i = 0; i <= dim_x; ++i) {
        const float i_curr = -1.0f + (i * 2.0f / (dim_x));
        for (int j = 0; j <= dim_y; ++j) {
            const float j_curr = -1.0f + (j * 2.0f / (dim_y));
            glVertex2f(i_curr, 1.0f);
            glVertex2f(i_curr, -1.0f);
            glVertex2f(-1.0f, j_curr);
            glVertex2f(1.0f, j_curr);
        }
    }

    glEnd();

    //3. Draws the agent(robot) as a blue circle
    glBegin(GL_POLYGON);
    glColor3f(0.2, 0.2, 0.7);
    float radius = min(size_x, size_y) / 2;
    float ori_x = -1.0f + (robot_coord.second * 2.0f / dim_x) + size_x / 2;
    float ori_y = -1.0f + ((dim_y - robot_coord.first - 1) * 2.0f / dim_y) + size_y / 2;
    for (int i = 0; i <= 360; i++) {
        float angle = 2 * 3.14 * i / 360;
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;
        glVertex2f(ori_x + x, ori_y + y);
    }
    glEnd();

    //4. Draws a rectangle showing the sweeping range of the robot
    glBegin(GL_LINES);
    glColor3f(0.9, 0, 0);
    glVertex2f(max(-1.0f, ori_x - size_x * sweep_range), max(-1.0f, ori_y - size_y * sweep_range));
    glVertex2f(max(-1.0f, ori_x - size_x * sweep_range), min(1.0f, ori_y + size_y * sweep_range));
    glVertex2f(min(1.0f, ori_x + size_x * sweep_range), min(1.0f, ori_y + size_y * sweep_range));
    glVertex2f(min(1.0f, ori_x + size_x * sweep_range), max(-1.0f, ori_y - size_y * sweep_range));
    glVertex2f(max(-1.0f, ori_x - size_x * sweep_range), max(-1.0f, ori_y - size_y * sweep_range));
    glVertex2f(min(1.0f, ori_x + size_x * sweep_range), max(-1.0f, ori_y - size_y * sweep_range));
    glVertex2f(max(-1.0f, ori_x - size_x * sweep_range), min(1.0f, ori_y + size_y * sweep_range));
    glVertex2f(min(1.0f, ori_x + size_x * sweep_range), min(1.0f, ori_y + size_y * sweep_range));
    glEnd();


    //5. Text output showing the g values of each "worth-exploring" cells
    for (int i = 0; i < dim_x; ++i) {
        const float i_curr = -1.0f + (i * 2.0f / dim_x);
        for (int j = 0; j < dim_y; ++j) {
            if (g_vals[j][i] == FLT_MAX) {
                continue;
            }
            const float j_curr = -1.0f + (j * 2.0f / dim_y);
            string val = to_string(g_vals[j][i]);
            val = val.substr(0, val.find(".") + 2);
            const unsigned char* v = (unsigned char*)val.c_str();
            glColor3f(0.1, 0.1, 0.5);
            glRasterPos2f(i_curr, -j_curr - size_y);
            glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, v);
        }
    }


    glutSwapBuffers();

}

void keyPressed(unsigned char key, int x, int y)
{
    //std::this_thread::sleep_for(std::chrono::milliseconds(100));

    switch (key) {
    case 'q':
    case 'Q':
        QPressed = true;
        break;
        //case ' ':
        //    break;
    }


}

void mouseFunc(int button, int state, int x, int y) {


    mbutton = button;
    //x = width, y = height dimension
    int i = x * 2 / (size_x * win_width);
    int j = y * 2 / (size_y * win_height);
    changed_cell.first = j;
    changed_cell.second = i;

    if ((mstate = state) == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            leftClicked = true;

        }
        else if (button == GLUT_RIGHT_BUTTON) {
            if (walls[i][j]) {
                walls[i][j] = false;
            }
            else {
                walls[i][j] = true;
            }
            rightClicked = true;
        }


    }
}

void DStar_Lite::PathFinder(int argc, char** argv) {

    //Initialzing draw methods global params
    start_coord = startCoord;
    terminal_coord = terminalCoord;
    robot_coord = startCoord;
    dim_x = nCol;
    dim_y = nRow;
    sweep_range = SWEEPING_RANGE;

    size_x = (2.0f / dim_x);
    size_y = (2.0f / dim_y);
    vector<vector<bool>> status_init(dim_x, vector<bool>(dim_y, false));
    walls = status_init;
    status_init.clear();
    for (int r = 0; r < nRow; ++r) {
        vector<float> temp;
        for (int c = 0; c < nCol; ++c) {
            temp.push_back(FLT_MAX);
        }
        g_vals.push_back(temp);
    }

    //Initializing D* environment
    initDStarLite();
    bool done = false;

    //Initialzing GUI
    QPressed = false; leftClicked = false; rightClicked = false;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(win_width, win_height);
    glutInitWindowPosition(50, 20);

    window = glutCreateWindow("D Star Lite - Current Environment");

    glutDisplayFunc(&DrawScene);
    glutIdleFunc(&DrawScene);
    glutKeyboardFunc(&keyPressed);
    glutMouseFunc(&mouseFunc);

    printf("----------------------------------\n");
    printf("Let's get it moving!\n");
    printf("Commands:\n");
    printf("[q/Q] - Quit\n");
    printf("Left mouse click - Move the robot\n");
    printf("Right mouse click - Place or Lift an obstacle if there is one\n");
    printf("----------------------------------\n");



    while (!done) {
        glutMainLoopEvent();

        if (QPressed) {
            QPressed = false;
            cout << "Exiting the Window..." << endl;
            done = true;
            glutDestroyWindow(window);
            exit(0);
            continue;
        }


        if (leftClicked && robot_coord != terminalCoord) {
            pair<int, int> new_robot_coord = MotionAndScan(robot_coord, SWEEPING_RANGE);
            leftClicked = false;
            robot_coord = new_robot_coord;

            if (new_robot_coord == terminalCoord) {
                cout << "Goal Reached!" << endl;
                cout << "Press 'q' to exit" << endl << endl;
                continue;
            }
        }

        if (rightClicked) {
            if (cells[changed_cell.first][changed_cell.second].status >= 0) {
                cells[changed_cell.first][changed_cell.second].status = -1;
            }
            else {
                cells[changed_cell.first][changed_cell.second].status = 1;
            }
            rightClicked = false;
        }


        //Extra functionality to print the g value of cells within the "worth-exploring" range. 
        for (int r = 0; r < nRow; ++r) {
            for (int c = 0; c < nCol; ++c) {
                g_vals[r][c] = cells[r][c].g;
            }
        }

    }


}