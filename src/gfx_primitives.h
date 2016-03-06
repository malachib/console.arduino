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
};

template <class TPrecision>
struct _Vector3D : public _Vector<TPrecision>
{
  TPrecision z;

  template <class TIn>
  _Vector3D(TIn tin) : _Vector<TPrecision>(tin)
  {
    //x = tin.x;
    //y = tin.y;
    z = tin.z;
  }

  _Vector3D() {}
};

template <class TPrecision>
struct _Rectangle
{
protected:
  // where within drawing area this rectangle begins
  _Vector<TPrecision>  origin;
  // horizontal and vertical size of rectangle
  _Vector<TPrecision>  size;

public:
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
};

struct Region : public Rectangle
{
  // region code FF is reserved
  uint8_t regionCode;

  Region(uint8_t regionCode, uint16_t x, uint16_t y, uint16_t width, uint16_t height) :
    Rectangle(x, y, width, height), regionCode(regionCode) {}
};
