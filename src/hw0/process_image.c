#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    if (x < 0) {
        x = 0;
    }
    if (x >= im.w) {
        x = im.w - 1;
    }
    if (y < 0) {
        y = 0;
    }
    if (y >= im.h) {
        y = im.h - 1;
    }
    if (c < 0) {
        c = 0;
    }
    if (c >= im.c) {
        c = im.c - 1;
    }
    return im.data[x + y * im.w + c * im.w * im.h];
    // return 0;
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    if (x < 0) {
        return;
    }
    if (x >= im.w) {
        return;
    }
    if (y < 0) {
        return;
    }
    if (y >= im.h) {
        return;
    }
    if (c < 0) {
        return;
    }
    if (c >= im.c) {
        return;
    }
    im.data[x + y * im.w + c * im.w * im.h] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, sizeof(float)*im.w * im.h * im.c);
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            gray.data[i + j * im.w] = 0.299 * im.data[i + j * im.w + 0] +
                0.587 * im.data[i + j * im.w + im.h * im.w] +
                0.114 * im.data[i + j * im.w + im.h * im.w * 2];
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            im.data[i + j * im.w + im.h * im.w * c] += v;
        }
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            for (int k = 0; k < im.c; k++) {
                if (im.data[i + j * im.w + im.h * im.w * k] < 0.0) {
                    im.data[i + j * im.w + im.h * im.w * k] = 0;
                }
                if (im.data[i + j * im.w + im.h * im.w * k] > 1.0) {
                    im.data[i + j * im.w + im.h * im.w * k] = 1;
                }
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            float R = im.data[i + j * im.w];
            float G = im.data[i + j * im.w + im.h * im.w];
            float B = im.data[i + j * im.w + im.h * im.w * 2];
            float V = three_way_max(R, G, B);
            float m = three_way_min(R, G, B);
            float C = V - m;
            float S = 0;
            if (V != 0) {
                S = C / V;
            }
            float H = 0;
            if (C == 0) {
                H = 0;
            } else if (V == R) {
                H = (G - B) / C;
            } else if (V == G) {
                H = (B - R) / C + 2;
            } else if (V == B) {
                H = (R - G) / C + 4;
            }
            if (H < 0) {
                H = H / 6 + 1;
            } else {
                H = H / 6;
            }
            im.data[i + j * im.w] = H;
            im.data[i + j * im.w + im.h * im.w] = S;
            im.data[i + j * im.w + im.h * im.w * 2] = V;
        }
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            double r, g, b, p, q, t, x;
            float h = im.data[i + j * im.w];
            float s = im.data[i + j * im.w + im.h * im.w];
            float v = im.data[i + j * im.w + im.h * im.w * 2];
            if (s <= 0.0) {
                r = v;
                g = v;
                b = v;
            } else {
                int hi = (int)(h * 6);
                x = h * 6 - hi;
                p = v * (1.0 - s);
                q = v * (1.0 - (s * x));
                t = v * (1.0 - (s * (1.0 - x)));
                if (0 == hi && hi < 1) {
                    r = v;
                    g = t;
                    b = p;
                } else if (1 == hi && hi < 2) {
                    r = q;
                    g = v;
                    b = p;
                } else if (2 <= hi && hi < 3) {
                    r = p;
                    g = v;
                    b = t;
                } else if (3 <= hi && hi < 4) {
                    r = p;
                    g = q;
                    b = v;
                } else if (4 <= hi && hi < 5) {
                    r = t;
                    g = p;
                    b = v;
                } else {
                    r = v;
                    g = p;
                    b = q;
                }
            }
            im.data[i + j * im.w] = r;
            im.data[i + j * im.w + im.h * im.w] = g;
            im.data[i + j * im.w + im.h * im.w * 2] = b;
        }
    }
}

void scale_image(image im, int c, float v) {
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            im.data[i + j * im.w + im.h * im.w * c] *= v;
        }
    }
}