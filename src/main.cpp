#include <algorithm> 

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

  al_init();
  al_init_image_addon();
  al_install_keyboard();

  display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);

  frame_timer = al_create_timer(1.0 / FPS);

  event_queue = al_create_event_queue();
  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_timer_event_source(frame_timer));

  init_materials();
  
  level l("assets/5.lv"); 

  l.justify(al_get_display_width(display), al_get_display_height(display));

  al_set_target_bitmap(al_get_backbuffer(display));
  al_start_timer(frame_timer); 

  while (loop) {
    ALLEGRO_EVENT e;
    al_wait_for_event(event_queue, &e);

    if (e.type == ALLEGRO_EVENT_TIMER) {
      redraw = true;
    }
    else if (e.type == ALLEGRO_EVENT_KEY_DOWN) {
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
}
