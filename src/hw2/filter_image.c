#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO
    double sum = 0.0;
    for (int c = 0; c < im.c; c++) {
        for (int i = 0; i < im.w; i++) {
            for (int j = 0; j < im.h; j++) {
                sum += get_pixel(im, i, j, c);
            }
        }
    }
    for (int c = 0; c < im.c; c++) {
        for (int i = 0; i < im.w; i++) {
            for (int j = 0; j < im.h; j++) {
                set_pixel(im, i, j, c, get_pixel(im, i, j, c) / sum);
            }
        }
    }
}

image make_box_filter(int w)
{
    // TODO
    // return make_image(1,1,1);
    image im = make_image(w, w, 1);
    float num = 1.0 / w / w;
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < w; j++) {
            set_pixel(im, i, j, 0, num);
        }
    }
    return im;
}

float calculate_filter_res(image im, int imgc, image filter, int i, int j, int c) {
    float sum = 0;
    for (int fi = 0; fi < filter.w; fi++) {
        for (int fj = 0; fj < filter.h; fj++) {
            int imgi = i + fi - filter.w / 2;
            int imgj = j + fj - filter.h / 2;
            sum += get_pixel(im, imgi, imgj, imgc) *
            get_pixel(filter, fi, fj, c);
        }
    }
    return sum;
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO
    // return make_image(1,1,1);
    image res;
    if (preserve) {
        res = make_image(im.w, im.h, im.c);
    } else {
        res = make_image(im.w, im.h, 1);
    }
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            if (preserve) {
                for (int c = 0; c < im.c; c++) {
                    float n = 0.0;
                    if (filter.c == 1 && im.c > 1) {
                        n = calculate_filter_res(im, c, filter, i, j, 0);
                    } else {
                        n = calculate_filter_res(im, c, filter, i, j, c);
                    }
                    set_pixel(res, i, j, c, n);
                }
            } else {
                float sum = 0;
                for (int c = 0; c < im.c; c++) {
                    float n = 0.0;
                    if (filter.c == 1 && im.c > 1) {
                        n = calculate_filter_res(im, c, filter, i, j, 0);
                    } else {
                        n = calculate_filter_res(im, c, filter, i, j, c);
                    }
                    sum += n;
                }
                set_pixel(res, i, j, 0, sum);
            }
        }
    }
    return res;
}

image make_highpass_filter()
{
    // TODO
    // return make_image(1,1,1);
    image im = make_image(3, 3, 1);
    int num[9] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
    for (int i = 0; i < 3 * 3; i++) {
        im.data[i] = num[i];
    }
    return im;
}

image make_sharpen_filter()
{
    // TODO
    // return make_image(1,1,1);
    image im = make_image(3, 3, 1);
    int num[9] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
    for (int i = 0; i < 3 * 3; i++) {
        im.data[i] = num[i];
    }
    return im;
}

image make_emboss_filter()
{
    // TODO
    // return make_image(1,1,1);
    image im = make_image(3, 3, 1);
    int num[9] = {-2, -1, 0, -1, 1, 1, 0, 1, 2};
    for (int i = 0; i < 3 * 3; i++) {
        im.data[i] = num[i];
    }
    return im;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: Highpass kernel aims to find edges so there is no need to preserve channels. For the other
//  two we need all channels beacuse we need to sharpen or add style to all channels.

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: We need to normalize the values for all of the three filters so each pixel value is betwwen
//  the valid range of 0 and 1

image make_gaussian_filter(float sigma)
{
    // TODO
    // return make_image(1,1,1);
    int w = sigma * 6;
    if (w % 2 == 0) {
        w += 1;
    }
    image filter = make_image(w, w, 1);
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < w; j++) {
            int x = i - w / 2;
            int y = j - w / 2;
            float g = 1 / TWOPI / sigma / sigma * exp(- (pow(x, 2) + pow(y, 2)) / 2 / sigma / sigma);
            filter.data[i * w + j] = g;
        }
    }
    l1_normalize(filter);
    return filter;
}

image add_image(image a, image b)
{
    // TODO
    // return make_image(1,1,1);
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image img = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.w * a.h * a.c; i++) {
        img.data[i] = a.data[i] + b.data[i];
    }
    return img;
}

image sub_image(image a, image b)
{
    // TODO
    // return make_image(1,1,1);
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image img = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.w * a.h * a.c; i++) {
        img.data[i] = a.data[i] - b.data[i];
    }
    return img;
}

image make_gx_filter()
{
    // TODO
    // return make_image(1,1,1);
    image im = make_image(3, 3, 1);
    int num[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    for (int i = 0; i < 3 * 3; i++) {
        im.data[i] = num[i];
    }
    return im;
}

image make_gy_filter()
{
    // TODO
    // return make_image(1,1,1);
    image im = make_image(3, 3, 1);
    int num[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    for (int i = 0; i < 3 * 3; i++) {
        im.data[i] = num[i];
    }
    return im;
}

void feature_normalize(image im)
{
    // TODO
    float minv = im.data[0];
    float maxv = im.data[0];
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            for (int c = 0; c < im.c; c++) {
                minv = fmin(minv, get_pixel(im, i, j, c));
                maxv = fmax(maxv, get_pixel(im, i, j, c));
            }
        }
    }
    float range = maxv - minv;
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            for (int c = 0; c < im.c; c++) {
                if (range == 0) {
                    set_pixel(im, i, j, c, 0);
                } else {
                    set_pixel(im, i, j, c, (get_pixel(im, i, j, c) - minv) / range);
                }
            }
        }
    }
}

image *sobel_image(image im)
{
    // TODO
    // return calloc(2, sizeof(image));
    image *res = calloc(2, sizeof(image));
    image img1 = make_image(im.w, im.h, 1);
    image img2 = make_image(im.w, im.h, 1);
    image gx_filter = make_gx_filter();
    image gy_filter = make_gy_filter();
    image gx_res = convolve_image(im, gx_filter, 0);
    image gy_res = convolve_image(im, gy_filter, 0);
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            float xy = sqrt(pow(get_pixel(gx_res, i, j, 0), 2) + pow(get_pixel(gy_res, i, j, 0), 2));
            float theta = atan2(get_pixel(gy_res, i, j, 0), get_pixel(gx_res, i, j, 0));
            set_pixel(img1, i, j, 0, xy);
            set_pixel(img2, i, j, 0, theta);
        }
    }
    res[0] = img1;
    res[1] = img2;
    free_image(gx_filter);
    free_image(gy_filter);
    free_image(gx_res);
    free_image(gy_res);
    return res;
}

image colorize_sobel(image im)
{
    // TODO
    // return make_image(1,1,1);
    image *img = sobel_image(im);
    image magnitude = img[0];
    image gradient = img[1];
    feature_normalize(magnitude);
    feature_normalize(gradient);
    image ret = make_image(im.w, im.h, 3);
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            set_pixel(ret, i, j, 0, get_pixel(magnitude, i, j, 0));
            set_pixel(ret, i, j, 1, get_pixel(gradient, i, j, 0));
            set_pixel(ret, i, j, 2, get_pixel(gradient, i, j, 0));
        }
    }
    hsv_to_rgb(ret);
    return ret;
}
