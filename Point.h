// Point.h

#ifndef _POINT_h
#define _POINT_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif


namespace cyber
{

namespace std {
    template<bool Cond, class T = void> struct enable_if {};
    template<class T> struct enable_if<true, T> { typedef T type; };
}
    
template<typename T=float, uint8_t n_dim=2>
class Point
{
protected:
    T coordinates[n_dim];

public:
    typedef T value_type;
    static const uint8_t number_of_dimensions = n_dim;


    Point(T value=T()) {
        set_coordinates(value);
    }
    
    Point(const Point& other) {
        set_coordinates(other.get_coordinates());
    }
    
    template <typename T2>
    Point(const Point<T2,n_dim>& other) {
        set_coordinates(other.get_coordinates());
    }
    
    template <typename T2>
    Point(const T2* values) {
        set_coordinates(values);
    }
    
    Point(T x, T y)
    {
        set_x(x);
        set_y(y);
    }
    Point(T x, T y, T z)
    {
        set_x(x);
        set_y(y);
        set_z(z);
    }
    
    
    void set_coordinates(T value=T()) {
        for(uint8_t i=0; i<n_dim; ++i)
        coordinates[i] = value;
    }
    
    template <typename T2>
    void set_coordinates(const T2* values) {
        for(uint8_t i=0; i<n_dim; ++i) {
            coordinates[i] = static_cast<T>(values[i]);
        }
    }
        
    const T* get_coordinates() const {
        return coordinates; // this is an inline function, no overload
    }
    
    template <typename T2>
    void set_coordinate(uint8_t i, T2 v) {
        coordinates[i] = static_cast<T>(v);
    }
    
    Point& operator=(const Point &p) {
        set_coordinates(p.get_coordinates());
        return *this;
    }        

//     template <typename T2>
//     Point& operator=(const Point<T2,n_dim> &p) {
//         set_coordinates(p.get_coordinates());
//         return *this;
//     }

    template <typename T2>
    Point& operator+=(const Point<T2,n_dim> &p) {
        for(uint8_t i=0; i<n_dim; ++i)
        coordinates[i] += static_cast<T>(p.get_coordinates()[i]);
        
        return *this;
    }
    
    template <typename T2>
    Point& operator-=(const Point<T2,n_dim> &p) {
        for(uint8_t i=0; i<n_dim; ++i)
            coordinates[i] -= static_cast<T>(p.get_coordinates()[i]);
        
        return *this;
    }    
    
    // template <typename T2> // to implement this
    // we would need a way to derive the correct return type
    // for now we can not add e.g. float and double points this way
    Point operator+(const Point &p) const {
        return Point(*this) += p; // create new object and add
    }
    Point operator-(const Point &p) const {
        return Point(*this) -= p;
    }    
    
    Point operator-() const {
        Point r(*this);
        for(uint8_t i=0; i<n_dim; ++i)
            r.coordinates[i] = -r.coordinates[i];
        
        return r;
    }


    T x() const {
        return get_coordinates()[0];
    }
    //template <class = typename std::enable_if<n_dim==2>::type>
    template <typename T2>
    void set_x(T2 v) {
        set_coordinate(0,v);
    }
    T y() const {
        return get_coordinates()[1];
    }
    template <typename T2>
    void set_y(T2 v) {
        set_coordinate(1,v);
    }
    T z() const {
        return get_coordinates()[2];
    }
    template <typename T2>
    void set_z(T2 v) {
        set_coordinate(0,v);
    }
};

} // cyber
#endif

