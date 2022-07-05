/* very worthwhile exercise, advancing step by step horizontally
 * and then by powers of two vertically. In this way a binomial pattern can be drawn with cairo
 * it's critical to see how those forloop indices are built up */
#include <cairo/cairo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include<stdint.h>
#include<string.h>
#include<sys/stat.h>

#define HBARNUMS 21
#define VBARNUMS HBARNUMS
#define SEED 112
#define CHUNKS16 200
#define MINX -5
#define MINY -5
#define MAXX 5
#define MAXY 5

typedef struct
{
    int x, y;
} pos_t;

int rfill16(pos_t *p, int *ind, int minx, int miny, int maxx, int maxy)
{
    unsigned x;
    int j, jj; 
    int i=*ind;
    int rn=rand();
    unsigned char firsttime=(i==0)?1:0;
    int startj=(firsttime)?1:0;
    for(j=startj;j<16;j++) {
        jj=j*2;
        x=(rn>>jj)&0x03;
        switch(x) {
            case 0x00:
                p[i+j].x = p[i+j-1].x+1; p[i+j].y = p[i+j-1].y; break; // horizontal right move only, vert same as previous
            case 0x01:
                p[i+j].y = p[i+j-1].y-1; p[i+j].x = p[i+j-1].x; break; // vertical down move only
            case 0x02:
                p[i+j].x = p[i+j-1].x-1; p[i+j].y = p[i+j-1].y; break; // horizon left move
            case 0x03:
                p[i+j].y = p[i+j-1].y+1; p[i+j].x = p[i+j-1].x; break; // vert up move
        }
        printf("step%i:(%i,%i)\n", i+j, p[i+j].x, p[i+j].y);
        if((p[i+j].x == minx) | (p[i+j].y == miny) | (p[i+j].x == maxx) | (p[i+j].y == maxy)) {
            *ind += j+1;
            return 1;
        }
    }
    *ind+=15; // next index
    return 0;
}

int main (int argc, char *argv[])
{
    int i, j, width=800, height=800;
    srand(SEED);
    cairo_surface_t *surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create (surface);

    /*  first we're going to set the background */
    cairo_rectangle (cr, 0, 0, width, height); /* arg explan: topleftcorner and size of shape  */
    cairo_set_source_rgba (cr, 0, 0, 0, 0.95); /*  final number is alpha, 1.0 is opaque */
    cairo_fill (cr);

    /* vertical divider how ar ewe going to section off the screen vertically */
    float vbar=(float)width/VBARNUMS; /* answer is: equally */
    float hbar=(float)height/HBARNUMS; /* answer is: equally */
    pos_t *lb=calloc(HBARNUMS*VBARNUMS, sizeof(pos_t)); /* ori: origin, lb, last bar, */

    cairo_set_source_rgb(cr, 0.65, 0.8, 0.45);
    for(i=0; i<VBARNUMS;i++) { 
        for(j=0; j<HBARNUMS;j++) { 
            lb[i*HBARNUMS+j].x= vbar*i + vbar/2.;
            lb[i*HBARNUMS+j].y= hbar*j + hbar/2.;
            cairo_arc(cr, lb[i*HBARNUMS+j].x, lb[i*HBARNUMS+j].y, 1.0, 0, 2 * M_PI);
            cairo_fill(cr);
        }
    }
    printf("\n"); 

    pos_t *pos=calloc(1, sizeof(pos_t));
    int ind=0;

    int minx=MINX, miny=MINY, maxx=MAXX, maxy=MAXY, ret;
    ret=0;
    i=0;
    while((!ret) & (i<CHUNKS16)) {
        // printf("ind=%i\n", ind); 
        pos=realloc(pos, (ind+16)*sizeof(pos_t));
        ret=rfill16(pos, &ind, minx, miny, maxx, maxy);
        printf("ret=%i\n", ret);
        i++;
    }
    printf("round=%i;ind=%i:(%i,%i)\n", i, ind-1, pos[ind-1].x, pos[ind-1].y);
    int centy=HBARNUMS/2;
    int centx=VBARNUMS/2;
    cairo_set_line_width(cr, 1.0);
    cairo_set_source_rgb(cr, 0.55, 0.5, 0.85);
    // x -axis
    cairo_move_to(cr, lb[centy].x, lb[centy].y);
    cairo_line_to(cr, lb[HBARNUMS*(VBARNUMS-1) +centy].x, lb[HBARNUMS*(VBARNUMS-1) +centy].y);
    cairo_stroke(cr);
    // y -axis
    cairo_move_to(cr, lb[HBARNUMS*centx].x, lb[HBARNUMS*centx].y);
    cairo_line_to(cr, lb[HBARNUMS*centx +HBARNUMS-1].x, lb[HBARNUMS*centx +HBARNUMS-1].y);
    cairo_stroke(cr);
    cairo_set_line_width(cr, 1.0);
    cairo_move_to(cr, lb[HBARNUMS*(pos[0].x+centx) + pos[0].y+centy].x, lb[HBARNUMS*(pos[0].x+centx) + pos[0].y+centy].y);
    cairo_set_source_rgb(cr, 0.65, 0, 0.15);
    int gridx, gridy; // x and y for our grid, esy to overlook as Cairo top left is (0,0) and we want it in middle with positive y's going up, i.e. back, so therefore negativising.
    for(i=1;i<ind;++i) {
        gridx = centx+pos[i].x;
        gridy = centy-pos[i].y;
        cairo_line_to(cr, lb[HBARNUMS*gridx + gridy].x, lb[HBARNUMS*gridx + gridy].y);
        cairo_stroke(cr);
        cairo_move_to(cr, lb[HBARNUMS*gridx + gridy].x, lb[HBARNUMS*gridx + gridy].y);
    }
    // printf("\n"); 

    /* Write output and clean up */
    cairo_surface_write_to_png (surface, "ra0.png");
    free(pos);
    free(lb);
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

    return 0;
}
