#ifndef REGION_H
#define REGION_H

#include "helpers.h"

class Region {
  // rect dimensions for the region
  uint16_t _top;
  uint16_t _bottom; 
  uint16_t _left;
  uint16_t _right;

  // internal margin from each edge for the rect region
  uint16_t _mtop;
  uint16_t _mbottom; 
  uint16_t _mleft;
  uint16_t _mright;

public:
  Region(const uint16_t left, const uint16_t top, 
         const uint16_t right, const uint16_t bottom)
  : _left(left), _top(top), _right(right), _bottom(bottom),
    _mleft(0), _mtop(0), _mright(0), _mbottom(0)
  {}

  // set the size in pixels of the margin to use for drawing in this region
  void setMargin(const uint16_t left, const uint16_t top, 
                const uint16_t right, const uint16_t bottom)
  {
    _mtop = top;
    _mbottom = bottom;
    _mleft = left;
    _mright = right;
  }

  // draw a border around the region
  void drawBoundingRect(uint16_t color); // TODO:, bool filled = false, bool rounded = false);

  // draw a rectangle at the margin of the region
  void drawMarginRect(uint16_t color, bool filled = false, bool rounded = false);

  // draw a triangle in the bounding box
  void drawTriangle(uint16_t color, bool filled=true, bool pointedUp=true);

  // center text in the region and draw it
  void centerText(const char* text, uint16_t textColor, uint8_t* font);

  // print the value of the region
  void printValue(uint16_t value, uint16_t textColor, uint8_t* font);
  
  uint16_t width() const { return _right - _left; }

  uint16_t height() const { return _bottom - _top; }

  Point center() const
  {
    return Point(_left + (_right - _mright - (_left + _mleft))/2,
                _top + (_bottom - _mbottom - (_top + _mtop))/2); 
  }

  bool contains(const Point& p) const;

};



class SelectValueRegion
{
  Region _boundingRegion;
  Region _textHeaderRegion;
  Region _upButtonRegion;
  Region _downButtonRegion;
  Region _numberDisplayRegion;

  // numeric value to set/display in this region
  uint16_t _value;
  uint16_t _increment;
  uint16_t _maxValue;
  uint16_t _minValue; // This is the default value at startup

  bool _active;
  
public:
  SelectValueRegion(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, uint8_t margin,
         const uint16_t increment, const uint16_t maxValue, const uint16_t minValue)
  : _boundingRegion(left, top+(bottom-top)/6, right, bottom), 
    _textHeaderRegion(left+margin, top+margin, right-margin, top+(bottom-top)/6 - margin), 
    _upButtonRegion(left, top+(bottom-top)/6, right, top+(bottom-top)*3/6),
    _downButtonRegion(left, top+(bottom-top)*4/6, right, bottom),
    _numberDisplayRegion(left, top+(bottom-top)*3/6, right, top+(bottom-top)*4/6), _value(minValue),
    _increment(increment), _maxValue(maxValue), _minValue(minValue), _active(false)
  {
    _boundingRegion.setMargin(margin, margin, margin, margin);
    _textHeaderRegion.setMargin(margin/2, margin/2, margin/2, margin/2);
    _upButtonRegion.setMargin(margin, margin, margin, margin);
    _downButtonRegion.setMargin(margin, margin, margin, margin);
    _numberDisplayRegion.setMargin(margin+2, margin+2, margin+2, margin+2);
  }

  void activate() { _active = true; }
  void deactivate() { _active = false; }
  bool isActive() const { return _active; }

  void draw(const char* text, uint16_t boundingColor, uint16_t textBGColor, uint16_t textColor);
  bool check(Point p);
  void printValue(uint16_t textColor, uint8_t* font, uint16_t backColor);
  uint16_t Value() const { return _value; }
};

class ButtonRegion
{
  Region _rectRegion;  // where and how to draw the button
  bool _active;

public:
  ButtonRegion(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, uint8_t margin)
  :_rectRegion(left, top, right, bottom), _active(false)
  {
    _rectRegion.setMargin(margin, margin, margin, margin);
  }
  void activate() { _active = true; }
  void deactivate() { _active = false; }
  bool isActive() const { return _active; }

  void draw(const char* text, uint16_t boundingColor, uint16_t textBGColor, uint16_t textColor);
  bool check(Point p);

};


#endif // REGION_H

