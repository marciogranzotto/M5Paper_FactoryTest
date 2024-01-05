#include "frame_main.h"
#include "frame_setting.h"
#include "frame_keyboard.h"
#include "frame_factorytest.h"
#include "frame_wifiscan.h"
#include "frame_lifegame.h"
#include "frame_fileindex.h"
#include "frame_compare.h"
#include "frame_home.h"
#include "frame_glucose.h"

enum {
    kKeyFactoryTest = 0,
    kKeySetting,
    kKeyKeyboard,
    kKeyWifiScan,
    kKeySDFile,
    kKeyCompare,
    kKeyHome,
    kKeyLifeGame,
    kKeyGlucose
};

#define KEY_W 92
#define KEY_H 92

void key_setting_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Setting");
    if (frame == NULL) {
        frame = new Frame_Setting();
        EPDGUI_AddFrame("Frame_Setting", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}

void key_keyboard_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Keyboard");
    if (frame == NULL) {
        frame = new Frame_Keyboard(0);
        EPDGUI_AddFrame("Frame_Keyboard", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}

void key_factorytest_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_FactoryTest");
    if (frame == NULL) {
        frame = new Frame_FactoryTest();
        EPDGUI_AddFrame("Frame_FactoryTest", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}

void key_wifiscan_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_WifiScan");
    if (frame == NULL) {
        frame = new Frame_WifiScan();
        EPDGUI_AddFrame("Frame_WifiScan", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}

void key_lifegame_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Lifegame");
    if (frame == NULL) {
        frame = new Frame_Lifegame();
        EPDGUI_AddFrame("Frame_Lifegame", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}

void key_sdfile_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = new Frame_FileIndex("/");
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}

void key_compare_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Compare");
    if (frame == NULL) {
        frame = new Frame_Compare();
        EPDGUI_AddFrame("Frame_Compare", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}

void key_home_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Home");
    if (frame == NULL) {
        frame = new Frame_Home();
        EPDGUI_AddFrame("Frame_Home", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}

void key_glucose_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Glucose");
    if (frame == NULL) {
        frame = new Frame_Glucose();
        EPDGUI_AddFrame("Frame_Glucose", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int *)(args[0])) = 0;
}

// list of button_t
const button_t buttons[] = {
    {"FactoryTest", ImageResource_main_icon_factorytest_92x92, key_factorytest_cb},
    {"Setting", ImageResource_main_icon_setting_92x92, key_setting_cb},
    {"Keyboard", ImageResource_main_icon_keyboard_92x92, key_keyboard_cb},
    {"WLAN", ImageResource_main_icon_wifi_92x92, key_wifiscan_cb},
    {"Storage", ImageResource_main_icon_sdcard_92x92, key_sdfile_cb},
    {"Compare", ImageResource_main_icon_compare_92x92, key_compare_cb},
    {"Home", ImageResource_main_icon_home_92x92, key_home_cb},
    {"LifeGame", ImageResource_main_icon_lifegame_92x92, key_lifegame_cb},
    {"Glucose", ImageResource_main_icon_glucose_92x92, key_glucose_cb}
};

Frame_Main::Frame_Main(void) : Frame_Base(false) {
    _frame_name = "Frame_Main";
    _frame_id   = 1;

    _bar = new M5EPD_Canvas(&M5.EPD);
    _bar->createCanvas(540, 44);
    _bar->setTextSize(26);

    _names = new M5EPD_Canvas(&M5.EPD);
    _names->createCanvas(540, 32);
    _names->setTextDatum(CC_DATUM);

    int num_buttons = sizeof(buttons) / sizeof(button_t);

    log_d("num_buttons: %d", num_buttons);

    for (int i = 0; i < num_buttons; i++) {
        int row = i / 4; // Calculate the row of the button (0 for the first four, 1 for the next four)
        int col = i % 4; // Calculate the column of the button (0 to 3)
        int x_position = 20 + col * 136;
        int y_position = 90 + row * 150;
        log_d("button: %d - row: %d, col: %d | x: %d, y: %d", i, row, col, x_position, y_position);
        _key[i] = new EPDGUI_Button(buttons[i].label, x_position, y_position, KEY_W, KEY_H);
        _key[i]->CanvasNormal()->pushImage(0, 0, KEY_W, KEY_H, buttons[i].icon);
        *(_key[i]->CanvasPressed()) = *(_key[i]->CanvasNormal());
        _key[i]->CanvasPressed()->ReverseColor();
        _key[i]->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
        _key[i]->Bind(EPDGUI_Button::EVENT_RELEASED, buttons[i].callback);
    }

    _time             = 0;
    _next_update_time = 0;
}

Frame_Main::~Frame_Main(void) {
    for (int i = 0; i < 8; i++) {
        delete _key[i];
    }
}

void Frame_Main::AppName(m5epd_update_mode_t mode) {
    const int iconsPerRow = 4;
    const int iconWidth = 136;
    const int xOffset = 20 + 46;
    const int yOffset = 16;
    const int rowHeight = 151; // Adjust as needed for the height of each row

    int totalRows = (sizeof(buttons) / sizeof(button_t) + iconsPerRow - 1) / iconsPerRow;

    if (!_names->isRenderExist(20)) {
        _names->createRender(20, 26);
    }
    _names->setTextSize(20);

    for (int row = 0; row < totalRows; ++row) {
        _names->fillCanvas(0);
        for (int col = 0; col < iconsPerRow; ++col) {
            int index = row * iconsPerRow + col;
            if (index < sizeof(buttons) / sizeof(button_t)) {
                int xPosition = xOffset + col * iconWidth;
                _names->drawString(buttons[index].label, xPosition, yOffset);
            }
        }
        _names->pushCanvas(0, 186 + row * rowHeight, mode);
    }
}

void Frame_Main::StatusBar(m5epd_update_mode_t mode) {
    if ((millis() - _time) < _next_update_time) {
        return;
    }
    char buf[20];
    _bar->fillCanvas(0);
    _bar->drawFastHLine(0, 43, 540, 15);
    _bar->setTextDatum(CL_DATUM);
    _bar->drawString("M5Paper", 10, 27);

    // Battery
    _bar->setTextDatum(CR_DATUM);
    _bar->pushImage(498, 8, 32, 32, ImageResource_status_bar_battery_32x32);
    uint32_t vol = M5.getBatteryVoltage();

    if (vol < 3300) {
        vol = 3300;
    } else if (vol > 4350) {
        vol = 4350;
    }
    float battery = (float)(vol - 3300) / (float)(4350 - 3300);
    if (battery <= 0.01) {
        battery = 0.01;
    }
    if (battery > 1) {
        battery = 1;
    }
    uint8_t px = battery * 25;
    sprintf(buf, "%d%%", (int)(battery * 100));
    // _bar->drawString(buf, 498 - 10, 27);
    _bar->fillRect(498 + 3, 8 + 10, px, 13, 15);
    // _bar->pushImage(498, 8, 32, 32, 2,
    // ImageResource_status_bar_battery_charging_32x32);

    // Time
    rtc_time_t time_struct;
    rtc_date_t date_struct;
    M5.RTC.getTime(&time_struct);
    M5.RTC.getDate(&date_struct);
    sprintf(buf, "%2d:%02d", time_struct.hour, time_struct.min);
    _bar->setTextDatum(CC_DATUM);
    _bar->drawString(buf, 270, 27);
    _bar->pushCanvas(0, 0, mode);

    _time             = millis();
    _next_update_time = (60 - time_struct.sec) * 1000;
}

int Frame_Main::init(epdgui_args_vector_t &args) {
    _is_run = 1;
    M5.EPD.WriteFullGram4bpp(GetWallpaper());
    int num_buttons = sizeof(buttons) / sizeof(button_t);
    for (int i = 0; i < num_buttons; i++) {
        EPDGUI_AddObject(_key[i]);
    }
    _time             = 0;
    _next_update_time = 0;
    StatusBar(UPDATE_MODE_NONE);
    AppName(UPDATE_MODE_NONE);
    return 9;
}

int Frame_Main::run() {
    Frame_Base::run();
    StatusBar(UPDATE_MODE_GL16);
    return 1;
}