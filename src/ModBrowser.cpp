#include "SubmarineUtility.hpp"
#include "window.hpp"

struct ModBrowserWidget;
struct ListElement {
	ModBrowserWidget *mbw;
	virtual void onAction(EventAction &e) { debug ("Not Implemented"); }
	virtual std::string GetLabelOne() { return std::string("Label 1"); };
	virtual std::string GetLabelTwo() { return std::string("Label 2"); };
};

namespace SubControls {

struct BackPanel : OpaqueWidget {
	void draw (NVGcontext *vg) override {
		nvgBeginPath(vg);
		nvgRect(vg, 0, 0, box.size.x, box.size.y);
		nvgFillColor(vg, nvgRGB(0, 0, 0));
		nvgFill(vg);
		OpaqueWidget::draw(vg);
	}
};

struct ButtonBase : Component {
	void onDragEnd(EventDragEnd &e) override {
		EventAction eAction;
		onAction(eAction);
	}
};

} // SubControls

struct TextButton : SubControls::ButtonBase {
	std::string label1;
	std::string label2;
	std::shared_ptr<ListElement> element;
	float label1Width = 0;
	float label2Width = 0;	
	void CalculateSizes(NVGcontext *vg) {
		nvgFontFaceId(vg, gGuiFont->handle);
		nvgFontSize(vg, 13);
		float bounds[4];
		nvgTextBounds(vg, 1, box.size.y / 2, label1.c_str(), NULL, bounds);
		label1Width = bounds[2] - bounds[0];
		nvgTextBounds(vg, 1, box.size.y / 2, label2.c_str(), NULL, bounds);
		label2Width = bounds[2] - bounds[0];	
		debug("%s %f %f", label1.c_str(), label1Width, label2Width);
	}
	void draw (NVGcontext *vg) override {
		if (label1Width == 0.0f)
			CalculateSizes(vg);
		if (gDraggedWidget == this) {
			nvgBeginPath(vg);
			nvgRect(vg, 0, 0, box.size.x - 2, box.size.y);
			nvgFillColor(vg, nvgRGB(0x20, 0x20, 0x20));
			nvgFill(vg);
		}
		nvgFontFaceId(vg, gGuiFont->handle);
		nvgFontSize(vg, 13);
		nvgFillColor(vg, nvgRGB(0xff, 0xff, 0xff));
		nvgTextAlign(vg, NVG_ALIGN_MIDDLE);
		nvgText(vg, 1, box.size.y / 2, label1.c_str(), NULL);
		nvgFillColor(vg, nvgRGB(0x80, 0x80, 0x80));
		nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_RIGHT);
		nvgText(vg, box.size.x - 1, box.size.y / 2, label2.c_str(), NULL);
		Component::draw(vg);
	}
	void GetLabels() {
		label1 = element->GetLabelOne();
		label2 = element->GetLabelTwo();
	}
	void onAction(EventAction &e) override {
		element->onAction(e);
	}
};

// Icons

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

