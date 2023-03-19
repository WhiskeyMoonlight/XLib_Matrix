#ifndef RK1_DETGRAPHICS_H
#define RK1_DETGRAPHICS_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/XKBlib.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

#define numWidth 40						//Ширина ячейки
#define numHeight 1.2*numWidth				//Высота ячейки
#define mainWidth numWidth * 3				//Ширина главного окна
#define mainHeight numHeight * 2	//Высота главного окна



struct Matrix {
    int size = 0;
    std::vector<int> matrix;
    Matrix(int _size) {
        size = _size;
        matrix.resize(size*size);
        for (auto i : matrix) {
            matrix.at(i) = 0;
        }
    }
    void deleteMatrix() {
        matrix.clear();
    }
};

Matrix MinorCreation(const Matrix &to_copy, int col_to_kick, int row_to_kick);

int det(const Matrix& origin, int* value);

GC CreateContext(Display*);
Window CreateMainWindow(Display*, int);
void reverse(Display*, const std::vector<Window>&, GC, Matrix&);
void change(Display*, Window, GC, int, Matrix&);
void draw(Display*, const std::vector<Window>&, GC);
Window createDet(Display*, Window, int);
void drawDet(Display*, Window, GC, int, int);
void nullify(Display*, const std::vector<Window>&, GC, Matrix&);
Window CreateMatrixWindow(Display*, Window, int, int);
int rekey(XEvent*, GC, const std::vector<Window>&, Matrix&, Window);


#endif //RK1_DETGRAPHICS_H
