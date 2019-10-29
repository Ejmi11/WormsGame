#ifndef SEP_FIELD_H
#define SEP_FIELD_H

namespace Sep
{
  //----------------------------------------------------------------------------
  // Field Class
  // class to model the fields
  //
  class Field
  {
    public:
      enum FieldType
      {
        AIR, EARTH, WATER, WORM, WORM2, CHEST
      };

      //------------------------------------------------------------------------
      // Field Constructor
      //
      Field(FieldType type);

      //------------------------------------------------------------------------
      // method that maps the enum values to their according names
      // @return ' ' if air
      // @return 'E' if earth
      // @return 'W' if water
      // @return '~' if worm belongs to player 1
      // @return '*' if worm belongs to player 2
      // @return not_happening if unknown
      //
      char getCharacter();

      //------------------------------------------------------------------------
      // method to get private variable type_
      // @return field type from private variable
      //
      FieldType getType();

      //------------------------------------------------------------------------
      // method to set private variable type_
      // @param type type of the field to be set
      //
      void setType(FieldType type);

    private:
      FieldType type_;
  };
}

#endif //SEP_FIELD_H
