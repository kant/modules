#include "ChortlingHamsterModules.hpp"


struct Mum : Module {
	enum ParamIds {
		ENUMS(TOGGLE_BUTTON, 10),
		ENUMS(MUTE_BUTTON, 10),
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(VOLTAGE_INPUT, 10),
		ENUMS(TRIGGER_INPUT, 10),
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(VOLTAGE_OUTPUT, 10),
		NUM_OUTPUTS
	};
	enum LightIds {
		ENUMS(TOGGLE_BUTTON_LIGHT, 10),
		ENUMS(MUTE_BUTTON_LIGHT, 10),
		NUM_LIGHTS
	};

	bool toggleState[10];
	bool muteState[10];
	bool hasToggleTriggerBeenTriggered[10];
	dsp::BooleanTrigger toggle[10];
	dsp::BooleanTrigger mute[10];
	dsp::SchmittTrigger toggleTrigger[10];
	dsp::PulseGenerator toggleTriggerLightPulse[10];

	Mum() {
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

		for (int i = 0; i < 10; i++) {
			configParam(TOGGLE_BUTTON + i, 0.f, 1.f, 0.f, "");
			configParam(MUTE_BUTTON + i, 0.f, 1.f, 0.f, "");
		}

		onReset();
	}

	void onReset() override {
		for (int i = 0; i < 10; i++) {
			muteState[i] = false;
			toggleState[i] = false;
		}
	}

	void onRandomize() override {
		for (int i = 0; i < 10; i++) {
			muteState[i] = (random::uniform() < 0.5f);
			toggleState[i] = (random::uniform() < 0.5f);
		}
	}

	void process(const ProcessArgs& args) override {

		for (int i = 0; i < 10; i++) {

			// Check if toggle button is being used.
			if (toggle[i].process(params[TOGGLE_BUTTON + i].getValue() > 0.f))
				toggleState[i] ^= true;

			// Set light for toggle button.
			lights[TOGGLE_BUTTON_LIGHT + i].setBrightness(toggleState[i] ? 0.9f : 0.f);

			if (mute[i].process(params[MUTE_BUTTON + i].getValue() > 0.f))
				muteState[i] ^= true;

			// Set light for mute button.
			lights[MUTE_BUTTON_LIGHT + i].setBrightness(muteState[i] ? 0.f : 0.9f);

			// Check toggle trigger state.
			if (
				toggleTrigger[i].process(inputs[TRIGGER_INPUT + i].getVoltage() / 0.7)
				&&
				toggleState[i]
			) {
				// Toggle mute state.
				muteState[i] ^= true;

				// Set toggle state to false.
				toggleState[i] = false;

				// Pulse the toggle trigger light.
				toggleTriggerLightPulse[i].trigger(1e-1f);
			}

			// Output
			if (muteState[i]) {
				outputs[VOLTAGE_OUTPUT + i].setVoltage(inputs[VOLTAGE_INPUT + i].getVoltage());
			} else {
				outputs[VOLTAGE_OUTPUT + i].setVoltage(0.f);
			}
		
		}

	}
};


