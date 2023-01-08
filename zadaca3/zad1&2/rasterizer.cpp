#include <iostream>
#include <cmath>
#include <algorithm>
#include "tgaimage.h"
#include "tgaimage.cpp"

using namespace std;

// dimenzije slike
const int width  = 512;
const int height = 512;

// definirajmo boje
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0, 0, 255);
const TGAColor blue  = TGAColor(0, 0, 255, 255);
const TGAColor green  = TGAColor(0, 255, 0, 255);

void set_color(int x, int y, TGAImage &image, TGAColor color, bool invert = false)
{
    image.set(y, x, color);    
}

float line(float x0, float y0, float x1, float y1, float x, float y)
{
    return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
}

void line_naive(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    for (float t = 0; t < 1; t += 0.01)
    {
        int x = x0 * (1.0f - t) + x1 * t;
        int y = x0 * (1.0f - t) + y1 * t;
        set_color(x, y, image, color);
    }
}

void line_midpoint(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color, bool invert)
{
    int y = y0;
    int d = line(x0, y0, x1, y1, x0 + 1, y0 + 0.5);
    
    int dx = (x1 - x0);
    int dy = (y1 - y0);
    int increment = 1;
    
    if (dy < 0)
    {
        // pravac ide od gore prema dolje
        dy = -dy;
        increment = -increment;
    }
    
    for (int x = x0; x <= x1; ++x)
    {
        if (invert)
        {
            set_color(x, y, image, color);
        }
        else
        {
            set_color(y, x, image, color);       
        }
        
        if (d < 0)
        {
            y = y + increment;
            d = d + dx - dy;
        }
        else
        {
            d = d - dy;
        }
    }
}

void draw_line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    // 'transponiraj' duzinu ako je veci od 1
    bool invert = false;
    if (abs(x1 - x0) < abs(y1 - y0))
    {
        swap(x0, y0);
        swap(x1, y1);
        invert = true;
    }
    
    // zamijeni tocke ako pravac ide s desna na lijevo
    if (x1 < x0)
    {
        swap(x0, x1);
        swap(y0, y1);
    }
    
    // nacrtaj duzinu
    line_midpoint(x0, y0, x1, y1, image, color, invert);
}


float form(float x0, float y0, float x1, float y1, float x, float y){
    float f = ((y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0);
    return f;
}

void draw_triangle_2d(TGAImage& image, float x0, float y0, float x1, float y1, float x2, float y2, TGAColor color)
{
    int xmin = floor(min(x0, min(x1, x2)));
    int xmax = ceil(max(x0, max(x1, x2)));
    int ymin = floor(min(y0, min(y1, y2)));
    int ymax = ceil(max(y0, max(y1, y2)));

    for (int y = ymin; y <= ymax; y++)
    {
        for (int x = xmin; x <= xmax; x++)
        {
            float a = form(x1, y1, x2, y2, x, y)/form(x1, y1, x2, y2, x0, y0);
            float b = form(x2, y2, x0, y0, x, y)/form(x2, y2, x0, y0, x1, y1);
            float g = form(x0, y0, x1, y1, x, y)/form(x0, y0, x1, y1, x2, y2);

            if((a >= 0 && a <=1) && (b >= 0 && b <=1) && (g >= 0 && g <=1))
            {
                if((a > 0 || (form(x1, y1, x2, y2, x, y)*form(x1, y1, x2, y2, -1, -1)) >= 0) &&
                    (b > 0 || (form(x2, y2, x0, y0, x, y)*form(x2, y2, x0, y0, -1, -1)) >= 0) &&
                    (g > 0 || (form(x0, y0, x1, y1, x, y)*form(x0, y0, x1, y1, -1, -1)) >= 0))
                {
                    image.set(x, y, color);
                }
            }

        }        
    }
}


void draw_triangle_2d_gouraurd(TGAImage& image, float x0, float y0, float x1, float y1, float x2, float y2, TGAColor c0, TGAColor c1, TGAColor c2)
{
    int xmin = floor(min(x0, min(x1, x2)));
    int xmax = ceil(max(x0, max(x1, x2)));
    int ymin = floor(min(y0, min(y1, y2)));
    int ymax = ceil(max(y0, max(y1, y2)));
    TGAColor color;

    for (int y = ymin; y <= ymax; y++)
    {
        for (int x = xmin; x <= xmax; x++)
        {
            float al = form(x1, y1, x2, y2, x, y)/form(x1, y1, x2, y2, x0, y0);
            float be = form(x2, y2, x0, y0, x, y)/form(x2, y2, x0, y0, x1, y1);
            float ga = form(x0, y0, x1, y1, x, y)/form(x0, y0, x1, y1, x2, y2);

            if((al >= 0 && al <=1) && (be >= 0 && be <=1) && (ga >= 0 && ga <=1))
            {
                if((al > 0 || (form(x1, y1, x2, y2, x, y)*form(x1, y1, x2, y2, -1, -1)) >= 0) &&
                    (be > 0 || (form(x2, y2, x0, y0, x, y)*form(x2, y2, x0, y0, -1, -1)) >= 0) &&
                    (ga > 0 || (form(x0, y0, x1, y1, x, y)*form(x0, y0, x1, y1, -1, -1)) >= 0))
                {
                    color.r = al * c0.r + be * c1.r + ga * c2.r;
                    color.g = al * c0.g + be * c1.g + ga * c2.g;
                    color.b = al * c0.b + be * c1.b + ga * c2.b;
                    image.set(x, y, color);
                }
            }

        }        
    }
}


int main()
{
    // definiraj sliku
    TGAImage image(width, height, TGAImage::RGB);
    
    draw_triangle_2d(image, 100, 350, 300, 400, 300, 300, red);
    draw_triangle_2d(image, 19, 18, 50, 150, 140, 140, green);
    draw_triangle_2d_gouraurd(image, 100, 248, 290, 86, 430, 248, red, green, blue);
    
    
    // spremi sliku 
    image.flip_vertically();
    image.write_tga_file("trokuti.tga");
}