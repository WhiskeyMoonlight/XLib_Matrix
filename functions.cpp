#include "DetGraphics.hpp"

GC CreateContext(Display* dpy) {
    GC gc = DefaultGC(dpy, DefaultScreen(dpy));
    XSetBackground(dpy, gc, WhitePixel(dpy, DefaultScreen(dpy)));

    return gc;
}

Window CreateMainWindow(Display* dpy, int siz) {
    Window win;
    XSetWindowAttributes attr;
    XSizeHints hint;
    int x, y;
    int scr;

    attr.override_redirect = False;
    attr.background_pixel = WhitePixel(dpy, DefaultScreen(dpy));
    attr.event_mask = (KeyPressMask | StructureNotifyMask);
    scr =  DefaultScreen(dpy);

    x = (DisplayWidth(dpy, scr) - mainWidth) / 2;
    y = (DisplayHeight(dpy, scr) - mainHeight) / 2;

    win = XCreateWindow(dpy, DefaultRootWindow(dpy), x, y,  mainWidth + siz * numWidth, mainHeight + siz * numHeight, 1,
                        DefaultDepth(dpy, scr), InputOutput, CopyFromParent,
                        (CWOverrideRedirect | CWBackPixel | CWEventMask ),
                        &attr);
    hint.flags = (PMinSize | PPosition | PMaxSize);
    hint.min_width = hint.max_width = mainWidth;
    hint.min_height = hint.max_height = mainHeight;
    hint.x = x;
    hint.y = y;

    XSetNormalHints(dpy, win, &hint);
    XMapWindow(dpy, win);

    return win;
}

Window CreateMatrixWindow(Display* dpy, Window main, int index, int siz) {
    Window win;
    int _j = index / siz;
    int _i = index % siz;
    XSetWindowAttributes attr;
    XSizeHints hint;
    int x, y;
    int w, h;
    int scr;

    attr.override_redirect = False;
    attr.background_pixel = WhitePixel(dpy, DefaultScreen(dpy));
    attr.event_mask = (ButtonPressMask | KeyPressMask);

    scr = DefaultScreen(dpy);

    w = numWidth;
    h = numHeight;
    x = mainWidth / 2 + _i * numWidth;		//Позиция по x определяется разрядом машинного слова, под который создаётся ячейка. Считается как отклонение на (16 - digit) ячеек от центра
    y = mainHeight / 9 + _j * numHeight;

    win = XCreateWindow(dpy, main, x, y, w, h, 1,
                        DefaultDepth(dpy, scr), InputOutput, CopyFromParent,
                        (CWOverrideRedirect | CWBackPixel | CWEventMask),
                        &attr);
    hint.flags = (PMinSize | PPosition | PMaxSize);
    hint.min_width = hint.max_width = w;
    hint.min_height = hint.max_height = h;
    hint.x = x;
    hint.y = y;

    XSetNormalHints(dpy, win, &hint);
    XMapWindow(dpy, win);

    return win;

}

void draw(Display* dpy, const std::vector<Window>& win_arr, GC gc) {
    Font font;
    font = XLoadFont(dpy, "*x24");
    XSetFont(dpy, gc, font);
    for (auto i : win_arr) {
        XDrawString(dpy, i, gc, numWidth/2-5, numHeight/2+6, "0", 1);
    }
}

void drawDet(Display* dpy, Window win, GC gc, int det_val, int siz) {
    Font font;
    font = XLoadFont(dpy, "*x24");
    XSetFont(dpy, gc, font);
    XClearArea(dpy, win, 0, 0, 0, 0, True);
    std::string string_det = "Det = " + std::to_string(det_val);
    if (siz < 3) { string_det = "D = " + std::to_string(det_val); }
    if (siz > 3) {
        for (int i = 3; i < siz; ++i) {
            string_det.insert(string_det.cbegin(), ' ');
            string_det.insert(string_det.cbegin(), ' ');
        }
    }
    const char* char_det = string_det.c_str();

    XDrawString(dpy, win, gc, numWidth/2-5, numHeight/2+6, char_det, string_det.size());
}

