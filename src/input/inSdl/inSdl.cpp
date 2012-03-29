#include "inSdl.h"


int inSdl::init()
{
  return 0;
}

bool inSdl::update(void)
{
  for(int i = 0; i < e_totalKeys; i++)
    m_keys[i] = false;

  char* internalKeys = (char*)SDL_GetKeyState(NULL);
    
  if(internalKeys['w'])
    m_keys[e_up] = true;
  if(internalKeys['a'])
    m_keys[e_left] = true;
  if(internalKeys['s'])
    m_keys[e_down] = true;
  if(internalKeys['d'])
    m_keys[e_right] = true;

  	int cx,cy;
	Uint8 ms = SDL_GetMouseState(&cx, &cy);
	if (ms & SDL_BUTTON(SDL_BUTTON_LEFT)) m_mouseButtons[e_leftClick] = true;
	else m_mouseButtons[e_leftClick] = false;

	if (ms & SDL_BUTTON(SDL_BUTTON_RIGHT)) m_mouseButtons[e_rightCLick] = true;
	else m_mouseButtons[e_rightCLick] = false;

	if (ms & SDL_BUTTON(SDL_BUTTON_MIDDLE)) m_mouseButtons[e_middleClick] = true;
	else m_mouseButtons[e_middleClick] = false;


  SDL_Event event;
  while (SDL_PollEvent(&event))
    {
      switch (event.type)
        {
	case SDL_QUIT:
	  {
	    return true;
	  }
        }
    }
  return false;
}
bool inSdl::isMouseClicked(mouseButtons _button)
{
	return m_mouseButtons[_button];
};

bool inSdl::isKeyPressed(keys _key)
{
  return m_keys[_key];
}


void inSdl::mouseDelta(float& _x, float& _y)
{
	int x = 0;
	int y = 0;

	SDL_GetRelativeMouseState(&x, &y);
	_x = x;
	_y = y;
}



input* input::inputFactory()
{
  return (input*)new inSdl; //returns an input class of sdl type
}

