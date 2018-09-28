#include "SubControls.hpp"
#include <map>
#include <algorithm>
#include "window.hpp"
#include "osdialog.h"

struct WireManagerWidget;

// Icons

struct WMIconWidget : SubControls::ButtonBase,SVGWidget {
	WireManagerWidget *wmw;
};

struct WMMinimizeIcon : WMIconWidget {
	WMMinimizeIcon() {
		box.size.x = 30;
		box.size.y = 30;
	}
	void onAction(EventAction &e) override;
};

struct WMColorIcon : WMIconWidget {
	WMColorIcon() {
		box.size.x = 30;
		box.size.y = 30;
	}
	void onAction(EventAction &e) override;
};

struct WMOptionIcon : WMIconWidget {
	WMOptionIcon() {
		box.size.x = 30;
		box.size.y = 30;
	}
	void onAction(EventAction &e) override;
};

struct WMHighlightButton : SubControls::RadioButton {
	WireManagerWidget *wmw;
	int status;
	void onAction(EventAction &e) override;	
};

struct WMWireCheck : SubControls::CheckButton {
	WireManagerWidget *wmw;
	void onAction(EventAction &e) override;
};

struct WMWireButton : VirtualWidget {
	NVGcolor color;
	WMWireCheck *wmc;
	WMWireButton() {
		wmc = Widget::create<WMWireCheck>(Vec(1,1));
		wmc->box.size.x = 19;
		wmc->box.size.y = 19;
		addChild(wmc);
	}
	void draw(NVGcontext *vg) override {
		NVGcolor colorOutline = nvgLerpRGBA(color, nvgRGBf(0.0, 0.0, 0.0), 0.5);

		nvgBeginPath(vg);
		nvgMoveTo(vg, 32, box.size.y / 2);
		nvgLineTo(vg, box.size.x, box.size.y / 2);
		nvgStrokeColor(vg, colorOutline);
		nvgStrokeWidth(vg, 5);
		nvgStroke(vg);

		nvgStrokeColor(vg, color);
		nvgStrokeWidth(vg, 3);
		nvgStroke(vg);

		nvgBeginPath(vg);
		nvgCircle(vg, 32, box.size.y / 2, 9);
		nvgFillColor(vg, color);
		nvgFill(vg);

		nvgStrokeWidth(vg, 1.0);
		nvgStrokeColor(vg, colorOutline);
		nvgStroke(vg);
	
		nvgBeginPath(vg);
		nvgCircle(vg, 32, box.size.y / 2, 5);
		nvgFillColor(vg, nvgRGBf(0.0, 0.0, 0.0));
		nvgFill(vg);

		Widget::draw(vg);
	}
};

struct WMCheckAll : SubControls::CheckButton {
	WireManagerWidget *wmw;
	void onAction(EventAction &e) override;
};

struct WMManageButton : VirtualWidget {
	WMCheckAll *wmc;
	WMManageButton() {
		wmc = Widget::create<WMCheckAll>(Vec(1,1));
		wmc->box.size.x = 19;
		wmc->box.size.y = 19;
		addChild(wmc);
	}
}; 

struct WireManagerWidget : SubControls::SizeableModuleWidget {

	enum {
		HIGHLIGHT_OFF,
		HIGHLIGHT_LOW,
		HIGHLIGHT_ON
	};

	WMColorIcon *colorIcon;
	WMOptionIcon *optionIcon;
	WMMinimizeIcon *minimizeIcon;
	ScrollWidget *colorWidget;
	Widget *optionWidget;
	WMHighlightButton *highlightOff;
	WMHighlightButton *highlightLow;
	WMHighlightButton *highlightOn;
	
	int wireCount = 0;
	Widget *lastWire = NULL;
	int highlight = 0;
	unsigned int newColorIndex = 1;