struct LoadIcon : SubControls::ButtonBase {
	ModBrowserWidget *mbw;
	int selected = 0;
	LoadIcon() {
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

struct MinimizeIcon : SubControls::ButtonBase {
	ModBrowserWidget *mbw;
	MinimizeIcon() {
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

struct MaximizeButton : SubControls::ButtonBase {
	ModBrowserWidget *mbw;
	MaximizeButton() {
		box.size.x = 15;
		box.size.y = 30;
	}
	void draw(NVGcontext *vg) override {
		nvgBeginPath(vg);
		nvgMoveTo(vg, 2, 4);
		nvgLineTo(vg, 13, 15);
		nvgLineTo(vg, 2, 26);
		nvgClosePath(vg);
		nvgFillColor(vg, nvgRGB(0x71, 0x9f, 0xcf));
		nvgFill(vg);
		Component::draw(vg);
	}
	void onAction(EventAction &e) override;
};

// Elements

struct PluginElement : ListElement {
	Plugin *plugin;
	std::string GetLabelOne() override {
		return plugin->slug;
	}
	std::string GetLabelTwo() override {
		return plugin->slug;
	} 
	void onAction(EventAction &e) override;
};

struct TagElement : ListElement {
	unsigned int tag;
	std::string GetLabelOne() override {
		return gTagNames[tag];
	}
	std::string GetLabelTwo() override {
		return gTagNames[tag];
	}
	void onAction(EventAction &e) override;
};

struct ModelElement : ListElement {
	Model *model;
	std::string GetLabelOne() override {
		return model->name;
	}
	std::string GetLabelTwo() override {
		return model->plugin->slug;
	}
	void onAction(EventAction &e) override;
};

struct PluginBackElement : ListElement {
	std::string label2;
	std::string GetLabelOne() override {
		return std::string("\xe2\x86\x90 Back");
	}
	std::string GetLabelTwo() override {
		return label2;
	}
	void onAction(EventAction &e) override;
};

struct TagBackElement : ListElement {
	std::string label2;
	std::string GetLabelOne() override {
		return std::string("\xe2\x86\x90 Back");
	}
	std::string GetLabelTwo() override {
		return label2;
	}
	void onAction(EventAction &e) override;
};

struct ModBrowserWidget : ModuleWidget {
	SubControls::BackPanel *backPanel;
	Widget *scrollContainer;
	PluginIcon *pluginIcon;
	TagIcon *tagIcon;
	FavIcon *favIcon;
	MinimizeIcon *minimizeIcon;
	MaximizeButton *maximizeButton;
	float width;
	std::list<std::shared_ptr<PluginElement>> pluginList;
	std::list<std::shared_ptr<TagElement>> tagList;
	std::list<std::shared_ptr<ModelElement>> modelList;
	std::shared_ptr<SVG> maximizedSVG;
	std::shared_ptr<SVG> minimizedSVG;
	ModBrowserWidget(Module *module) : ModuleWidget(module) {
		maximizedSVG = SVG::load(assetPlugin(plugin, "res/ModBrowser.svg"));
		minimizedSVG = SVG::load(assetPlugin(plugin, "res/ModBrowserMin.svg"));	
		setPanel(maximizedSVG);
		
		maximizeButton = Widget::create<MaximizeButton>(Vec(0, 30));
		maximizeButton->visible = false;
		maximizeButton->mbw = this;
		addChild(maximizeButton);
		
		backPanel = Widget::create<SubControls::BackPanel>(Vec(10, 20));
		backPanel->box.size.x = box.size.x - 20;
		backPanel->box.size.y = box.size.y - 40;
		addChild(backPanel);

		pluginIcon = Widget::create<PluginIcon>(Vec(0, 0));
		pluginIcon->selected = 1;
		pluginIcon->mbw = this;
		backPanel->addChild(pluginIcon);

		tagIcon = Widget::create<TagIcon>(Vec(30, 0));
		tagIcon->mbw = this;
		backPanel->addChild(tagIcon);

		favIcon = Widget::create<FavIcon>(Vec(60, 0));
		favIcon->mbw = this;
		backPanel->addChild(favIcon);
	
		minimizeIcon = Widget::create<MinimizeIcon>(Vec(90, 0));
		minimizeIcon->mbw = this;
		backPanel->addChild(minimizeIcon);	

		ScrollWidget *scrollWidget = Widget::create<ScrollWidget>(Vec(0, 30));
		scrollWidget->box.size.x = box.size.x - 20;
		scrollWidget->box.size.y = box.size.y - 70;
		width = scrollWidget->box.size.x - 20;
		backPanel->addChild(scrollWidget);

		scrollContainer = scrollWidget->container;
		for (Plugin *plugin : gPlugins) {
			std::shared_ptr<PluginElement> pe = std::make_shared<PluginElement>();
			pe->mbw = this;
			pe->plugin = plugin;
			pluginList.push_back(pe);
		}
		for (unsigned int i = 1; i < NUM_TAGS; i++) {
			std::shared_ptr<TagElement> te = std::make_shared<TagElement>();
			te->mbw = this;
			te->tag = i;
			tagList.push_back(te);
		}
		for (Plugin *plugin : gPlugins) {
			for (Model *model : plugin->models) {
				std::shared_ptr<ModelElement> me = std::make_shared<ModelElement>();
				me->mbw = this;
				me->model = model;
				modelList.push_back(me);
			}
		}
		
		AddPlugins();
	}
	void ResetIcons() {
		pluginIcon->selected = 0;
		tagIcon->selected = 0;
		favIcon->selected = 0;
	}
	void SetListWidth() {
		float width = scrollContainer->parent->box.size.x;
		if (scrollContainer->children.size() * 15 > scrollContainer->parent->box.size.y)
			width -= 13;
		for (Widget *w : scrollContainer->children) {
			w->box.size.x = width;
		}
	}
	void AddElement(std::shared_ptr<ListElement> le, float y) {
		TextButton *tb = Widget::create<TextButton>(Vec(0, y));
		tb->element = le;
		tb->GetLabels();
		tb->box.size.x = width;
		tb->box.size.y = 15;
		scrollContainer->addChild(tb);
	}
	void AddPlugins() {
		scrollContainer->clearChildren();
		unsigned int y = 0;
		for (std::shared_ptr<PluginElement> pe : pluginList) {
			AddElement(pe, y);			
			y += 15;
		}
		SetListWidth();
	}
	void AddTags() {
		scrollContainer->clearChildren();
		unsigned int y = 0;
		for (std::shared_ptr<TagElement> te : tagList) {
			AddElement(te, y);
			y += 15;
		}
		SetListWidth();
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
					for (std::shared_ptr<ModelElement> me : modelList) {
						if (me->model == model) {
							AddElement(me, y);
							y += 15;
						}
					}
				}
			}
		}
		json_decref(rootJ);
		fclose(file);
		SetListWidth();
	}
	void AddModels(Plugin *plugin) {
		scrollContainer->clearChildren();
		std::shared_ptr<PluginBackElement> pbe = std::make_shared<PluginBackElement>();	
		pbe->mbw = this;
		pbe->label2 = plugin->slug;
		AddElement(pbe, 0);
		unsigned int y = 15;
		for (std::shared_ptr<ModelElement> me : modelList) {
			if (me->model->plugin == plugin) {
				AddElement(me, y);
				y += 15;
			}
		}
		SetListWidth();
	}
	void AddModels(unsigned int tag) {
		scrollContainer->clearChildren();
		std::shared_ptr<TagBackElement> tbe = std::make_shared<TagBackElement>();
		tbe->mbw = this;
		tbe->label2 = gTagNames[tag];
		AddElement(tbe, 0);
		unsigned int y = 15;
		for (std::shared_ptr<ModelElement> me : modelList) {
			for (ModelTag mt : me->model->tags) {
				if (mt == tag) {
					AddElement(me, y);
					y += 15;
				}
			}
		}	
		SetListWidth();
	}
	void Minimize(unsigned int minimize) {
		if (minimize) {
			panel->setBackground(minimizedSVG);
			box.size.x = panel->box.size.x;
			panel->dirty = true;
			backPanel->visible = false;
			maximizeButton->visible = true;
		}
		else {
			panel->setBackground(maximizedSVG);
			box.size.x = panel->box.size.x;
			panel->dirty = true;
			backPanel->visible = true;
			maximizeButton->visible = false;
		}
	}
};

// Icon onAction

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

void LoadIcon::onAction(EventAction &e) {
}

void MinimizeIcon::onAction(EventAction &e) {
	mbw->Minimize(true);
}

void MaximizeButton::onAction(EventAction &e) {
	mbw->Minimize(false);
}

// Element onAction

void PluginElement::onAction(EventAction &e) {
	mbw->AddModels(plugin);
}

void TagElement::onAction(EventAction &e) {
	mbw->AddModels(tag);
}

void ModelElement::onAction(EventAction &e) {
	ModuleWidget *moduleWidget = model->createModuleWidget();
	if (!moduleWidget)
		return;
	gRackWidget->addModule(moduleWidget);
	moduleWidget->box.pos = gRackWidget->lastMousePos.minus(moduleWidget->box.size.div(2));
	gRackWidget->requestModuleBoxNearest(moduleWidget, moduleWidget->box);
}

void PluginBackElement::onAction(EventAction &e) {
	mbw->AddPlugins();
}

void TagBackElement::onAction(EventAction &e) {
	mbw->AddTags();
}

Model *modelModBrowser = Model::create<Module, ModBrowserWidget>("SubmarineUtility", "ModBrowser", "ModuleBrowser", UTILITY_TAG);
