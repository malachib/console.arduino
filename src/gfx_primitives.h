#pragma once

// surprising these grid primitive aren't already in any of our GFX libs
// OK the touch screen lib does have its own TSPoint
template <class TPrecision>
struct _Vector
{
  TPrecision  x;
  TPrecision  y;

  template <class TIn>
  _Vector(TIn tin)
  {
    x = tin.x;
    y = tin.y;
  }

  _Vector() {}

  template <class T>
  _Vector<TPrecision>& operator-=(T& in)
  {
    x -= in.x;
    y -= in.y;
    return *this;
  }

  template <class T1, class T2>
  friend _Vector<TPrecision> operator-(T1& v1, T2& v2)
  {
    _Vector<TPrecision> out(v1);

    out -= v2;

    return out;
  }
};

template <class TPrecision>
struct _Vector3D : public _Vector<TPrecision>
{
  TPrecision z;

  template <class TIn>
  _Vector3D(TIn tin) : _Vector<TPrecision>(tin)
  {
    z = tin.z;
  }

  _Vector3D() {}


  _Vector3D<TPrecision>& operator-=(_Vector<TPrecision>& in)
  {
    _Vector<TPrecision>::x -= in.x;
    _Vector<TPrecision>::y -= in.y;

    return *this;
  }

  template <class T>
  _Vector3D<TPrecision>& operator-=(T& in)
  {
    _Vector<TPrecision>::x -= in.x;
    _Vector<TPrecision>::y -= in.y;
    z -= in.z;

    return *this;
  }
};


struct Vector : public _Vector<uint16_t>
{
  template <class TIn>
  Vector(TIn tin) : _Vector<uint16_t>(tin)
  { }

  Vector() {}

  //Vector(_Vector<uint16_t>& in)

  /*
  friend Vector operator-(_Vector<uint16_t>& v1, _Vector<uint16_t>& v2)
  {
    Vector out;

    out.x = v1.x - v2.x;
    out.y = v1.y - v2.y;

    return out;
  } */

  Vector operator - (_Vector<uint16_t>& in)
  {
    Vector out;

    out.x = x - in.x;
    out.y = y - out.y;

    return out;
  }
};


// FIX: this one doesn't seem to get picked up :(
template<class T>
inline Print& operator <<(Print& obj, _Vector<T>& v)
{
  obj.print(v.x);
  obj.print(',');
  obj.print(v.y);
  return obj;
}

/*
inline Print& operator <<(Print& obj, _Vector<uint16_t>& v)
{
  obj.print(v.x);
  obj.print(',');
  obj.print(v.y);
  return obj;
}*/

//template<class T>
inline Print& operator <<(Print& obj, Vector& v)
{
  obj.print(v.x);
  obj.print(',');
  obj.print(v.y);
  return obj;
}

struct Vector3D : public _Vector3D<uint16_t>
{
  template <class TIn>
  Vector3D(TIn tin) : _Vector3D<uint16_t>(tin)
  { }

  Vector3D() {}
};


inline Print& operator <<(Print& obj, Vector3D& v)
{
  obj.print(v.x);
  obj.print(',');
  obj.print(v.y);
  return obj;
}




template <class TPrecision>
struct _Rectangle
{
//protected:
  // where within drawing area this rectangle begins
  _Vector<TPrecision>  origin;
  // horizontal and vertical size of rectangle
  _Vector<TPrecision>  size;

//public:
  _Rectangle() {}
  _Rectangle(_Vector<TPrecision>& origin, _Vector<TPrecision>& size) :
    origin(origin), size(size) {}

  TPrecision getX() { return origin.x; }
  TPrecision getY() { return origin.y; }
  TPrecision getWidth() { return size.x; }
  TPrecision getHeight() { return size.y; }
  TPrecision getBoundX() { return origin.x + size.x; }
  TPrecision getBoundY() { return origin.y + size.y; }
};

struct Rectangle : public _Rectangle<uint16_t>
{
  Rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
  {
    origin.x = x;
    origin.y = y;
    size.x = width;
    size.y = height;
  }

  Rectangle(Vector& origin, Vector& size) : _Rectangle<uint16_t>(origin, size)
  {}

  Rectangle() {}
};

struct Region : public Rectangle
{
  // region code FF is reserved
  uint8_t regionCode;

  Region(uint8_t regionCode, uint16_t x, uint16_t y, uint16_t width, uint16_t height) :
    Rectangle(x, y, width, height), regionCode(regionCode) {}
};