	WireManagerWidget(Module *module) : SubControls::SizeableModuleWidget(module) {
		moduleName = "Wire Manager";
		moduleWidth = 150;
		minimumWidth = 150;
		sizeable = false;
		box.size.x = moduleWidth;
		Resize();

		colorIcon = Widget::create<WMColorIcon>(Vec(2, 2));
		colorIcon->wmw = this;
		colorIcon->setSVG(SVG::load(assetPlugin(plugin, "res/colors.svg")));
		backPanel->addChild(colorIcon);

		optionIcon = Widget::create<WMOptionIcon>(Vec(34, 2));
		optionIcon->wmw = this;
		optionIcon->setSVG(SVG::load(assetPlugin(plugin, "res/hls.svg")));
		backPanel->addChild(optionIcon);
	
		minimizeIcon = Widget::create<WMMinimizeIcon>(Vec(66, 2));
		minimizeIcon->wmw = this;
		minimizeIcon->setSVG(SVG::load(assetPlugin(plugin, "res/min.svg")));
		backPanel->addChild(minimizeIcon);	

		colorWidget = Widget::create<ScrollWidget>(Vec(0,35));
		colorWidget->box.size.x = box.size.x - 20;
		colorWidget->box.size.y = box.size.y - 65;
		backPanel->addChild(colorWidget);

		WMManageButton *wb = Widget::create<WMManageButton>(Vec(0, 0));
		wb->wmc->wmw = this;
		wb->box.size.x = colorWidget->box.size.x;
		wb->box.size.y = 21;
		colorWidget->container->addChild(wb);

		optionWidget = Widget::create<Widget>(Vec(0, 35));
		optionWidget->box.size.x = box.size.x - 20;
		optionWidget->box.size.y = box.size.y - 65;
		optionWidget->visible = false;
		backPanel->addChild(optionWidget);

		SubControls::Label *label = Widget::create<SubControls::Label>(Vec(10, 5));
		label->label = "Highlighting";
		label->box.size.x = box.size.x - 10;
		label->box.size.y = 19;
		optionWidget->addChild(label);

		highlightOff = Widget::create<WMHighlightButton>(Vec(10, 25));
		highlightOff->wmw = this;
		highlightOff->status = HIGHLIGHT_OFF;
		highlightOff->box.size.x = box.size.x - 10;
		highlightOff->box.size.y = 19;
		highlightOff->selected = true;
		highlightOff->label = "Off";
		optionWidget->addChild(highlightOff);
	
		highlightLow = Widget::create<WMHighlightButton>(Vec(10, 45));
		highlightLow->wmw = this;
		highlightLow->status = HIGHLIGHT_LOW;
		highlightLow->box.size.x = box.size.x - 10;
		highlightLow->box.size.y = 19;
		highlightLow->label = "When hovering";
		optionWidget->addChild(highlightLow);
	
		highlightOn = Widget::create<WMHighlightButton>(Vec(10, 65));
		highlightOn->wmw = this;
		highlightOn->status = HIGHLIGHT_ON;
		highlightOn->box.size.x = box.size.x - 10;
		highlightOn->box.size.y = 19;
		highlightOn->label = "Always On";
		optionWidget->addChild(highlightOn);

		LoadSettings();

		wireCount = gRackWidget->wireContainer->children.size();
		if (wireCount)
			lastWire = gRackWidget->wireContainer->children.back();
	}

	void ResetIcons() {
	}

	void AddColor(NVGcolor color, int selected) {
		float y = colorWidget->container->children.size() * 21.0;
		WMWireButton *wb = Widget::create<WMWireButton>(Vec(0, y));
		wb->box.size.x = colorWidget->box.size.x;
		wb->box.size.y = 21;
		wb->color = color;
		wb->wmc->selected = selected;
		wb->wmc->wmw = this;
		colorWidget->container->addChild(wb);
	}

	void SetDefaults() {
		AddColor(nvgRGB(0xc9, 0xb7, 0x0e), true);
		AddColor(nvgRGB(0xc9, 0x18, 0x47), true);
		AddColor(nvgRGB(0x0c, 0x8e, 0x15), true);
		AddColor(nvgRGB(0x09, 0x86, 0xad), true);
		AddColor(nvgRGB(0xff, 0xae, 0xc9), false);
	}

	NVGcolor findColor() {
		auto vi = colorWidget->container->children.begin();
		std::advance(vi, newColorIndex);
		for (int i = 0; i < 2; i++) {
			while(newColorIndex < colorWidget->container->children.size()) {
				newColorIndex++;
				WMWireButton *wb = dynamic_cast<WMWireButton *>(*vi);
				if (wb->wmc->selected) {
					return wb->color;
				}
				std::advance(vi, 1);	
			}
			newColorIndex = 1;
			vi = colorWidget->container->children.begin();
			std::advance(vi, newColorIndex);
		}
		return nvgRGB(0x80, 0x80, 0x80);
	}

	void colorWire(Widget *widget) {
		WireWidget *wire = dynamic_cast<WireWidget *>(widget);
		wire->color = findColor();
	}

	void step() override {
		if (!stabilized) {
			wireCount = gRackWidget->wireContainer->children.size();
			stabilized = true;
		}
		int newSize = gRackWidget->wireContainer->children.size();
		if (newSize < wireCount) {
			wireCount = newSize;
			if (wireCount)
				lastWire = gRackWidget->wireContainer->children.back();
			else
				lastWire = NULL;
		}
		else if (newSize > wireCount) {
			std::list<Widget *>::reverse_iterator iterator = gRackWidget->wireContainer->children.rbegin();
			for (int i = 0; i < newSize - wireCount; i++) {
				colorWire(*iterator);
				++iterator;
			}
			wireCount = newSize;
			if (wireCount)
				lastWire = gRackWidget->wireContainer->children.back();
			else
				lastWire = NULL;
		}
		if (highlight)
			highlightWires();
	}
	void highlightWires() {
		ModuleWidget *focusedModuleWidget = nullptr;
		if (highlight) {
			if (gHoveredWidget) {
				focusedModuleWidget = dynamic_cast<ModuleWidget *>(gHoveredWidget);
				if (!focusedModuleWidget)
					focusedModuleWidget = gHoveredWidget->getAncestorOfType<ModuleWidget>();
			}
		}
		for (Widget *widget : gRackWidget->wireContainer->children) {
			WireWidget *wire = dynamic_cast<WireWidget *>(widget);
			if (focusedModuleWidget) {
				if (!wire->outputPort || !wire->inputPort) {
					wire->color = nvgTransRGBA(wire->color, 0xFF);
				}
				else if (wire->outputPort->getAncestorOfType<ModuleWidget>() == focusedModuleWidget) {
					wire->color = nvgTransRGBA(wire->color, 0xFF);
				}
				else if (wire->inputPort->getAncestorOfType<ModuleWidget>() == focusedModuleWidget) {
					wire->color = nvgTransRGBA(wire->color, 0xFF);
				}
				else {
					wire->color = nvgTransRGBA(wire->color, 0x10);
				}
			}
			else {
				if (highlight == 2)
					wire->color = nvgTransRGBA(wire->color, 0x10);
				else
					wire->color = nvgTransRGBA(wire->color, 0xFF);
			}
		}
		ModuleWidget::step();
	} 

