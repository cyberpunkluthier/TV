// Polygon.h

#ifndef _POLYGON_h
#define _POLYGON_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Point.h"

namespace cyber
{

template<uint8_t n_vert, typename T=float>
class Polygon
{
public:
    typedef Point<T,2> PointT;
    static const uint8_t n_vertexes = n_vert;
    
protected:
    PointT vertexes[n_vertexes];
    uint8_t indexes[n_vertexes];
    PointT offset; // coordinates of center
    T r;
    // double angle; // of the first vertex
    
public:
    Polygon(T r = 1., PointT offset = PointT()): r(r), offset(offset) {
        calc_vertexes();
    }
    
    Polygon(const Polygon& other): r(other.r), offset(other.offset) {
        set_vertexes(other.get_vertexes(), other.get_indexes());
    }
    
    template<class T2>
    Polygon(const Polygon<n_vert,T2>& other): r(other.get_r()), offset(other.get_offset()) {
        set_vertexes(other.get_vertexes(), other.get_indexes());
    }
    
    const PointT& get_offset() const {
        return offset;
    }

    uint8_t get_r() const {
        return r;
    }

    const PointT* get_vertexes() const {
        return vertexes;
    }
    
    const uint8_t* get_indexes() const {
        return indexes;
    }
    
    template<class PointT2>
    void set_vertexes(const PointT2* new_vertexes) {
        for(uint8_t i=0; i<n_vertexes; ++i)
            vertexes[i] = new_vertexes[i];
    }
    template<class PointT2>
    void set_vertexes(const PointT2* new_vertexes, const uint8_t* new_indexes) {
        for(uint8_t i=0; i<n_vertexes; ++i) {
            vertexes[i] = new_vertexes[i];
            indexes[i] = new_indexes[i];
        }
    }        


    void calc_vertexes() {
        double angle_pizza = 2*PI/n_vertexes;
        for(uint8_t i=0; i<n_vertexes; ++i) {
            vertexes[i].set_x(r*cos(angle_pizza*i) + offset.x());
            vertexes[i].set_y(r*sin(angle_pizza*i) + offset.y());
            indexes[i]=i;
        }   
    }
    

    Polygon& operator=(const Polygon& other) {
        offset = other.offset;
        r = other.r;
        set_vertexes(other.get_vertexes(), other.get_indexes());
    }
    
    void rotate(float angle) {
        // https://en.wikipedia.org/wiki/Rotation_matrix
        double x;
        double y;
        for(uint8_t i=0; i<n_vertexes; ++i) {
            x = vertexes[i].x()*cos(angle) - vertexes[i].y()*sin(angle);
            y = vertexes[i].x()*sin(angle) + vertexes[i].y()*cos(angle);
            vertexes[i].set_x(x);
            vertexes[i].set_y(y);
        }
    }

    void rotate_around_center(float angle) {
        translate(-offset, false);
        rotate(angle);
        translate(offset, false);
    }

    void translate(const PointT& off, bool update_offset=true) {
        for(uint8_t i=0; i<n_vertexes; ++i) {
            vertexes[i] += off;
        }
        if(update_offset)
            offset += off;
    }

    void scale(float factor) {
        translate(-offset, false);
        for(uint8_t i=0; i<n_vertexes; ++i) {
            vertexes[i].set_x(vertexes[i].x() * factor);
            vertexes[i].set_y(vertexes[i].y() * factor);
        }
        translate(offset, false);
    }
    
    template <class TVoutT>
    void draw(TVoutT& tv, uint8_t color, uint8_t delayy=0) {
        for(uint8_t i=0; i<n_vertexes; ++i) {
            tv.draw_line((uint8_t) vertexes[indexes[i]].x(),
            (uint8_t) vertexes[indexes[i]].y(),
            (uint8_t) vertexes[indexes[(i+1)%n_vertexes]].x(),
            (uint8_t) vertexes[indexes[(i+1)%n_vertexes]].y(),
            color);
            
            if(delayy>0)
              tv.delay(delayy);
        }
    }

    template <class TVoutT>
    void debug(TVoutT& tv) {
        for(uint8_t i=0; i<n_vertexes; ++i) {
            tv.print((int) indexes[i]); tv.print("___");
            tv.print(vertexes[indexes[i]].x()); tv.print("___");
            tv.println(vertexes[indexes[i]].y());
        }
    }

    template <class TVoutT>
    void debug2(TVoutT& tv) {
        for(uint8_t i=0; i<n_vertexes; ++i) {
            tv.print((int) indexes[i]); tv.print("___");
            tv.print(vertexes[i].x()); tv.print("___");
            tv.println(vertexes[i].y());
        }
    }


    class Star: public Polygon {
    protected:
        static const uint8_t p = n_vertexes;
        uint8_t q;
    
        void make_star_indexes(){
            uint8_t polindexes[p];
            for(uint8_t i=0; i<p; ++i) {
                polindexes[i] = indexes[i]; // copy all
            }
            for(uint8_t i=0; i<p; ++i) {
                indexes[i] = polindexes[(i*q)%p];
            }
        }

    public:
        Star(const Polygon& poli, uint8_t q=2): Polygon(poli), q(q) {
            make_star_indexes();
        }
    };

    // A regular star polygon is denoted by its Schläfli symbol {p/q},
    // where p and q are relatively prime (they share no factors) and q ? 2.
    // https://en.wikipedia.org/wiki/Star_polygon
    Star a_star(uint8_t q=2) {
        return(Star(*this, q));
    }
};




} // cyber
#endif

