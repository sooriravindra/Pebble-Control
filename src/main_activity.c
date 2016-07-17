#include <pebble.h>
#define ACCEL_STEP_MS 200
const uint32_t inbox_size = 64;
const uint32_t outbox_size = 256;
static Window *s_main_window;
static TextLayer *s_time_layer;
static bool s_js_ready;
bool send_data = true;
bool is_mouse_mode = true;
enum pebble_keys {
  KEY_UP =0,
  KEY_SEL,
  KEY_DOWN
};

int pressed_button = -1;

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
static void timer_callback(void *data) {
  DictionaryIterator *iter;
  int msgValue;
  
  if(is_mouse_mode){
    msgValue = 10;
  }
  else {
    msgValue = 20;
  }
  AccelData accel = (AccelData) { .x = 0, .y = 0, .z = 0 };
  accel_service_peek(&accel);

  if(app_message_outbox_begin(&iter) == APP_MSG_OK) {
    dict_write_int(iter, MESSAGE_KEY_AppEvent, &msgValue, sizeof(int), true);
    dict_write_int16(iter, MESSAGE_KEY_AccelX, accel.x);
    dict_write_int16(iter, MESSAGE_KEY_AccelY, accel.y);
    dict_write_int16(iter, MESSAGE_KEY_AccelZ, accel.z);
    dict_write_int16(iter, MESSAGE_KEY_KeyPress, pressed_button);
    app_message_outbox_send();
    pressed_button = -1;
  }

  app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
 // Window *window = (Window *)context;
  
  int ret, tries = 4;
  static char s_buffer[8];
  DictionaryIterator *iter;
  int msgValue;
  ButtonId button = click_recognizer_get_button_id(recognizer);
  
  switch(button)
  {
    case BUTTON_ID_DOWN:
    strcpy(s_buffer, "down :P");
    pressed_button = KEY_DOWN;
    break;
    
    case BUTTON_ID_UP:
    strcpy(s_buffer, "UP B)");
    pressed_button = KEY_UP;
    break;
    
    case BUTTON_ID_SELECT:
    strcpy(s_buffer, "select");
    pressed_button = KEY_SEL;
    break;
    
    default:
    pressed_button = -1;
    strcpy(s_buffer, "unknown");
    
  }
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}
void down_long_click_handler(ClickRecognizerRef recognizer, void *context) {
 // Window *window = (Window *)context;
  static char s_buffer[8];
  ButtonId button = click_recognizer_get_button_id(recognizer);
  
  if(button == BUTTON_ID_SELECT) {
    
    strcpy(s_buffer, "Swap X");
    text_layer_set_text(s_time_layer, s_buffer);
    is_mouse_mode = !is_mouse_mode;
  }
}
void config_provider(Window *window) {
 // single click / repeat-on-hold config:
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, down_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, down_single_click_handler);
  window_long_click_subscribe(BUTTON_ID_SELECT, 0, down_long_click_handler,NULL);
  /*
  window_single_repeating_click_subscribe(BUTTON_ID_SELECT, 1000, select_single_click_handler);

  // multi click config:
  window_multi_click_subscribe(BUTTON_ID_SELECT, 2, 10, 0, true, select_multi_click_handler);

  // long click config:
  window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_click_handler, select_long_click_release_handler);
  */
}
static void outbox_failed_callback(DictionaryIterator *iter,
                                      AppMessageResult reason, void *context) {
  // The message just sent failed to be delivered
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message send failed. Reason: %d", (int)reason);
  /*if(app_message_outbox_begin(&iter) == APP_MSG_OK) {
    app_message_outbox_send();
  }*/
  
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
  
  app_message_open(inbox_size, outbox_size);
  app_message_register_outbox_failed(outbox_failed_callback);
  accel_data_service_subscribe(0, NULL);
  app_timer_register(ACCEL_STEP_MS, timer_callback, NULL);
  
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}


static void deinit()
{
  accel_data_service_unsubscribe();
  window_destroy(s_main_window); 
}
int main()
{
  init();
  app_event_loop();
  deinit();
  
  
}
