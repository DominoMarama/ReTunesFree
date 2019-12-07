#include "plugin.hpp"


struct ReTunes_ForFour : Module {
    enum ParamIds {
        COUNT_X_PARAM,
        COUNT_Y_PARAM,
        COUNT_Z_PARAM,
        PATTERN_A_PARAM,
        PATTERN_B_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        CLOCK_INPUT,
        RESET_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        // BEAT1 to BEAT4 must be first four outputs
        BEAT_1_OUTPUT,
        BEAT_2_OUTPUT,
        BEAT_3_OUTPUT,
        BEAT_4_OUTPUT,
        PATTERN_A_OUTPUT,
        PATTERN_B_OUTPUT,
        COUNT_X_OUTPUT,
        COUNT_Y_OUTPUT,
        COUNT_Z_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };

    ReTunes_ForFour() {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
        configParam(PATTERN_A_PARAM, 0.f, 255.f, 12.f, "Pattern A");
        configParam(PATTERN_B_PARAM, 0.f, 255.f, 24.f, "Pattern B");
        configParam(COUNT_X_PARAM, 1.f, 256.f, 8.f, "Count X");
        configParam(COUNT_Y_PARAM, 1.f, 256.f, 16.f, "Count Y");
        configParam(COUNT_Z_PARAM, 1.f, 256.f, 32.f, "Count Z");
        onReset();
    };

    void process(const ProcessArgs& args) override;

    int counter, beat, patternA, patternB, countdown1, countdown2, countdown3;

    bool resetNext = true;

    dsp::SchmittTrigger clockTrigger, resetTrigger;
};


void ReTunes_ForFour::process(const ProcessArgs &args) {
    if (resetTrigger.process(inputs[RESET_INPUT].value)) {
        resetNext = true;
    }

    if (clockTrigger.process(inputs[CLOCK_INPUT].value)) {
        counter++;
        if (counter == 256) counter = 0;
        countdown1--;
        if (countdown1 < 0){
            countdown1 = (int)params[COUNT_X_PARAM].value - 1;
        };
        countdown2--;
        if (countdown2 < 0){
            countdown2 = (int)params[COUNT_Y_PARAM].value - 1;
        };
        countdown3--;
        if (countdown3 < 0){
            countdown3 = (int)params[COUNT_Z_PARAM].value - 1;
        };
    } else if (resetNext) {
        counter = -1;
        countdown1 = 1;
        countdown2 = 1;
        countdown3 = 1;
        resetNext = false;
    };

    beat = counter % 4;
    for (int i = 0; i < 4; i++){
        outputs[i].value = (beat == i ? inputs[CLOCK_INPUT].value : 0.0f);
    }

    patternA = (int)params[PATTERN_A_PARAM].value;
    patternB = (int)params[PATTERN_B_PARAM].value;

    outputs[PATTERN_A_OUTPUT].value = ((counter & patternA) == patternA ? 10.0f : 0.0f);
    outputs[PATTERN_B_OUTPUT].value = ((counter & patternB) == patternB ? 10.0f : 0.0f);

    outputs[COUNT_X_OUTPUT].value = (countdown1 ? 0.0f : inputs[CLOCK_INPUT].value);
    outputs[COUNT_Y_OUTPUT].value = (countdown2 ? 0.0f : inputs[CLOCK_INPUT].value);
    outputs[COUNT_Z_OUTPUT].value = (countdown3 ? 0.0f : inputs[CLOCK_INPUT].value);
};


struct ReTunes_ForFourWidget : ModuleWidget {
    ReTunes_ForFourWidget(ReTunes_ForFour* module) {
        setModule(module);
        setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ReTunes-ForFour.svg")));

        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
        addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
        addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

        {
            auto w = createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(8, 65.291)), module, ReTunes_ForFour::PATTERN_A_PARAM);
            dynamic_cast<Knob*>(w)->snap = true;
            addParam(w);
        };
        {
            auto w = createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(8, 77.363)), module, ReTunes_ForFour::PATTERN_B_PARAM);
            dynamic_cast<Knob*>(w)->snap = true;
            addParam(w);
        };

        {
            auto w = createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(8, 89.435)), module, ReTunes_ForFour::COUNT_X_PARAM);
            dynamic_cast<Knob*>(w)->snap = true;
            addParam(w);
        };
        {
            auto w = createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(8, 101.507)), module, ReTunes_ForFour::COUNT_Y_PARAM);
            dynamic_cast<Knob*>(w)->snap = true;
            addParam(w);
        };
        {
            auto w = createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(8, 113.58)), module, ReTunes_ForFour::COUNT_Z_PARAM);
            dynamic_cast<Knob*>(w)->snap = true;
            addParam(w);
        };

        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.544, 22.292)), module, ReTunes_ForFour::CLOCK_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.544, 40.087)), module, ReTunes_ForFour::RESET_INPUT));

        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 17.0)), module, ReTunes_ForFour::BEAT_1_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 29.073)), module, ReTunes_ForFour::BEAT_2_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 41.145)), module, ReTunes_ForFour::BEAT_3_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 53.218)), module, ReTunes_ForFour::BEAT_4_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 65.29)), module, ReTunes_ForFour::PATTERN_A_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 77.363)), module, ReTunes_ForFour::PATTERN_B_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 89.435)), module, ReTunes_ForFour::COUNT_X_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 101.507)), module, ReTunes_ForFour::COUNT_Y_OUTPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 113.58)), module, ReTunes_ForFour::COUNT_Z_OUTPUT));
    }
};


Model* modelReTunes_ForFour = createModel<ReTunes_ForFour, ReTunes_ForFourWidget>("ReTunes-ForFour");
