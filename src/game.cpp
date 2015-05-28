/* 
* @Author: sxf
* @Date:   2015-05-19 11:22:49
* @Last Modified by:   sxf
* @Last Modified time: 2015-05-19 20:36:29
*/

#include "game.h"
#include <stdio.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <gdk/gdkkeysyms.h>
#include <gtkmm.h>
#include <string.h>
#include "myarea.h"

class Game_private
{
public:
	Game_private(Game* game):
		m_box(Gtk::ORIENTATION_VERTICAL, 2),
		m_score("Score : ")
	{
		this->game = game; 
		score = 0;
		m_box.pack_start(m_score, Gtk::PACK_SHRINK);
		m_box.pack_start(m_area);
		srand(time(NULL));
		memset(data, 0, sizeof(int)*16);
	}
	~Game_private() {

	}
	
 	Game* game;

	MyArea m_area;
	Gtk::Box m_box;
	Gtk::Label m_score;
	int score;
	const static int fx[4][2];

	int data[4][4];

	bool combine(int i, int j, int k) {
		int ni = i; int nj = j;
		ni += fx[k][0];
		nj += fx[k][1];
		int obji = i, objj = j;
		while ( 0 <= ni && ni < 4 &&
				0 <= nj && nj < 4 )
		{
			if (data[ni][nj] == 0) {
				obji = ni; objj = nj;
			} else {
				if (data[ni][nj] == data[i][j]) {
					score += (1 << data[ni][nj]);
					++data[ni][nj];
					data[i][j] = 0;
					if (data[ni][nj] == 12) return true;
					return false;
				} else break;
			}
			ni += fx[k][0];
			nj += fx[k][1];
		}
		if (!(obji == i && objj == j)) {
			data[obji][objj] = data[i][j];
			data[i][j] = 0;
		}
		return false;
	}

	bool randomNew() {
		int empty_block[17][2]; int sum = 0;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				if (data[i][j] == 0) {
					empty_block[sum][0] = i; 
					empty_block[sum][1] = j;
					++sum;
				}
			}
		}
		if (sum < 1) return false;

		int t = rand() % sum;
		data[ empty_block[t][0] ][ empty_block[t][1] ] = (rand() % 4) < 3 ? 1 : 2;
		empty_block[t][0] = empty_block[sum-1][0];
		empty_block[t][1] = empty_block[sum-1][1]; 
		--sum;
		
		return true;
	}

	void cleanData() {
		memset(data, 0, sizeof(int)*16);
		randomNew();
		randomNew();
	}

	void gameWin() {
		Gtk::MessageDialog dialog(*game, "YOU WIN!");
		dialog.run();
		cleanData();
	}

	void gameOver() {
		Gtk::MessageDialog dialog(*game, "GAME OVER!");
		dialog.run();
		cleanData();
	}

	void gameRun(int k) {
		bool winflag = false;
		switch (k) {
			case 0 : 
				for (int i = 0; i < 4; ++i)
					for (int j = 0; j < 4; ++j)
						if (combine(i,j,k)) winflag = true;
			break;
			case 1 : 
				for (int j = 3; j >= 0; --j)
					for (int i = 0; i < 4; ++i)
						if (combine(i,j,k)) winflag = true;
			break;
			case 2 : 
				for (int i = 3; i >= 0; --i)
					for (int j = 0; j < 4; ++j)
						if (combine(i,j,k)) winflag = true;
			break;
			case 3 : 
				for (int j = 0; j < 4; ++j)
					for (int i = 0; i < 4; ++i)
						if (combine(i,j,k)) winflag = true;
			break;
		}

		// 判断胜负条件
		if (winflag) {
			gameWin();
			return;
		}
		if (!randomNew()) {
			gameOver();
		}

		Glib::RefPtr<Gdk::Window> win = game->get_window();
		if (win)
		{
			m_area.setData(data);
		    Gdk::Rectangle r(0, 0, 600, 600);
		    win->invalidate_rect(r, false);
			m_area.show();
			char score_text[20]; memset(score_text, 0, 20);
			sprintf(score_text, "Score : %d", score);
			m_score.set_text(score_text);
		}
	}


	bool on_key_press_event(GdkEventKey* event) {
		if (event->type == GDK_KEY_PRESS)
		{
			switch (event->keyval) {
				case GDK_KEY_Up:
					gameRun(0); break;
				case GDK_KEY_Right:
					gameRun(1); break;
				case GDK_KEY_Down:
					gameRun(2); break;
				case GDK_KEY_Left:
					gameRun(3); break;	
				default:
					return false;
			}
			return true;
		}
		return false;
	}
};

const int Game_private::fx[4][2] = { {-1,0}, {0,1}, {1,0}, {0,-1} };

Game::Game() {
	priv = new Game_private(this);
	this->set_default_size(600, 600);
	this->signal_key_press_event().connect( sigc::mem_fun(*priv, &Game_private::on_key_press_event), false);
	this->add_events(Gdk::KEY_PRESS_MASK);
	add(priv->m_box);
	priv->cleanData();
	priv->m_area.setData(priv->data);
	priv->m_area.show();
	show_all_children();
}

Game::~Game() {
	delete priv;
}

