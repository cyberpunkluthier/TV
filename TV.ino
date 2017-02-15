
#include <TVout.h>
#include <fontALL.h>

TVout TV;
const uint8_t w = 120;
const uint8_t h = 96;

#include "Polygon.h"
using namespace cyber;


const uint8_t n = 19;
Point<double> center(w/2-1,h/2-1);
Polygon<n,double> poli(h/2,center);
  

void setup() {
    randomSeed(analogRead(0));
    TV.begin(NTSC,w,h);
    TV.select_font(font4x6);
}


void loop() {
    
    TV.clear_screen();
    //TV.set_pixel(center.x(),center.y(),INVERT);

    //poli.debug(TV);
    //poli.draw(TV, INVERT, 4);
    //TV.delay(500);
    poli.a_star(11).draw(TV,INVERT);
    TV.delay(50);
    
    poli.rotate_around_center(PI/12);
    //poli.scale(.7);   
}