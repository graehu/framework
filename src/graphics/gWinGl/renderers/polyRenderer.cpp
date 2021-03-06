#include "../gWinGl.h"
#include "../../../physics/polygon.h"

void gWinGl::visit(polygon* _poly)
{
	if(_poly->m_vertices.empty())return;
	glColor3f(0,1,0);
	glBegin(GL_LINE_STRIP);
  for(unsigned int i = 0; i < _poly->m_vertices.size(); i++)
    glVertex3f(_poly->m_vertices[i].i, _poly->m_vertices[i].j, _poly->m_vertices[i].k);
  glVertex3f(_poly->m_vertices[0].i, _poly->m_vertices[0].j, _poly->m_vertices[0].k);
	glEnd();
	glColor3f(1,1,1);
};
