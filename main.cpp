#include "DetGraphics.hpp"

using namespace std;

int rekey(XEvent* event, GC gc, const std::vector<Window>& win_arr, Matrix& matrix, Window det_win) {
    Display* dpy = event->xkey.display;
    KeySym ks = XkbKeycodeToKeysym(dpy, event->xkey.keycode, 0, 0);
    if(ks == XK_q || ks == XK_Q) return 1;
    if (ks == XK_Alt_L || ks == XK_Alt_R) {
        reverse(dpy, win_arr, gc, matrix);
          int det_val = 0;
        det(matrix, &det_val);
        drawDet(dpy, det_win, gc, det_val, matrix.size);
        return 0;
    }
    if (ks == XK_Escape) {
        nullify(dpy, win_arr, gc, matrix);
        int det_val = 0;
        det(matrix, &det_val);
        drawDet(dpy, det_win, gc, det_val, matrix.size);
        return 0;
    }

    return 0;
}

void dispatch(Display* dpy, std::vector<Window> win_arr, GC gc, Matrix& matrix, Window det_win) {
    XEvent event;
    int done = 0;
    Window win;
    int i = 0;
    int det_val = 0;
    while(done == 0) {
        XNextEvent(dpy, &event);
        switch (event.type) {
            case ButtonPress:
                win = event.xkey.window;
                for (i = 0; win != win_arr[i]; ++i) ;
                det_val = 0;
                change(dpy, win, gc, i, matrix);
                det(matrix, &det_val);
                drawDet(dpy, det_win, gc, det_val, matrix.size);
                break;
            case KeyPress:
                done = rekey(&event, gc, win_arr, matrix, det_win);
                break;
            default:
                break;
        }
    }
}

void stop(Display* dpy, Window main, const vector<Window>& win_arr, Window det_win) {
    for (auto i: win_arr) {
        XDestroyWindow(dpy, i);
    }
    XDestroyWindow(dpy, det_win);
    XDestroyWindow(dpy, main);
    XCloseDisplay(dpy);
}

int main(int argc, char* argv[]) {

    if(argc < 2) { cout << "Мало аргументов. Ошибка." << endl; return -1; }
    if(!atoi(argv[1])) { cout << "Некорректное значение аргумента. Ошибка." << endl; return -2; }

    int siz = atoi(argv[1]);
    if(siz <= 0 || siz >= 8) { cout << "Введите значения из диапазона [1; 7]" << endl; return -3; }

    Matrix matrix = Matrix(siz);
    Display* dpy;
    Window main;
    GC gc;

    dpy = XOpenDisplay(nullptr);
    gc = CreateContext(dpy);
    main = CreateMainWindow(dpy, siz);
    std::vector<Window> win_arr;
    win_arr.resize(matrix.size*matrix.size);
    for (int i=0; i < win_arr.size(); ++i) {
            win_arr[i] = CreateMatrixWindow(dpy, main, i, matrix.size);
    }
    draw(dpy, win_arr, gc);
    Window det_win = createDet(dpy, main, matrix.size);
    drawDet(dpy, det_win, gc, 0, matrix.size);
    dispatch(dpy, win_arr, gc, matrix, det_win);
    matrix.deleteMatrix();
    stop(dpy, main, win_arr, det_win);

    return 0;
}
