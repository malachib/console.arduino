#pragma once

#include <cartesian.h>


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