void change(Display* dpy, Window win, GC gc, int i, Matrix& matrix) {
    int num = matrix.matrix.at(i);
    XClearArea(dpy, win, 0, 0, 0, 0, True);
    if (num == 0) {
        matrix.matrix.at(i) = 1;
        XDrawString(dpy, win, gc, numWidth/2-5, numHeight/2+6, "1", 1);
    }
    if(num == 1){
        matrix.matrix.at(i) = 0;
        XDrawString(dpy, win, gc, numWidth/2-5, numHeight/2+6, "0", 1);
    }
}

void reverse(Display* dpy, const std::vector<Window>& win_arr, GC gc, Matrix& matrix) {
    int n = 0;
    for(auto i: win_arr) {
        XClearArea(dpy, i, 0, 0, 0, 0, True);
        int num = matrix.matrix.at(n);
        if (num == 0) {
            matrix.matrix.at(n) = 1;
            XDrawString(dpy, i, gc, numWidth/2-5, numHeight/2+6, "1", 1);
        }
        if (num == 1) {
            matrix.matrix.at(n) = 0;
            XDrawString(dpy, i, gc, numWidth/2-5, numHeight/2+6, "0", 1);
        }
        n++;
    }
}

void nullify(Display* dpy, const std::vector<Window>& win_arr, GC gc, Matrix& matrix) {
    int n = 0;
    for(auto i: win_arr) {
        XClearArea(dpy, i, 0, 0, 0, 0, True);
        matrix.matrix.at(n) = 0;
        XDrawString(dpy, i, gc, numWidth/2-5, numHeight/2+6, "0", 1);
        n++;
    }
}

Window createDet(Display* dpy, Window main, int siz) {
    Window win;
    XSetWindowAttributes attr;
    XSizeHints hint;
    int x, y;
    int w, h;
    int scr;

    attr.override_redirect = False;
    attr.background_pixel = WhitePixel(dpy, DefaultScreen(dpy));

    scr = DefaultScreen(dpy);

    if (siz == 1 ) {
        w = numWidth * 2;
        x = mainWidth / 2 - numWidth / 2;
    } else {
        w = numWidth * siz;
        x = mainWidth / 2;
    }
    h = numHeight;
    y = mainHeight / 9 + siz * numHeight + numHeight / 2;

    win = XCreateWindow(dpy, main, x, y, w, h, 1,
                        DefaultDepth(dpy, scr), InputOutput, CopyFromParent,
                        (CWOverrideRedirect | CWBackPixel),
                        &attr);
    hint.flags = (PMinSize | PPosition | PMaxSize);
    hint.min_width = hint.max_width = w;
    hint.min_height = hint.max_height = h;
    hint.x = x;
    hint.y = y;

    XSetNormalHints(dpy, win, &hint);
    XMapWindow(dpy, win);

    return win;
}

Matrix MinorCreation(const Matrix &to_copy, int col_to_kick, int row_to_kick) {
    int siz = to_copy.size - 1;
    Matrix new_matrix(siz);
    int counter = 0;
    for (int i = 0; i <= siz; ++i) {
        if(i != row_to_kick) {
            for (int j = 0; j <= siz; ++j) {
                if(j != col_to_kick) {
                    new_matrix.matrix.at(counter++) = to_copy.matrix.at(to_copy.size * i + j);
                }
            }
        }
    }
    return new_matrix;
}

int det(const Matrix &origin, int *value) {
    int temp = 0;
    int degree = 1;
    switch(origin.size) {
        case 1:
            *value = origin.matrix.at(0);
            return 0;
        case 2:
            *value = origin.matrix.at(0) * origin.matrix.at(3) - origin.matrix.at(1) * origin.matrix.at(2);
            return 0;
        default:
            for (int i = 0; i < origin.size; ++i) {
                Matrix new_matrix = MinorCreation(origin, i, 0);
                if (det(new_matrix, &temp) != 0) {
                    return -1;
                }
                int det_temp = temp;
                *value += degree * origin.matrix.at(i) * det_temp;
                temp = 0;
                degree *= -1;
            }
            break;
    }

    return 0;
}