struct MumWidget : ModuleWidget {
	MumWidget(Mum* module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Mum.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<LEDBezel>(mm2px(Vec(14.313, 14.761)), module, Mum::TOGGLE_BUTTON + 0));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(14.313, 25.759)), module, Mum::TOGGLE_BUTTON + 1));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(14.313, 36.756)), module, Mum::TOGGLE_BUTTON + 2));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(14.313, 47.754)), module, Mum::TOGGLE_BUTTON + 3));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(14.313, 58.751)), module, Mum::TOGGLE_BUTTON + 4));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(14.313, 69.749)), module, Mum::TOGGLE_BUTTON + 5));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(14.313, 80.746)), module, Mum::TOGGLE_BUTTON + 6));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(14.313, 91.744)), module, Mum::TOGGLE_BUTTON + 7));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(14.313, 102.741)), module, Mum::TOGGLE_BUTTON + 8));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(14.313, 113.739)), module, Mum::TOGGLE_BUTTON + 9));

		addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(14.313, 14.761)), module, Mum::TOGGLE_BUTTON_LIGHT + 0));
		addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(14.313, 25.759)), module, Mum::TOGGLE_BUTTON_LIGHT + 1));
		addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(14.313, 36.756)), module, Mum::TOGGLE_BUTTON_LIGHT + 2));
		addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(14.313, 47.754)), module, Mum::TOGGLE_BUTTON_LIGHT + 3));
		addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(14.313, 58.751)), module, Mum::TOGGLE_BUTTON_LIGHT + 4));
		addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(14.313, 69.749)), module, Mum::TOGGLE_BUTTON_LIGHT + 5));
		addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(14.313, 80.746)), module, Mum::TOGGLE_BUTTON_LIGHT + 6));
		addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(14.313, 91.744)), module, Mum::TOGGLE_BUTTON_LIGHT + 7));
		addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(14.313, 102.741)), module, Mum::TOGGLE_BUTTON_LIGHT + 8));
		addChild(createLightCentered<LEDBezelLight<RedLight>>(mm2px(Vec(14.313, 113.739)), module, Mum::TOGGLE_BUTTON_LIGHT + 9));

		addParam(createParamCentered<LEDBezel>(mm2px(Vec(31.407, 14.761)), module, Mum::MUTE_BUTTON + 0));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(31.407, 25.759)), module, Mum::MUTE_BUTTON + 1));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(31.407, 36.756)), module, Mum::MUTE_BUTTON + 2));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(31.407, 47.754)), module, Mum::MUTE_BUTTON + 3));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(31.407, 58.751)), module, Mum::MUTE_BUTTON + 4));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(31.407, 69.749)), module, Mum::MUTE_BUTTON + 5));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(31.407, 80.746)), module, Mum::MUTE_BUTTON + 6));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(31.407, 91.744)), module, Mum::MUTE_BUTTON + 7));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(31.407, 102.741)), module, Mum::MUTE_BUTTON + 8));
		addParam(createParamCentered<LEDBezel>(mm2px(Vec(31.407, 113.739)), module, Mum::MUTE_BUTTON + 9));

		addChild(createLightCentered<LEDBezelLight<GreenLight>>(mm2px(Vec(31.407, 14.761)), module, Mum::MUTE_BUTTON_LIGHT + 0));
		addChild(createLightCentered<LEDBezelLight<GreenLight>>(mm2px(Vec(31.407, 25.759)), module, Mum::MUTE_BUTTON_LIGHT + 1));
		addChild(createLightCentered<LEDBezelLight<GreenLight>>(mm2px(Vec(31.407, 36.756)), module, Mum::MUTE_BUTTON_LIGHT + 2));
		addChild(createLightCentered<LEDBezelLight<GreenLight>>(mm2px(Vec(31.407, 47.754)), module, Mum::MUTE_BUTTON_LIGHT + 3));
		addChild(createLightCentered<LEDBezelLight<GreenLight>>(mm2px(Vec(31.407, 58.751)), module, Mum::MUTE_BUTTON_LIGHT + 4));
		addChild(createLightCentered<LEDBezelLight<GreenLight>>(mm2px(Vec(31.407, 69.749)), module, Mum::MUTE_BUTTON_LIGHT + 5));
		addChild(createLightCentered<LEDBezelLight<GreenLight>>(mm2px(Vec(31.407, 80.746)), module, Mum::MUTE_BUTTON_LIGHT + 6));
		addChild(createLightCentered<LEDBezelLight<GreenLight>>(mm2px(Vec(31.407, 91.744)), module, Mum::MUTE_BUTTON_LIGHT + 7));
		addChild(createLightCentered<LEDBezelLight<GreenLight>>(mm2px(Vec(31.407, 102.741)), module, Mum::MUTE_BUTTON_LIGHT + 8));
		addChild(createLightCentered<LEDBezelLight<GreenLight>>(mm2px(Vec(31.407, 113.739)), module, Mum::MUTE_BUTTON_LIGHT + 9));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.765, 14.761)), module, Mum::VOLTAGE_INPUT + 0));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.765, 25.759)), module, Mum::VOLTAGE_INPUT + 1));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.765, 36.756)), module, Mum::VOLTAGE_INPUT + 2));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.765, 47.754)), module, Mum::VOLTAGE_INPUT + 3));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.765, 58.751)), module, Mum::VOLTAGE_INPUT + 4));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.765, 69.749)), module, Mum::VOLTAGE_INPUT + 5));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.765, 80.746)), module, Mum::VOLTAGE_INPUT + 6));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.765, 91.744)), module, Mum::VOLTAGE_INPUT + 7));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.765, 102.741)), module, Mum::VOLTAGE_INPUT + 8));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(5.765, 113.739)), module, Mum::VOLTAGE_INPUT + 9));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.86, 14.761)), module, Mum::TRIGGER_INPUT + 0));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.86, 25.759)), module, Mum::TRIGGER_INPUT + 1));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.86, 36.756)), module, Mum::TRIGGER_INPUT + 2));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.86, 47.754)), module, Mum::TRIGGER_INPUT + 3));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.86, 58.751)), module, Mum::TRIGGER_INPUT + 4));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.86, 69.749)), module, Mum::TRIGGER_INPUT + 5));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.86, 80.746)), module, Mum::TRIGGER_INPUT + 6));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.86, 91.744)), module, Mum::TRIGGER_INPUT + 7));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.86, 102.741)), module, Mum::TRIGGER_INPUT + 8));
		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(22.86, 113.739)), module, Mum::TRIGGER_INPUT + 9));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(39.954, 14.761)), module, Mum::VOLTAGE_OUTPUT + 0));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(39.954, 25.759)), module, Mum::VOLTAGE_OUTPUT + 1));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(39.954, 36.756)), module, Mum::VOLTAGE_OUTPUT + 2));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(39.954, 47.754)), module, Mum::VOLTAGE_OUTPUT + 3));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(39.954, 58.751)), module, Mum::VOLTAGE_OUTPUT + 4));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(39.954, 69.749)), module, Mum::VOLTAGE_OUTPUT + 5));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(39.954, 80.746)), module, Mum::VOLTAGE_OUTPUT + 6));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(39.954, 91.744)), module, Mum::VOLTAGE_OUTPUT + 7));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(39.954, 102.741)), module, Mum::VOLTAGE_OUTPUT + 8));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(39.954, 113.739)), module, Mum::VOLTAGE_OUTPUT + 9));
	}
};


Model* modelMum = createModel<Mum, MumWidget>("Mum");