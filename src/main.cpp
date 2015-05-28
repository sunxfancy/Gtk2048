/* 
* @Author: sxf
* @Date:   2015-05-07 12:22:40
* @Last Modified by:   sxf
* @Last Modified time: 2015-05-20 21:58:16
*/

#include <gtkmm.h>
#include "game.h"
int main(int argc, char *argv[])
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.abs.gtk2048");

	Game game;
	//Shows the window and returns when it is closed.
	return app->run(game);
}