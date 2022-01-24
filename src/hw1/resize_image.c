#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    // return 0;
    return get_pixel(im, roundf(x), roundf(y), c);
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    // return make_image(1,1,1);
    image new_img = make_image(w, h, im.c);
    float ax = (float)im.w / w;
    float bx = -0.5 + 0.5 * ax;
    float ay = (float)im.h / h;
    float by = -0.5 + 0.5 * ay;
    for (int i = 0; i < im.c; i++) {
        for (int j = 0; j < w; j++) {
            for (int k = 0; k < h; k++) {
                float x = (float)ax * j + bx;
                float y = (float)ay * k + by;
                set_pixel(new_img, j, k, i, nn_interpolate(im, x, y, i));
            }
        }
    }
    return new_img;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    // return 0;
    float v1 = get_pixel(im, floorf(x), floorf(y), c);
    float v2 = get_pixel(im, ceilf(x), floorf(y), c);
    float v3 = get_pixel(im, floorf(x), ceilf(y), c);
    float v4 = get_pixel(im, ceilf(x), ceilf(y), c);
    float q1 = v2 * (x - floorf(x)) + v1 * (ceilf(x) - x);
    float q2 = v4 * (x - floorf(x)) + v3 * (ceilf(x) - x);
    float q = q2 * (y - floorf(y)) + q1 * (ceilf(y) - y);
    return q;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    // return make_image(1,1,1);
    image new_img = make_image(w, h, im.c);
    float ax = (float)im.w / w;
    float bx = -0.5 + 0.5 * ax;
    float ay = (float)im.h / h;
    float by = -0.5 + 0.5 * ay;
    for (int i = 0; i < im.c; i++) {
        for (int j = 0; j < w; j++) {
            for (int k = 0; k < h; k++) {
                float x = (float)ax * j + bx;
                float y = (float)ay * k + by;
                set_pixel(new_img, j, k, i, bilinear_interpolate(im, x, y, i));
            }
        }
    }
    return new_img;
}

