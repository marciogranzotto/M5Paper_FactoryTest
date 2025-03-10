#ifndef _FRAME_MAIN_H_
#define _FRAME_MAIN_H_

#include "frame_base.h"

class Frame_Main : public Frame_Base {
   public:
    Frame_Main();
    ~Frame_Main();
    int run();
    int init(epdgui_args_vector_t &args);
    void StatusBar(m5epd_update_mode_t mode);
    void AppName(m5epd_update_mode_t mode);

   private:
    EPDGUI_Button *_key[9];
    M5EPD_Canvas *_bar;
    M5EPD_Canvas *_names;
    uint32_t _next_update_time;
    uint32_t _time;
};

typedef struct {
    String label;
    const unsigned char *icon;
    void (*callback)(epdgui_args_vector_t&);
} button_t;

#endif  //_FRAME_MAIN_H_