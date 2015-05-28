/* 
* @Author: sxf
* @Date:   2015-05-19 11:20:42
* @Last Modified by:   sxf
* @Last Modified time: 2015-05-20 21:58:35
*/

#ifndef GAME_H
#define GAME_H

#include <gtkmm/window.h>

class Game_private;
class Game : public Gtk::Window
{
public:
	Game();
	virtual ~Game();

private:
	Game_private* priv;
};


#endif // GAME_H
