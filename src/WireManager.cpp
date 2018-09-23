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

struct WMRadioButton : SubControls::ButtonBase {
	std::string label;
	int selected = false;
	WireManagerWidget *wmw;
	void draw (NVGcontext *vg) override {
		nvgFontFaceId(vg, gGuiFont->handle);
		nvgFontSize(vg, 13);
	// Draw primary text
		nvgFillColor(vg, nvgRGB(0xff, 0xff, 0xff));
		nvgTextAlign(vg, NVG_ALIGN_MIDDLE);
		nvgText(vg, 21, box.size.y / 2, label.c_str(), NULL);
		Component::draw(vg);
	}
};

struct HighlightOffButton : WMRadioButton {
	void onAction(EventAction &e) override;	
};

struct HighlightLowButton : WMRadioButton {
	void onAction(EventAction &e) override;	
};

struct HighlightOnButton : WMRadioButton {
	void onAction(EventAction &e) override;	
};

struct WireManagerWidget : SubControls::SizeableModuleWidget {
	WMColorIcon *colorIcon;
	WMOptionIcon *optionIcon;
	WMMinimizeIcon *minimizeIcon;
	ScrollWidget *colorWidget;
	Widget *optionWidget;
	HighlightOffButton *highlightOff;
	HighlightLowButton *highlightLow;
	HighlightOnButton *highlightOn;
	
	int wireCount = 0;
	Widget *lastWire = NULL;
	WireManagerWidget(Module *module) : SubControls::SizeableModuleWidget(module) {
		moduleName = "Wire Manager";

		colorIcon = Widget::create<WMColorIcon>(Vec(2, 2));
		colorIcon->wmw = this;
		colorIcon->setSVG(SVG::load(assetPlugin(plugin, "res/min.svg")));
		backPanel->addChild(colorIcon);

		optionIcon = Widget::create<WMOptionIcon>(Vec(34, 2));
		optionIcon->wmw = this;
		optionIcon->setSVG(SVG::load(assetPlugin(plugin, "res/min.svg")));
		backPanel->addChild(optionIcon);
	
		minimizeIcon = Widget::create<WMMinimizeIcon>(Vec(66, 2));
		minimizeIcon->wmw = this;
		minimizeIcon->setSVG(SVG::load(assetPlugin(plugin, "res/min.svg")));
		backPanel->addChild(minimizeIcon);	

		colorWidget = Widget::create<ScrollWidget>(Vec(0,35));
		colorWidget->box.size.x = box.size.x - 20;
		colorWidget->box.size.y = box.size.y - 65;
		backPanel->addChild(colorWidget);

		optionWidget = Widget::create<Widget>(Vec(0, 35));
		optionWidget->box.size.x = box.size.x - 20;
		optionWidget->box.size.y = box.size.y - 65;
		optionWidget->visible = false;
		backPanel->addChild(optionWidget);

debug("1");
		highlightOff = Widget::create<HighlightOffButton>(Vec(5, 5));
		highlightOff->wmw = this;
		highlightOff->box.size.x = box.size.x - 10;
		highlightOff->box.size.y = 19;
		highlightOff->selected = true;
		highlightOff->label = "Highlighting Off";
		optionWidget->addChild(highlightOff);
	
debug("2");
		highlightLow = Widget::create<HighlightLowButton>(Vec(5, 25));
		highlightLow->wmw = this;
		highlightLow->box.size.x = box.size.x - 10;
		highlightLow->box.size.y = 19;
		highlightLow->label = "Lowlighting On";
		optionWidget->addChild(highlightLow);
	
debug("3");
		highlightOn = Widget::create<HighlightOnButton>(Vec(5, 45));
		highlightOn->wmw = this;
		highlightOn->box.size.x = box.size.x - 10;
		highlightOn->box.size.y = 19;
		highlightOn->label = "Highlighting On";
		optionWidget->addChild(highlightOn);
	
debug("4");
		wireCount = gRackWidget->wireContainer->children.size();
		if (wireCount)
			lastWire = gRackWidget->wireContainer->children.back();
	}
	void ResetIcons() {
	}

	void onResize() override {
	} 

	void colorWire(Widget *widget) {
		WireWidget *wire = dynamic_cast<WireWidget *>(widget);
		NVGcolor newColor = nvgRGB(0xff, 0xae, 0xc9);
		wire->color = newColor;
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
		ModuleWidget *focusedModuleWidget = nullptr;
		if (gHoveredWidget) {
			focusedModuleWidget = dynamic_cast<ModuleWidget *>(gHoveredWidget);
			if (!focusedModuleWidget)
				focusedModuleWidget = gHoveredWidget->getAncestorOfType<ModuleWidget>();
		}
		for (Widget *widget : gRackWidget->wireContainer->children) {
			WireWidget *wire = dynamic_cast<WireWidget *>(widget);
			if (focusedModuleWidget) {
				if (wire->outputPort && wire->outputPort->getAncestorOfType<ModuleWidget>() == focusedModuleWidget) {
					wire->color = nvgTransRGBA(wire->color, 0xFF);
				}
				else if (wire->inputPort && wire->inputPort->getAncestorOfType<ModuleWidget>() == focusedModuleWidget) {
					wire->color = nvgTransRGBA(wire->color, 0xFF);
				}
				else {
					wire->color = nvgTransRGBA(wire->color, 0x10);
				}
			}
			else {
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

	void HighlightOff() {
		highlightOn->selected = false;
		highlightOff->selected = true;
		highlightLow->selected = false;		
	}

	void HighlightLow() {
		highlightOn->selected = false;
		highlightOff->selected = false;
		highlightLow->selected = true;
	}

	void HighlightOn() {
		highlightOn->selected = true;
		highlightOff->selected = false;
		highlightLow->selected = false;
	}

};

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

void HighlightOffButton::onAction(EventAction &e) {
	wmw->HighlightOff();
}

void HighlightLowButton::onAction(EventAction &e) {
	wmw->HighlightLow();
}

void HighlightOnButton::onAction(EventAction &e) {
	wmw->HighlightOn();
}

Model *modelWireManager = Model::create<Module, WireManagerWidget>("Submarine (Utilities)", "WireManager", "Wire Manager", UTILITY_TAG);
