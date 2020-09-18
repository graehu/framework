#ifndef BOX_H
#define BOX_H

#include "polygon.h"

namespace physics
{
   namespace collider
   {
      ///A Generic polygon shape class, used for colidable areas.
      class box : public polygon
      {
      public:
	 box(float _size = 1.0f)
	 {
	    m_vertices.push_back(vec3f(-_size, -_size));
	    m_vertices.push_back(vec3f(-_size, _size));
	    m_vertices.push_back(vec3f(_size, _size));
	    m_vertices.push_back(vec3f(_size, -_size));
	 }
	 box(float _size_x, float _size_y)
	 {
	    m_vertices.push_back(vec3f(-_size_x, -_size_y));
	    m_vertices.push_back(vec3f(-_size_x, _size_y));
	    m_vertices.push_back(vec3f(_size_x, _size_y));
	    m_vertices.push_back(vec3f(_size_x, -_size_y));
	 }
      protected:

      private:
      };
   }
}
#endif//BOX_H
