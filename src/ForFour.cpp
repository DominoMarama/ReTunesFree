#include "plugin.hpp"


struct ReTunes_ForFour : Module {
	enum ParamIds {
		COUNT1_PARAM,
		COUNT2_PARAM,
		COUNT3_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		CLOCK_INPUT,
		RESET_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
                // BEAT1 to BEAT4 must be first four outputs
		BEAT1_OUTPUT,
		BEAT2_OUTPUT,
		BEAT3_OUTPUT,
		BEAT4_OUTPUT,
		EVENBAR_OUTPUT,
		FILLBARS_OUTPUT,
		COUNTER1_OUTPUT,
		COUNTER2_OUTPUT,
		COUNTER3_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

        ReTunes_ForFour() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
                configParam(COUNT1_PARAM, 1.f, 256.f, 8.f, "Count 1");
                configParam(COUNT2_PARAM, 1.f, 256.f, 16.f, "Count 2");
                configParam(COUNT3_PARAM, 1.f, 256.f, 32.f, "Count 3");
                onReset();
        };

        void process(const ProcessArgs& args) override;

        int counter, beat, countdown1, countdown2, countdown3;

        bool resetNext = true;

        dsp::SchmittTrigger clockTrigger, resetTrigger;
};

void ReTunes_ForFour::process(const ProcessArgs &args) {
    if (resetTrigger.process(inputs[RESET_INPUT].value)) {
        resetNext = true;
    }

    if (clockTrigger.process(inputs[CLOCK_INPUT].value)) {
        counter++;
        countdown1--;
        if (countdown1 < 0){
            countdown1 = (int)params[COUNT1_PARAM].value - 1;
        };
        countdown2--;
        if (countdown2 < 0){
            countdown2 = (int)params[COUNT2_PARAM].value - 1;
        };
        countdown3--;
        if (countdown3 < 0){
            countdown3 = (int)params[COUNT3_PARAM].value - 1;
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

    outputs[EVENBAR_OUTPUT].value = (counter & 4 ? 10.0f : 0.0f);
    outputs[FILLBARS_OUTPUT].value = ((counter & 12) == 12 ? 10.0f : 0.0f);
    outputs[COUNTER1_OUTPUT].value = (countdown1 ? 0.0f : inputs[CLOCK_INPUT].value);
    outputs[COUNTER2_OUTPUT].value = (countdown2 ? 0.0f : inputs[CLOCK_INPUT].value);
    outputs[COUNTER3_OUTPUT].value = (countdown3 ? 0.0f : inputs[CLOCK_INPUT].value);
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
            auto w = createParamCentered<RoundBlackKnob>(mm2px(Vec(9.131, 89.435)), module, ReTunes_ForFour::COUNT1_PARAM);
            dynamic_cast<Knob*>(w)->snap = true;
            addParam(w);
        };
        {
            auto w = createParamCentered<RoundBlackKnob>(mm2px(Vec(9.131, 101.507)), module, ReTunes_ForFour::COUNT2_PARAM);
            dynamic_cast<Knob*>(w)->snap = true;
            addParam(w);
        };
        {
            auto w = createParamCentered<RoundBlackKnob>(mm2px(Vec(9.131, 113.58)), module, ReTunes_ForFour::COUNT3_PARAM);
            dynamic_cast<Knob*>(w)->snap = true;
            addParam(w);
        };

        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.544, 22.292)), module, ReTunes_ForFour::CLOCK_INPUT));
        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(7.544, 40.087)), module, ReTunes_ForFour::RESET_INPUT));

                addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 17.0)), module, ReTunes_ForFour::BEAT1_OUTPUT));
                addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 29.073)), module, ReTunes_ForFour::BEAT2_OUTPUT));
                addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 41.145)), module, ReTunes_ForFour::BEAT3_OUTPUT));
                addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 53.218)), module, ReTunes_ForFour::BEAT4_OUTPUT));
                addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 65.29)), module, ReTunes_ForFour::EVENBAR_OUTPUT));
                addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 77.363)), module, ReTunes_ForFour::FILLBARS_OUTPUT));
                addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 89.435)), module, ReTunes_ForFour::COUNTER1_OUTPUT));
                addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 101.507)), module, ReTunes_ForFour::COUNTER2_OUTPUT));
                addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.936, 113.58)), module, ReTunes_ForFour::COUNTER3_OUTPUT));
	}
};


Model* modelReTunes_ForFour = createModel<ReTunes_ForFour, ReTunes_ForFourWidget>("ReTunes-ForFour");
