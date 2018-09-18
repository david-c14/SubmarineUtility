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

struct WireManagerWidget : SubControls::SizeableModuleWidget {
	WMMinimizeIcon *minimizeIcon;
	int wireCount = 0;
	Widget *lastWire = NULL;
	WireManagerWidget(Module *module) : SubControls::SizeableModuleWidget(module) {
		moduleName = "Wire Manager";
	
		minimizeIcon = Widget::create<WMMinimizeIcon>(Vec(2, 2));
		minimizeIcon->wmw = this;
		minimizeIcon->setSVG(SVG::load(assetPlugin(plugin, "res/min.svg")));
		backPanel->addChild(minimizeIcon);	
	
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
		ModuleWidget::step();
	} 

};

// Icon onAction

void WMMinimizeIcon::onAction(EventAction &e) {
	wmw->Minimize(true);
}

Model *modelWireManager = Model::create<Module, WireManagerWidget>("Submarine (Utilities)", "WireManager", "Wire Manager", UTILITY_TAG);