	void Colors() {
		colorWidget->visible = true;
		optionWidget->visible = false;
	}

	void Options() {
		colorWidget->visible = false;
		optionWidget->visible = true;
	}

	void SetHighlight(int status) {
		highlightOff->selected = (highlightOff->status == status);
		highlightLow->selected = (highlightLow->status == status);
		highlightOn->selected = (highlightOn->status == status);
		highlight = status;
		highlightWires();
	}

	void SaveSettings() {
		json_t *settings = json_object();
		json_t *arr = json_array();
		auto __begin = std::begin(colorWidget->container->children);
		auto __end = std::end(colorWidget->container->children);
		for (++__begin; __begin != __end; ++__begin) {
			WMWireButton *wb = dynamic_cast<WMWireButton *>(*__begin);
			json_t *color = json_object();
			std::string s = colorToHexString(wb->color);	
			json_object_set_new(color, "color", json_string(s.c_str()));
			json_object_set_new(color, "selected", json_real(wb->wmc->selected));
			json_array_append_new(arr, color);
		}
		json_object_set_new(settings, "colors", arr);
		json_object_set_new(settings, "highlight", json_real(highlight));

		systemCreateDirectory(assetLocal("SubmarineUtility"));
		std::string settingsFilename = assetLocal("SubmarineUtility/WireManager.json");
		FILE *file = fopen(settingsFilename.c_str(), "w");
		if (file) {
			json_dumpf(settings, file, JSON_INDENT(2) | JSON_REAL_PRECISION(9));
			fclose(file);
		}
		json_decref(settings);
	}
	void LoadSettings() {
		json_error_t error;
		FILE *file = fopen(assetLocal("SubmarineUtility/WireManager.json").c_str(), "r");
		if (!file) {
			SetDefaults();
			return;
		}
		json_t *rootJ = json_loadf(file, 0, &error);
		fclose(file);
		if (!rootJ) {
			std::string message = stringf("Submarine Utilities Wire Manager: JSON parsing error at %s %d:%d %s", error.source, error.line, error.column, error.text);
			warn(message.c_str());
			return;
		}
		json_t *arr = json_object_get(rootJ, "colors");
		if (arr) {
			int size = json_array_size(arr);
			for (int i = 0; i < size; i++) {
				json_t *j1 = json_array_get(arr, i);
				if (j1) {
					json_t *c1 = json_object_get(j1, "color");
					if (c1) {
						int selected = false;
						json_t *s1 = json_object_get(j1, "selected");	
						if (s1) {
							selected = json_number_value(s1);
						}
						AddColor(colorFromHexString(json_string_value(c1)), selected);
					}	
				}
			}
		}
		json_t *h1 = json_object_get(rootJ, "highlight");
		if (h1) {
			highlight = json_number_value(h1);
			SetHighlight(highlight);
		}
		json_decref(rootJ);
	}
};

void WMWireCheck::onAction(EventAction &e) {
	selected = !selected;
	wmw->SaveSettings();
}

void WMCheckAll::onAction(EventAction &e) {
	selected = !selected;
	auto __begin = std::begin(wmw->colorWidget->container->children);
	auto __end = std::end(wmw->colorWidget->container->children);
	for (++__begin; __begin != __end; ++__begin) {
		WMWireButton *wb = dynamic_cast<WMWireButton *>(*__begin);
		wb->wmc->selected = selected;
	}
	wmw->SaveSettings();
}

// Icon onAction

void WMMinimizeIcon::onAction(EventAction &e) {
	wmw->Minimize(true);
}

void WMOptionIcon::onAction(EventAction &e) {
	wmw->Options();
}

void WMColorIcon::onAction(EventAction &e) {
	wmw->Colors();
}

void WMHighlightButton::onAction(EventAction &e) {
	wmw->SetHighlight(status);
	wmw->SaveSettings();
}

Model *modelWireManager = Model::create<Module, WireManagerWidget>("Submarine (Utilities)", "WireManager", "Wire Manager", UTILITY_TAG);
