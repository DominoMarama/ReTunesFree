#pragma once
#include <rack.hpp>


using namespace rack;

// Declare the Plugin, defined in plugin.cpp
extern Plugin* pluginInstance;

// Declare each Model, defined in each module source file
// extern Model* modelMyModule;
extern Model* modelReTunes_ForFour;

struct OptionalInput : app::SvgPort {
        OptionalInput() {
                setSvg(APP->window->loadSvg(asset::system("res/ComponentLibrary/PJ301M.svg")));
        }

        void step() {
            if (!module)
                    return;

            CableWidget* cw = APP->scene->rack->incompleteCable;
            if (cw) {
                    if (cw->outputPort)
                        Widget::show();
                    else
                        Widget::hide();
            } else if (module->inputs[portId].isConnected()){
                Widget::show();
            } else {
                Widget::hide();
            }
            Widget::step();
        }
};
