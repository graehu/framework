#ifndef PACKETUTILS_H
#define PACKETUTILS_H
#include <stdio.h>


class dataUtils
{
public:

  static dataUtils& instance(void)
  {
    static dataUtils* instance = 0;
    if(instance == 0)
      {
	instance = new dataUtils();
      }
    return *instance;
  }

  int readInteger(unsigned char* _data);
  unsigned int readUInteger(unsigned char* _data); ///these return the data at the offset, with the type used.

  short readShort(unsigned char* _data);
  unsigned short readUShort(unsigned char* _data);

  char readChar(unsigned char* _data);
  unsigned char readUChar(unsigned char* _data);

  template <typename T>
  T readData(unsigned char* _data)
  {
    // #TODO: Find a way to do this in a more generic way
    //      like writeData.
    unsigned int size = sizeof(T);
    if(size == 1)
      return ((T)*_data);
    else if(size == 2)
      return  (((T)_data[0] << 8) | ((T)_data[1]));
    else if(size == 4)
      return (((T)_data[0] << 24) | ((T)_data[1] << 16) |
	      ((T)_data[2] << 8)  | ((T)_data[3]));
    else
      printf("BAD DATA SIZE");
       
    return 0;
  }

  template <typename T>
  bool writeData(T _type, unsigned char* _data)
  {
    size_t size = sizeof(_type);
    for(unsigned int i = 0; i < size; i++)
      {
	if (i > 0) _data[i] = (_type >> (size-(i+1))*8);
	else       _data[i] = ((_type >> (size-(i+1))*8) & 0xFF);
      }
    return false;
  }

private:
  dataUtils(){};
  ~dataUtils(){};

};

#endif//PACKETUTILS_H
