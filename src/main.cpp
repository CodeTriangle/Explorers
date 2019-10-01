#include <vector>
#include <algorithm>
#include <cstdio>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "../include/tilemap.hpp"
#include "../include/constants.hpp"
#include "../include/materials.hpp"
#include "../include/level.hpp"

int main(int argc, char **argv) {
  ALLEGRO_DISPLAY *display;
  ALLEGRO_EVENT_QUEUE *event_queue;
  ALLEGRO_TIMER *frame_timer;

  int scale_factor, origin_x, origin_y;
  
  bool loop = true, redraw = true;

  bool held = false, should_move = false;

  int current_level = 0;

  int direction = -1;

  al_init();
  al_init_image_addon();
  al_install_keyboard();

  display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);

  frame_timer = al_create_timer(1.0 / FPS);

  event_queue = al_create_event_queue();
  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(frame_timer));
  al_register_event_source(event_queue, al_get_keyboard_event_source());

  init_materials();
  
  level l("assets/1.lv");

  l.justify(al_get_display_width(display), al_get_display_height(display));

  al_set_target_bitmap(al_get_backbuffer(display));
  al_start_timer(frame_timer); 

  while (loop) {
    ALLEGRO_EVENT e;
    al_wait_for_event(event_queue, &e);

    if (e.type == ALLEGRO_EVENT_TIMER) {
      redraw = true;
      
      if (held && direction >= 0) {
	l.move_player(direction);
	direction = -1;
      }
    }
    else if (e.type == ALLEGRO_EVENT_KEY_DOWN) {
      if (!held) {

	switch (e.keyboard.keycode) {
	case ALLEGRO_KEY_UP:
	case ALLEGRO_KEY_W:
	  direction = 0;
	  break;
	case ALLEGRO_KEY_RIGHT:
	case ALLEGRO_KEY_D:
	  direction = 1;
	  break;
	case ALLEGRO_KEY_DOWN:
	case ALLEGRO_KEY_S:
	  direction = 2;
	  break;
	case ALLEGRO_KEY_LEFT:
	case ALLEGRO_KEY_A:
	  direction = 3;
	  break;
	case ALLEGRO_KEY_R:
	  l.reload();
	}

	if (direction >= 0) {
	  held = true;
	}
      }
    }
    else if (e.type == ALLEGRO_EVENT_KEY_UP)  {
      if (held)
	held = false;
    }
    else if (e.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
      l.justify(e.display.width, e.display.height);
    }
    else if (e.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      loop = false;
    }

    if (redraw && al_is_event_queue_empty(event_queue)) {
      redraw = false;
      al_clear_to_color(al_map_rgb(0,0,0));

      l.draw();
      
      al_flip_display();
    }
  }
  al_destroy_display(display);
  al_destroy_event_queue(event_queue);
  al_destroy_timer(frame_timer);
  l.clear();
}
