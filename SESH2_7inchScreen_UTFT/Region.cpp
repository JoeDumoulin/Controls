#include <stdlib.h>
#include <string.h>
#include <UTFT.h>
#include <UTFT_Geometry.h>
#include <URTouch.h>
#include "Region.h"

extern UTFT glcd;
extern UTFT_Geometry geo;
extern URTouch touch;
extern uint8_t SmallFont[];
extern uint8_t BigFont[];

void Region::drawBoundingRect(uint16_t color)
{
  glcd.setColor(color);
  glcd.drawRect(_left, _top, _right, _bottom);
}

void Region::drawMarginRect(uint16_t color, bool filled, bool rounded)
{
  glcd.setColor(color);
  if (filled && rounded)
  {
    glcd.fillRoundRect(_left+_mleft, _top+_mtop, _right-_mright, _bottom-_mbottom);
  }
  else if (filled)
  {
    glcd.fillRect(_left+_mleft, _top+_mtop, _right-_mright, _bottom-_mbottom);
  }
  else if (rounded)
  {
    glcd.drawRoundRect(_left+_mleft, _top+_mtop, _right-_mright, _bottom-_mbottom);  
  }
  else
  {
    glcd.drawRect(_left+_mleft, _top+_mtop, _right-_mright, _bottom-_mbottom);    
  }
  // debug
//  Serial.print("Rect{"); Serial.print(_left); Serial.print(", "); Serial.print(_top); Serial.print(", ");
//  Serial.print(_right); Serial.print(", "); Serial.print(_bottom); Serial.println("}");
//  Serial.print("name= ");Serial.println(text);

}

void Region::drawTriangle(uint16_t color, bool filled, bool pointedUp)
{
  glcd.setColor(color);
  if (filled && pointedUp)
  {
      geo.fillTriangle(_left+_mleft, _bottom-_mbottom, _right-_mright, _bottom-_mbottom
                  , _left+_mleft + (_right-_mright - (_left+_mleft))/2, _top+_mtop);
  }
  else if (filled) // pointed down
  {
      geo.fillTriangle(_left+_mleft, _top+_mtop, _right-_mright, _top+_mtop
                  , _left+_mleft + (_right-_mright - (_left+_mleft))/2, _bottom-_mbottom);
  }
  else if (pointedUp)  
  {
      geo.drawTriangle(_left+_mleft, _bottom-_mbottom, _right-_mright, _bottom-_mbottom
                  , _left+_mleft + (_right-_mright - (_left+_mleft))/2, _top+_mtop);
  }
  else // draw outline pointed down
  {
      geo.drawTriangle(_left+_mleft, _bottom-_mbottom, _right-_mright, _bottom-_mbottom
                  , _left+_mleft + (_right-_mright - (_left+_mleft))/2, _top+_mtop);
  }
}

// Center text in a region (center in margins)
void Region::centerText(const char* text, uint16_t textColor, uint8_t* font)
{
  Serial.println("Region::centerText called");
  // Get the center point of the bounding rect with margins
  Point c = center();

  // set the font and get parameters
  glcd.setFont(font);
  uint8_t textHeight = glcd.getFontYsize();
  uint8_t charWidth = glcd.getFontXsize();

  uint16_t len = strlen(text) * charWidth;

  // print in the text color and then resotre the system color.
  uint16_t saveColor = glcd.getColor();
//  glcd.setBackColor(saveColor); // sometimes...
  glcd.setColor(textColor);
  glcd.print(text, c.x - len/2, c.y - textHeight/2);
  glcd.setColor(saveColor);
}

// print the value of the region
void Region::printValue(uint16_t value, uint16_t textColor, uint8_t* font)
{
  Serial.println("Region::printValue called");
  char textBuffer[5];
  itoa(value, textBuffer, 10);
  centerText(textBuffer, textColor, font);
}

bool Region::contains(const Point& p) const
{ 
  // Serial.print("Rect{"); Serial.print(_left); Serial.print(", "); Serial.print(_top); Serial.print(", ");
  // Serial.print(_right); Serial.print(", "); Serial.print(_bottom); Serial.println("}");
  return (_left <= p.x && p.x <= _right && _top <= p.y && p.y <= _bottom); 
}


void SelectValueRegion::draw(const char* text, uint16_t boundingColor, uint16_t textBGColor, uint16_t textColor)
{
  _boundingRegion.drawMarginRect(boundingColor);
  _textHeaderRegion.drawMarginRect(textBGColor, true, true);
  _textHeaderRegion.centerText(text, textColor, SmallFont);
  _upButtonRegion.drawTriangle(boundingColor);
  _downButtonRegion.drawTriangle(boundingColor, true, false);
  _numberDisplayRegion.printValue(_value, textColor, BigFont);

}

bool SelectValueRegion::check(Point p)
{
  bool hit = false;
  if (!isActive()){
//    Serial.println("not active");
    hit = false;
  }
  else if (_upButtonRegion.contains(p)) {
//    Serial.println("is active!");
    // increment value
    uint16_t temp = _value + _increment;
    _value = (temp >= _maxValue) ? _maxValue : temp;
    Serial.print("Value = ");Serial.println(_value);
    hit = true;
  }
  else if (_downButtonRegion.contains(p)) {
//    Serial.println("is active!");
    // decrement value
    uint16_t temp = _value - _increment;
    _value = (temp <= _minValue) ? _minValue : temp;
    hit = true;
  }
  return hit;
}

void SelectValueRegion::printValue(uint16_t textColor, uint8_t* font, uint16_t backColor)
{
  // clear text
  _numberDisplayRegion.drawMarginRect(backColor, true, false);

  // write new text
  Serial.println("writing new value");
  Serial.println("SelectValueRegion::printValue");
  _numberDisplayRegion.printValue(_value, textColor, font);
}

void ButtonRegion::draw(const char* text, uint16_t boundingColor, uint16_t textBGColor, uint16_t textColor)
{
  _rectRegion.drawMarginRect(textBGColor, true, true);
  _rectRegion.centerText(text, textColor, SmallFont);
}

bool ButtonRegion::check(Point p)
{
  bool hit = false;
  if (!isActive()){
    hit = false;
  }
  else if (_rectRegion.contains(p)) {
    Serial.println("button is active!");
    hit = true;
  }
  return hit;
}

