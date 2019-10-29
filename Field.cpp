#include "Field.h"

//------------------------------------------------------------------------------
Sep::Field::Field(FieldType type) : type_(type)
{
}

//------------------------------------------------------------------------------
char Sep::Field::getCharacter()
{
  char not_happening = 0;

  switch (type_)
  {
    case AIR:
      return ' ';
    case EARTH:
      return 'E';
    case WATER:
      return 'W';
    case WORM:
      return '~';
    case WORM2:
      return '*';
    case CHEST:
      return '#';
  }
  return not_happening;
}

//------------------------------------------------------------------------------
Sep::Field::FieldType Sep::Field::getType()
{
  return type_;
}

//------------------------------------------------------------------------------
void Sep::Field::setType(Field::FieldType type)
{
  type_ = type;
}
