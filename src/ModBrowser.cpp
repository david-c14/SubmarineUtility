#include "SubmarineUtility.hpp"

struct ModBrowserWidget;

struct MyButton : PB61303Button {

	ModBrowserWidget * mw;
	Plugin *plugin;
	MyButton() {
		setSVGs(SVG::load(assetGlobal("res/ComponentLibrary/CKSS_0.svg")),NULL);
	}
	void onAction(EventAction &e) override; 
};

struct ModBrowserWidget : ModuleWidget {
	ModBrowserWidget(Module *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/LA-108.svg")));
		unsigned int y = 20;
		ScrollWidget *scrollWidget = Widget::create<ScrollWidget>(Vec(10, 150));
		scrollWidget->box.size.x = 200;
		scrollWidget->box.size.y = 100;
		addChild(scrollWidget);
		for (Plugin *plugin : gPlugins) {
			debug("%s", plugin->slug.c_str());
			Label *slug = Widget::create<Label>(Vec(40,y));
			slug->text = plugin->slug;
			scrollWidget->addChild(slug);
			MyButton *button = Widget::create<MyButton>(Vec(10, y));
			button->mw = this;
			button->plugin = plugin;
			scrollWidget->container->addChild(button);
			y += 30;
		}

	}
};

void MyButton::onAction(EventAction &e) {
	debug("Clicked");
	for (Model *model : plugin->models) {
		debug("%s", model->slug.c_str());
	}
}


Model *modelModBrowser = Model::create<Module, ModBrowserWidget>("SubmarineUtility", "ModBrowser", "ModuleBrowser", UTILITY_TAG);
