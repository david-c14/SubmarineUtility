#include "SubmarineUtility.hpp"

struct ModBrowserWidget;

struct MyButton : PB61303Button {

	ModBrowserWidget * mw;
	Plugin *plugin;
	Model *model;
	MyButton() {
		setSVGs(SVG::load(assetGlobal("res/ComponentLibrary/CKSS_0.svg")),NULL);
	}
	void onAction(EventAction &e) override; 
};

struct BrowserScroller : ScrollWidget {
	void draw (NVGcontext *vg) override {
		nvgBeginPath(vg);
		nvgRect(vg, 0, 0, box.size.x, box.size.y);
		nvgFillColor(vg, nvgRGB(0, 0, 0));
		nvgFill(vg);	
		ScrollWidget::draw(vg);
	}
};

struct ModBrowserWidget : ModuleWidget {
	Widget *scrollContainer;
	ModBrowserWidget(Module *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/LA-108.svg")));
		unsigned int y = 20;
		BrowserScroller *scrollWidget = Widget::create<BrowserScroller>(Vec(10, 20));
		scrollWidget->box.size.x = box.size.x - 20;
		scrollWidget->box.size.y = box.size.y - 40;
		addChild(scrollWidget);
		scrollContainer = scrollWidget->container;
		for (Plugin *plugin : gPlugins) {
			debug("%s", plugin->slug.c_str());
			Label *slug = Widget::create<Label>(Vec(40,y));
			slug->text = plugin->slug;
			scrollWidget->container->addChild(slug);
			MyButton *button = Widget::create<MyButton>(Vec(10, y));
			button->mw = this;
			button->plugin = plugin;
			scrollWidget->container->addChild(button);
			y += 30;
		}

	}
};

void MyButton::onAction(EventAction &e) {
	if (plugin) {
		while (mw->scrollContainer->children.size()) {
			Widget * w = mw->scrollContainer->children.front();
			mw->scrollContainer->removeChild(w);
	//		delete w;
	};
		unsigned int y = 20;
		for (Model *model : plugin->models) {
			Label *slug = Widget::create<Label>(Vec(40,y));	
			slug->text = model->slug;
			mw->scrollContainer->addChild(slug);
			MyButton *button = Widget::create<MyButton>(Vec(10,y));
			button->mw = mw;
			button->model = model;
			mw->scrollContainer->addChild(button);
			y += 30;
		}
	}
	if (model) {
		ModuleWidget *moduleWidget = model->createModuleWidget();
		if (!moduleWidget)
			return;
		gRackWidget->addModule(moduleWidget);
		// Move module nearest to the mouse position
		moduleWidget->box.pos = gRackWidget->lastMousePos.minus(moduleWidget->box.size.div(2));
		gRackWidget->requestModuleBoxNearest(moduleWidget, moduleWidget->box);
	}
	//for (Widget *widget : gRackWidget->wireContainer->children) {
		//WireWidget *wireWidget = dynamic_cast<WireWidget *>(widget);
		//wireWidget->color = nvgRGB(0xff, 0xc0, 0xc0);
	//}
}


Model *modelModBrowser = Model::create<Module, ModBrowserWidget>("SubmarineUtility", "ModBrowser", "ModuleBrowser", UTILITY_TAG);
