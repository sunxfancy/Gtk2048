/* 
* @Author: sxf
* @Date:   2015-05-19 14:51:19
* @Last Modified by:   sxf
* @Last Modified time: 2015-05-19 19:17:14
*/

#include "myarea.h"
#include <cairomm/context.h>
#include <string.h>

class MyArea_private
{
public:
	MyArea_private() {
		memset(data, 0, sizeof(int)*16);
		
		// 准备好字体
		font.set_family("monospace");
		font.set_weight(Pango::WEIGHT_BOLD);
		font.set_size(16*PANGO_SCALE);
	}
	~MyArea_private() {

	}
	
	double xc, yc;

	int data[4][4];
	const static double color[13][3];

	Pango::FontDescription font;

	void drawOneBox(int i, int j,
		const Cairo::RefPtr<Cairo::Context>& cc);

	void drawOneText(int i, int j,
		const Cairo::RefPtr<Cairo::Context>& cc, 
		Glib::RefPtr<Pango::Layout>& layout);
};


const double MyArea_private::color[13][3] = {
	{1, 1, 1},		// 0
	{0, 0.8, 0.5},	// 1
	{0.7, 0.8, 0},	// 2
	{0.3, 0.7, 0.6},// 4
	{0.5, 0.3, 0.7},// 8
	{0.6, 0.5, 0.1},// 16
	{0.3, 0.4, 0},	// 32
	{0.5, 0.2, 0.3},// 64
	{0.3, 0.1, 0},	// 128
	{0, 0, 0.8},	// 256
	{0.1, 0.1, 0.3},// 512
	{0, 0.6, 0.1},	// 1024
	{0, 1.0, 0} 	// 2048
};



MyArea::MyArea() {
	priv = new MyArea_private();

}

MyArea::~MyArea() {
	delete priv;
}

void MyArea::setData(const int data[4][4]) {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			priv->data[i][j] = data[i][j];
		}
	}
}

void MyArea_private::drawOneBox(int i, int j,
	const Cairo::RefPtr<Cairo::Context>& cc)
{
	int elem = data[i][j];
	double color_red   = color[elem][0];
	double color_green = color[elem][1];
	double color_blue  = color[elem][2];
	cc->set_source_rgb(color_red, color_green, color_blue);
	cc->rectangle(j*xc+1 , i*yc+1, xc-2, yc-2);
	cc->fill();
}

void MyArea_private::drawOneText(int i, int j,
	const Cairo::RefPtr<Cairo::Context>& cc, 
	Glib::RefPtr<Pango::Layout>& layout) 
{
	layout->set_font_description(font);
	cc->set_source_rgb(0, 0, 0);

	int text_width;
	int text_height;
	layout->get_pixel_size(text_width, text_height);
	cc->move_to( j*xc+ (xc-text_width)/2, i*yc+ (yc-text_height)/2);
	layout->show_in_cairo_context(cc);
}


bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cc)
{
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	priv->xc = width / 4.0;
	priv->yc = height / 4.0;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			priv->drawOneBox(i, j, cc);
			int elem = priv->data[i][j];
			if (elem != 0) {
				char number_string[5];
				int elem_data = elem != 0 ? 1 << (elem-1) : 0;
				sprintf(number_string, "%d", elem_data);
				Glib::RefPtr<Pango::Layout> layout = create_pango_layout(number_string);
				priv->drawOneText(i, j, cc, layout);
			}
		}
	}
	cc->restore();
	cc->stroke();

	return true;
}