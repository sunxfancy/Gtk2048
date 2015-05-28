/* 
* @Author: sxf
* @Date:   2015-05-19 14:50:36
* @Last Modified by:   sxf
* @Last Modified time: 2015-05-19 15:44:10
*/

#ifndef MYAREA_H
#define MYAREA_H

#include <gtkmm/drawingarea.h>

class MyArea_private;
class MyArea : public Gtk::DrawingArea
{
public:
	MyArea();
	virtual ~MyArea();

	void setData(const int[4][4]);

protected:
	MyArea_private* priv;

	//Override default signal handler:
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
};


#endif // MYAREA_H
