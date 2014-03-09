#ifndef _CONTROLS_GUI_H_
#define _CONTROLS_GUI_H_

#include <iostream>
#include "NFont.h"

class Text
{
private:
    SDL_Rect m_Rect;
    NFont* m_Police;
    std::string m_Text;

public:
    Text(NFont* police, const std::string& text)
    {
         m_Police = police;
         m_Text = text;
         m_Rect.w = m_Police->getWidth(m_Text.c_str());
         m_Rect.h = m_Police->getHeight(m_Text.c_str());
    }

    int getWidth() const    {   return m_Rect.w;    }
    int getHeight() const    {   return m_Rect.h;    }
    int getX() const    {   return m_Rect.x;    }
    int getY() const    {   return m_Rect.y;    }
    std::string getText() const  {   return m_Text;  }

    void setX(int x)    {   m_Rect.x = x;   }
    void setY(int y)    {   m_Rect.y = y;   }
    void setText(const std::string& text)
    {
        m_Text = text;
        m_Rect.w = m_Police->getWidth(m_Text.c_str());
        m_Rect.h = m_Police->getHeight(m_Text.c_str());
    }

    void dessineText()
    {
        m_Police->draw(m_Rect.x, m_Rect.y, m_Text.c_str());
    }
};

#endif // _CONTROLS_GUI_H_
