#include <TVout.h>
#include <fontALL.h>

TVout TV;

const uint8_t w = 120;
const uint8_t h = 96;


/*
// http://mathworld.wolfram.com/Pentagram.html
const double a = 0.381966;
const double b = 0.236068;
const double r = 0.16246;
const double R = 0.200811;
const double rho = 0.525731;
const double x = 0.309017;
const double y = 0.224514;
*/


struct Point {
  double x;
  double y;
  Point(double x=0., double y=0.): x(x), y(y) {}

  Point operator-() const {
    return Point(-x, -y);
  }
  
  Point operator+(const Point &p) const {
    return Point(x+p.x, y+p.y);
  }
  
  Point operator-(const Point &p) const {
    return(*this - p);
  }

  Point& operator=(const Point &p) {
    x = p.x;
    y = p.y;
    return *this;
  }
};


// http://en.cppreference.com/w/cpp/language/template_parameters
template<uint8_t n>
struct Polygon {
  Point points[n];
  uint8_t indexes[n];
  Point offset;
  
  Polygon(double r = 1., Point offset = Point()): offset(offset) {
    double angle = 2*PI/n;
    for(uint8_t i=0; i<n; ++i) {
      points[i].x = r*cos(angle*i) + offset.x;
      points[i].y = r*sin(angle*i) + offset.y;
      indexes[i]=i;
    } 
  }
  
  Polygon(const Polygon& other) {
    *this = other;
  }

  Polygon& operator=(const Polygon& other) {
    offset = other.offset;
    for(uint8_t i=0; i<n; ++i) {
      points[i] = other.points[i];
      indexes[i] = other.indexes[i];
    }
  }
  
  void rotate(float angle) {
    // https://en.wikipedia.org/wiki/Rotation_matrix
    double x;
    double y;
    for(uint8_t i=0; i<n; ++i) {
      x = points[i].x*cos(angle) - points[i].y*sin(angle);
      y = points[i].x*sin(angle) + points[i].y*cos(angle);
      points[i].x = x;
      points[i].y = y;
    } 
  }

  void rotate_around_center(float angle) {
    translate(-offset);
    rotate(angle);
    translate(offset);
  }

  void translate(Point off) {
    for(uint8_t i=0; i<n; ++i) {
      points[i].x += off.x;
      points[i].y += off.y;
    }
  }

  void scale(float factor) {
    translate(-offset);
    for(uint8_t i=0; i<n; ++i) {
      points[i].x *= factor;
      points[i].y *= factor;
    }
    translate(offset);
  }
  
  void draw(TVout& TV, uint8_t color=WHITE, uint8_t delayy=0) {
    for(uint8_t i=0; i<n; ++i) {
      TV.draw_line((uint8_t) points[indexes[i]].x, 
                   (uint8_t) points[indexes[i]].y,
                   (uint8_t) points[indexes[(i+1)%n]].x,
                   (uint8_t) points[indexes[(i+1)%n]].y,
                   color);
                   
      if(delayy>0)
        TV.delay(delayy);
    }
  }

  void debug(TVout& TV) {
    for(uint8_t* i=indexes; i<indexes+n; ++i) {
      TV.print((int) *i); TV.print("___");
      TV.print(points[*i].x); TV.print("___");
      TV.println(points[*i].y);
    }
  }

  void debug2(TVout& TV) {
    for(uint8_t i=0; i<n; ++i) {
      TV.print((int) indexes[i]); TV.print("___");
      TV.print(points[i].x); TV.print("___");
      TV.println(points[i].y);
    }
  }

  struct Star;

  // A regular star polygon is denoted by its Schläfli symbol {p/q},
  // where p and q are relatively prime (they share no factors) and q ? 2.
  // https://en.wikipedia.org/wiki/Star_polygon
  Star a_star(uint8_t q=2) {
    return(Star(*this, q));
  }
};

template<uint8_t p>
struct Polygon<p>::Star: public Polygon<p> {
  uint8_t q;
  Star(const Polygon<p>& poli, uint8_t q=2): Polygon<p>(poli), q(q) {
    make_star_indexes();
  }
  void make_star_indexes(){
    uint8_t polindexes[p];
    for(uint8_t i=0; i<p; ++i) {
      polindexes[i] = indexes[i]; // copy all
    }
    for(uint8_t i=0; i<p; ++i) {
      indexes[i] = polindexes[(i*q)%p];
    }
  }
};


const uint8_t n = 17;
Point center = Point(w/2,h/2);
Polygon<n> poli(h/2,center);
//Polygon<n> poli(100.);

  
void setup() {
  TV.begin(NTSC,w,h);
  TV.select_font(font6x8);
//  randomSeed(analogRead(0));

  //poli.draw(TV);
  //poli.debug(TV);
  //TV.delay(2000);
  //TV.clear_screen();

/*
  float div[9] = {2,3,5,7,9,11,13,17,19};
  for(uint8_t i = 0; i<3; ++i) {
    poli = Polygon<n>(h/div[i], center);
    poli.rotate_around_center(PI/2.);
    poli.a_star(5).draw(TV);
  }
  */
}


void loop() {
  TV.clear_screen();
  poli.a_star(11).draw(TV,WHITE, 4);
  Polygon<n> p = poli;
  p.scale(.44);
  p.a_star(11).draw(TV,WHITE, 4);
  p.scale(.44);
  p.a_star(11).draw(TV,WHITE, 4);
  p.scale(.44);
  p.a_star(11).draw(TV,WHITE, 4);
  poli.rotate_around_center(-PI/7);
  TV.delay(44);
}