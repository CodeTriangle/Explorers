#include <allegro5/allegro.h>

#include "../include/tilemap.hpp"
#include "../include/constants.hpp"
#include "../include/levels.hpp"
#include "../include/tiles.hpp"

int main(int argc, char **argv) {
  ALLEGRO_DISPLAY *display;
  ALLEGRO_EVENT_QUEUE *event_queue;
  ALLEGRO_TIMER *frame_timer;

  tilemap *bg, *fg;
  level l;
  
  bool loop = true, redraw = true;

  bool future = false, travel = false;

  al_init();
  al_install_keyboard();

  display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);

  frame_timer = al_create_timer(1.0 / FPS);

  event_queue = al_create_event_queue();
  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(frame_timer));

  init_materials(); 

  l = read_level("assets/5.lv");

  al_set_target_bitmap(al_get_backbuffer(display));
  al_start_timer(frame_timer);

  while (loop) {
    ALLEGRO_EVENT e;
    al_wait_for_event(event_queue, &e);

    if (e.type == ALLEGRO_EVENT_TIMER) {
    }
    else if (e.type == ALLEGRO_EVENT_KEY_DOWN) {
    }
    else if (e.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      loop = false;
    }

    if (redraw && al_is_event_queue_empty(event_queue)) {
      al_clear_to_color(al_map_rgb(0,0,0));
      
      draw_tilemap(&l.back, SCALE_FACTOR);
      draw_tilemap(&l.fore, SCALE_FACTOR);
      if (future)
	draw_tilemap(&l.rubble, SCALE_FACTOR);
      
      al_flip_display();
    }
  }
}
