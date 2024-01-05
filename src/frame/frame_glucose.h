// frame_glucose.h
#ifndef _FRAME_GLUCOSE_H_
#define _FRAME_GLUCOSE_H_

#include <M5EPD.h>
#include "frame_base.h"

class Frame_Glucose : public Frame_Base {
public:
    Frame_Glucose();
    ~Frame_Glucose();
    int run();
    int init(epdgui_args_vector_t &args);
};

#endif //_FRAME_GLUCOSE_H_
