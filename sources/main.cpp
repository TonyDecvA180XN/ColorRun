#include "W4Framework.h"

W4_USE_UNSTRICT_INTERFACE

class BasicApp : public IGame {
public:
    void onStart() override {
        W4_LOG_INFO("BasicApp: Hello world!");
        createWidget<Label>(nullptr, "Hello world!", ivec2(540, 200));
    }
};

W4_RUN(BasicApp)
