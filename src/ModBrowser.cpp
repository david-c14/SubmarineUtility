#include "SubmarineUtility.hpp"
#include "window.hpp"

namespace SubControls {

struct ButtonBase : Component {
	void onDragEnd(EventDragEnd &e) override {
		EventAction eAction;
		onAction(eAction);
	}
};

struct TextButton : ButtonBase {
	std::string label;
	void draw (NVGcontext *vg) override {
		if (gDraggedWidget == this) {
			nvgBeginPath(vg);
			nvgRect(vg, 0, 0, box.size.x, box.size.y);
			nvgFillColor(vg, nvgRGB(0x20, 0x20, 0x20));
			nvgFill(vg);
		}
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

struct PluginIcon : SubControls::ButtonBase {
	ModBrowserWidget *mbw;
	int selected = 0;
	PluginIcon() {
		box.size.x = 30;
		box.size.y = 30;
	}
	void draw(NVGcontext *vg) override {
		nvgBeginPath(vg);
		nvgRoundedRect(vg, 2, 2, box.size.x - 4, box.size.y - 4, 3);
		nvgFillColor(vg, nvgRGB(0x20, 0x20, 0x20));
		nvgFill(vg);
		nvgBeginPath(vg);
		nvgRoundedRect(vg, 7, 15, 16, 10, 2);
		nvgRect(vg, 7, 15, 16, 3);
		nvgRoundedRect(vg, 9, 6, 3, 12, 2);
		nvgRoundedRect(vg, 18, 6, 3, 12, 2);
		nvgRect(vg, 14, 24, 2, 4);
		nvgFillColor(vg, nvgRGB(0x80, 0x80, 0x80));
		nvgFill(vg);

		Component::draw(vg);
	}
	void onAction(EventAction &e) override;
};

struct TagIcon : SubControls::ButtonBase {
	ModBrowserWidget *mbw;
	int selected = 0;
	TagIcon() {
		box.size.x = 30;
		box.size.y = 30;
	}
	void draw(NVGcontext *vg) override {
		nvgBeginPath(vg);
		nvgRoundedRect(vg, 2, 2, box.size.x - 4, box.size.y - 4, 3);
		nvgFillColor(vg, nvgRGB(0x20, 0x20, 0x20));
		nvgFill(vg);
		Component::draw(vg);
	}
	void onAction(EventAction &e) override;
};

struct FavIcon : SubControls::ButtonBase {
	ModBrowserWidget *mbw;
	int selected = 0;
	FavIcon() {
		box.size.x = 30;
		box.size.y = 30;
	}
	void draw(NVGcontext *vg) override {
		nvgBeginPath(vg);
		nvgRoundedRect(vg, 2, 2, box.size.x - 4, box.size.y - 4, 3);
		nvgFillColor(vg, nvgRGB(0x20, 0x20, 0x20));
		nvgFill(vg);
		Component::draw(vg);
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

struct PluginButton : SubControls::TextButton {
	ModBrowserWidget *mbw;
	Plugin *plugin;
	void onAction(EventAction &e) override;
};

struct TagButton : SubControls::TextButton {
	ModBrowserWidget *mbw;
	unsigned int tag;
	void onAction(EventAction &e) override;
};

struct ModelButton : SubControls::TextButton {
	ModBrowserWidget *mbw;
	Model *model;
	void onAction(EventAction &e) override;
};

struct PluginBackButton : SubControls::TextButton {
	ModBrowserWidget *mbw;
	void onAction(EventAction &e) override;
};

struct TagBackButton : SubControls::TextButton {
	ModBrowserWidget *mbw;
	void onAction(EventAction &e) override;
};

struct ModBrowserWidget : ModuleWidget {
	Widget *scrollContainer;
	PluginIcon *pluginIcon;
	TagIcon *tagIcon;
	FavIcon *favIcon;
	float width;
	ModBrowserWidget(Module *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "res/ModBrowser.svg")));
		pluginIcon = Widget::create<PluginIcon>(Vec(10, 20));
		pluginIcon->selected = 1;
		pluginIcon->mbw = this;
		addChild(pluginIcon);
		tagIcon = Widget::create<TagIcon>(Vec(40, 20));
		tagIcon->mbw = this;
		addChild(tagIcon);
		favIcon = Widget::create<FavIcon>(Vec(70, 20));
		favIcon->mbw = this;
		addChild(favIcon);
		BrowserScroller *scrollWidget = Widget::create<BrowserScroller>(Vec(10, 50));
		scrollWidget->box.size.x = box.size.x - 20;
		scrollWidget->box.size.y = box.size.y - 70;
		width = scrollWidget->box.size.y - 20;
		addChild(scrollWidget);
		scrollContainer = scrollWidget->container;
		AddPlugins();
	}
	void ResetIcons() {
		pluginIcon->selected = 0;
		tagIcon->selected = 0;
		favIcon->selected = 0;
	}
	void AddPlugins() {
		scrollContainer->clearChildren();
		unsigned int y = 0;
		for (Plugin *plugin : gPlugins) {
			PluginButton * pb = Widget::create<PluginButton>(Vec(0, y));
			pb->mbw = this;
			pb->plugin = plugin;
			pb->box.size.x = width;
			pb->box.size.y = 15;
			pb->label = plugin->slug;
			scrollContainer->addChild(pb);
			y += 15;
		} 
	}
	void AddTags() {
		scrollContainer->clearChildren();
		unsigned int y = 0;
		for (unsigned int i = 1; i < NUM_TAGS; i++) {
			TagButton *tb = Widget::create<TagButton>(Vec(0, y));
			tb->mbw = this;
			tb->tag = i;
			tb->box.size.x = width;
			tb->box.size.y = 15;
			tb->label = gTagNames[i];
			scrollContainer->addChild(tb);
			y += 15;
		}
	}
	void AddFavorites() {
		scrollContainer->clearChildren();
		unsigned int y = 0;
		FILE *file = fopen(assetLocal("settings.json").c_str(), "r");
		if (!file)
			return;
		json_error_t error;
		json_t *rootJ = json_loadf(file, 0, &error);
		if (!rootJ) {
			warn("JSON parsing error at %s %d:%d %s", error.source, error.line, error.column, error.text);
			fclose(file);
			return;
		}
		json_t *modb = json_object_get(rootJ, "moduleBrowser");
		if (modb) {
			json_t *favoritesJ = json_object_get(modb, "favorites");
			if (favoritesJ) {
				size_t i;
				json_t *favoriteJ;
				json_array_foreach(favoritesJ, i, favoriteJ) {
					json_t *pluginJ = json_object_get(favoriteJ, "plugin");
					json_t *modelJ = json_object_get(favoriteJ, "model");
					if (!pluginJ || !modelJ)
						continue;
					std::string pluginSlug = json_string_value(pluginJ);
					std::string modelSlug = json_string_value(modelJ);
					Model *model = pluginGetModel(pluginSlug, modelSlug);
					if (!model)
						continue;
					ModelButton *mb = Widget::create<ModelButton>(Vec(0, y));
					mb->mbw = this;
					mb->model = model;
					mb->box.size.x = width;
					mb->box.size.y = 15;
					mb->label = model->plugin->slug + " " + model->name;
					scrollContainer->addChild(mb);
					y += 15;	
				}
			}
		}
		json_decref(rootJ);
		fclose(file);
	}
	void AddModels(Plugin *plugin) {
		scrollContainer->clearChildren();
		PluginBackButton *pbb = Widget::create<PluginBackButton>(Vec(0, 0));
		pbb->mbw = this;
		pbb->box.size.x = width;
		pbb->box.size.y = 15;
		pbb->label = "\xe2\x86\x90 Back";
		scrollContainer->addChild(pbb);
		unsigned int y = 15;
		for (Model *model : plugin->models) {
			ModelButton *mb = Widget::create<ModelButton>(Vec(0, y));
			mb->mbw = this;
			mb->model = model;
			mb->box.size.x = width;
			mb->box.size.y = 15;
			mb->label = model->name;
			scrollContainer->addChild(mb);
			y += 15;
		}
	}
	void AddModels(unsigned int tag) {
		scrollContainer->clearChildren();
		TagBackButton *tbb = Widget::create<TagBackButton>(Vec(0, 0));
		tbb->mbw = this;
		tbb->box.size.x = width;
		tbb->box.size.y = 15;
		tbb->label = "\xe2\x86\x90 Back";
		scrollContainer->addChild(tbb);
		unsigned int y = 15;
		for (Plugin *plugin : gPlugins) {
			for (Model *model : plugin->models) {
				for (ModelTag mt : model->tags) {
					if (mt == tag) {
						ModelButton *mb = Widget::create<ModelButton>(Vec(0, y));
						mb->mbw = this;
						mb->model = model;
						mb->box.size.x = width;
						mb->box.size.y = 15;
						mb->label = model->name;
						scrollContainer->addChild(mb);
						y += 15;
					}
				} 
			}
		}
	}
};

void PluginIcon::onAction(EventAction &e) {
	mbw->ResetIcons();
	mbw->pluginIcon->selected = 1;
	mbw->AddPlugins();
}

void TagIcon::onAction(EventAction &e) {
	mbw->ResetIcons();
	mbw->tagIcon->selected = 1;
	mbw->AddTags();
}

void FavIcon::onAction(EventAction &e) {
	mbw->pluginIcon->selected = 0;
	mbw->favIcon->selected = 1;
	mbw->AddFavorites();
}

void PluginButton::onAction(EventAction &e) {
	mbw->AddModels(plugin);
}

void TagButton::onAction(EventAction &e) {
	mbw->AddModels(tag);
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

void TagBackButton::onAction(EventAction &e) {
	mbw->AddTags();
}

Model *modelModBrowser = Model::create<Module, ModBrowserWidget>("SubmarineUtility", "ModBrowser", "ModuleBrowser", UTILITY_TAG);
