#include "SubmarineUtility.hpp"
#include "window.hpp"

namespace SubControls {

struct ButtonBase : Component {
	void onDragStart(EventDragStart &e) override {
		EventAction eAction;
		onAction(eAction);
	}
};

struct ButtonDragBase : Component {
	void onDragEnd(EventDragEnd &e) override {
		EventAction eAction;
		onAction(eAction);
	}
};

struct TextButton : ButtonBase {
	std::string label;
	void draw (NVGcontext *vg) override {
		nvgFontFaceId(vg, gGuiFont->handle);
		nvgFontSize(vg, 13);
		nvgFillColor(vg, nvgRGB(0xff, 0xff, 0xff));
		nvgTextAlign(vg, NVG_ALIGN_MIDDLE);
		nvgText(vg, 1, box.size.y / 2, label.c_str(), NULL);
		Component::draw(vg);
	}
};

struct TextDragButton : ButtonDragBase {
	std::string label;
	void draw (NVGcontext *vg) override {
		nvgFontFaceId(vg, gGuiFont->handle);
		nvgFontSize(vg, 13);
		nvgFillColor(vg, nvgRGB(0xff, 0xff, 0xff));
		nvgTextAlign(vg, NVG_ALIGN_MIDDLE);
		nvgText(vg, 1, box.size.y / 2, label.c_str(), NULL);
		Component::draw(vg);
	}
};

} // SubControls

struct ModBrowserWidget;

struct BrowserScroller : ScrollWidget {
	void draw (NVGcontext *vg) override {
		nvgBeginPath(vg);
		nvgRect(vg, 0, 0, box.size.x, box.size.y);
		nvgFillColor(vg, nvgRGB(0, 0, 0));
		nvgFill(vg);	
		ScrollWidget::draw(vg);
	}
};

struct PluginButton : SubControls::TextButton {
	ModBrowserWidget *mbw;
	Plugin *plugin;
	void onAction(EventAction &e) override;
};

struct ModelButton : SubControls::TextDragButton {
	ModBrowserWidget *mbw;
	Model *model;
	void onAction(EventAction &e) override;
};

struct PluginBackButton : SubControls::TextButton {
	ModBrowserWidget *mbw;
	void onAction(EventAction &e) override;
};

struct ModBrowserWidget : ModuleWidget {
	Widget *scrollContainer;
	ModBrowserWidget(Module *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/ModBrowser.svg")));
		BrowserScroller *scrollWidget = Widget::create<BrowserScroller>(Vec(10, 20));
		scrollWidget->box.size.x = box.size.x - 20;
		scrollWidget->box.size.y = box.size.y - 40;
		addChild(scrollWidget);
		scrollContainer = scrollWidget->container;
		AddPlugins();
	}
	void AddPlugins() {
		scrollContainer->clearChildren();
		unsigned int y = 0;
		for (Plugin *plugin : gPlugins) {
			PluginButton * pb = Widget::create<PluginButton>(Vec(0, y));
			pb->mbw = this;
			pb->plugin = plugin;
			pb->box.size.x = scrollContainer->box.size.x - 20;
			pb->box.size.y = 15;
			pb->label = plugin->slug;
			scrollContainer->addChild(pb);
			y += 15;
		} 
	}
	void AddModels(Plugin *plugin) {
		scrollContainer->clearChildren();
		PluginBackButton *pbb = Widget::create<PluginBackButton>(Vec(0, 0));
		pbb->mbw = this;
		pbb->box.size.x = scrollContainer->box.size.x - 20;
		pbb->box.size.y = 15;
		pbb->label = "\xe2\x86\x90 Back";
		scrollContainer->addChild(pbb);
		unsigned int y = 15;
		for (Model *model : plugin->models) {
			ModelButton *mb = Widget::create<ModelButton>(Vec(0, y));
			mb->mbw = this;
			mb->model = model;
			mb->box.size.x = scrollContainer->box.size.x - 20;
			mb->box.size.y = 15;
			mb->label = model->name;
			scrollContainer->addChild(mb);
			y += 15;
		}
	}
};

void PluginButton::onAction(EventAction &e) {
	mbw->AddModels(plugin);
}

void ModelButton::onAction(EventAction &e) {
	ModuleWidget *moduleWidget = model->createModuleWidget();
	if (!moduleWidget)
		return;
	gRackWidget->addModule(moduleWidget);
	moduleWidget->box.pos = gRackWidget->lastMousePos.minus(moduleWidget->box.size.div(2));
	gRackWidget->requestModuleBoxNearest(moduleWidget, moduleWidget->box);
}

void PluginBackButton::onAction(EventAction &e) {
	mbw->AddPlugins();
}

Model *modelModBrowser = Model::create<Module, ModBrowserWidget>("SubmarineUtility", "ModBrowser", "ModuleBrowser", UTILITY_TAG);
