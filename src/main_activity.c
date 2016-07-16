#include <pebble.h>
static Window *s_main_window;
static TextLayer *s_time_layer;
static bool s_js_ready;

bool comm_is_js_ready() {
  return s_js_ready;
}

/*static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *ready_tuple = dict_find(iter, MESSAGE_KEY_JSReady);
  if(ready_tuple) {
    // PebbleKit JS is ready! Safe to send messages
    s_js_ready = true;
  }
}*/

static void main_window_load(Window *window) {

  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
}

static void main_window_unload(Window *window) {

}
void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
 // Window *window = (Window *)context;
  static char s_buffer[8];
  DictionaryIterator *iter;
  int key = 1, value = 2;
  ButtonId button = click_recognizer_get_button_id(recognizer);
  
  switch(button)
  {
    case BUTTON_ID_DOWN:
    strcpy(s_buffer, "down :P");
    if(app_message_outbox_begin(&iter) == APP_MSG_OK) {
      dict_write_int(iter, key, &value, sizeof(int), true);
      app_message_outbox_send();
    }

    break;
    case BUTTON_ID_UP:
    strcpy(s_buffer, "UP B)");
    break;
    case BUTTON_ID_SELECT:
    strcpy(s_buffer, "select");
    break;
    default:
    strcpy(s_buffer, "unknown");
    
  }
  
  

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}
void config_provider(Window *window) {
 // single click / repeat-on-hold config:
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, down_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, down_single_click_handler);
  /*
  window_single_repeating_click_subscribe(BUTTON_ID_SELECT, 1000, select_single_click_handler);

  // multi click config:
  window_multi_click_subscribe(BUTTON_ID_SELECT, 2, 10, 0, true, select_multi_click_handler);

  // long click config:
  window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_click_handler, select_long_click_release_handler);
  */
}

static void init()
{
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
   window_set_click_config_provider(s_main_window, (ClickConfigProvider) config_provider);
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}


static void deinit()
{
 window_destroy(s_main_window); 
}
int main()
{
  init();
  app_event_loop();
  deinit();
  
  
}
