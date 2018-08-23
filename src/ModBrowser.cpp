#include "SubmarineUtility.hpp"
#include <map>
#include <algorithm>
#include "window.hpp"
#include "osdialog.h"

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
	// Draw secondary text
		nvgFillColor(vg, nvgRGB(0x80, 0x80, 0x80));
		nvgTextAlign(vg, NVG_ALIGN_MIDDLE | NVG_ALIGN_RIGHT);
		nvgText(vg, box.size.x - 1, box.size.y / 2, label2.c_str(), NULL);
	// If text overlaps, feather out overlap
		if (label1Width + label2Width > box.size.x) {
			NVGpaint grad;
			if (gDraggedWidget == this) {
				nvgFillColor(vg, nvgRGB(0x20, 0x20, 0x20));
				grad = nvgLinearGradient(vg, label1Width, 0, label1Width + 10, 0, nvgRGBA(0x20, 0x20, 0x20, 0xff), nvgRGBA(0x20, 0x20, 0x20, 0));
			}
			else {
				nvgFillColor(vg, nvgRGB(0, 0, 0));
				grad = nvgLinearGradient(vg, label1Width, 0, label1Width + 10, 0, nvgRGBA(0, 0, 0, 0xff), nvgRGBA(0, 0, 0, 0));
			}
			nvgBeginPath(vg);
			nvgRect(vg, box.size.x - label2Width, 0, label1Width - box.size.x + label2Width, box.size.y);
			nvgFill(vg);
			nvgBeginPath(vg);
			nvgRect(vg, label1Width, 0, 10, box.size.y);
			nvgFillPaint(vg, grad);
			nvgFill(vg); 
		}
	// Draw primary text
		nvgFillColor(vg, nvgRGB(0xff, 0xff, 0xff));
		nvgTextAlign(vg, NVG_ALIGN_MIDDLE);
		nvgText(vg, 1, box.size.y / 2, label1.c_str(), NULL);
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
	LoadIcon *loadIcon;
	MinimizeIcon *minimizeIcon;
	MaximizeButton *maximizeButton;
	float width;
	std::list<std::shared_ptr<PluginElement>> pluginList;
	std::list<std::shared_ptr<TagElement>> tagList;
	std::list<std::shared_ptr<ModelElement>> modelList;
	std::shared_ptr<SVG> maximizedSVG;
	std::shared_ptr<SVG> minimizedSVG;
	std::string allfilters;
	ModBrowserWidget(Module *module) : ModuleWidget(module) {
		allfilters.assign(PATCH_FILTERS);
		allfilters.append(";");
		allfilters.append(PRESET_FILTERS);
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
	
		loadIcon = Widget::create<LoadIcon>(Vec(90, 0));
		loadIcon->mbw = this;
		backPanel->addChild(loadIcon);
	
		minimizeIcon = Widget::create<MinimizeIcon>(Vec(120, 0));
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
	void Load() {
		std::string dir;
		if (gRackWidget->lastPath.empty()) {
			dir = assetLocal("patches");
			systemCreateDirectory(dir);
		}
		else {
			dir = stringDirectory(gRackWidget->lastPath);
		}
		osdialog_filters *filters = osdialog_filters_parse(allfilters.c_str());
		debug("%s", dir.c_str());
		char *path = osdialog_file(OSDIALOG_OPEN, dir.c_str(), NULL, filters);
		debug("%s", path);
			
		if (path) {
			Load(path);
			gRackWidget->lastPath = path;
			free(path);
		}
		osdialog_filters_free(filters);
	}
	void Load(std::string filename) {
		FILE *file = fopen(filename.c_str(), "r");
		if (!file) {	
			debug("Unable to open patch %s", filename.c_str());
			return;
		}

		json_error_t error;
		json_t *rootJ = json_loadf(file, 0, &error);
		if (rootJ) {
			Load(rootJ);
			json_decref(rootJ);
		}
		else {
			std::string message = stringf("JSON parsing error at %s %d:%d %s", error.source, error.line, error.column, error.text);
			osdialog_message(OSDIALOG_WARNING, OSDIALOG_OK, message.c_str());
		}		
		fclose(file);
	}
	void LoadPreset(json_t *rootJ) {
		ModuleWidget *moduleWidget = gRackWidget->moduleFromJson(rootJ);
		if (moduleWidget) {
			moduleWidget->box.pos = gRackWidget->lastMousePos.minus(moduleWidget->box.size.div(2));
			gRackWidget->requestModuleBoxNearest(moduleWidget, moduleWidget->box);
		}
	}
	void Load(json_t *rootJ) {
		std::string message;
		Rect newBox;
		newBox.pos.x = -1;
		//load modules
		std::map<int, ModuleWidget *> moduleWidgets;
		json_t *modulesJ = json_object_get(rootJ, "modules");
		if (!modulesJ) {
			LoadPreset(rootJ);
			return;
		}
		std::vector<Widget *> existingWidgets;
		for (Widget *child : gRackWidget->moduleContainer->children) {
			existingWidgets.push_back(child);
		} 
		size_t moduleId;
		json_t *moduleJ;
		json_array_foreach(modulesJ, moduleId, moduleJ) {
			ModuleWidget *moduleWidget = gRackWidget->moduleFromJson(moduleJ);
			if (moduleWidget) {
				json_t *posJ = json_object_get(moduleJ, "pos");
				double x, y;
				json_unpack(posJ, "[F, F]", &x, &y);
				Vec pos = Vec(x,y);
				moduleWidget->box.pos = pos.mult(RACK_GRID_SIZE);
				moduleWidgets[moduleId] = moduleWidget;
				if (newBox.pos.x == -1) {
					newBox.pos.x = moduleWidget->box.pos.x;
					newBox.pos.y = moduleWidget->box.pos.y;
					newBox.size.x = moduleWidget->box.size.x;
					newBox.size.y = moduleWidget->box.size.y;
				}
				else {
					Rect mbox = moduleWidget->box;
					if (mbox.pos.x < newBox.pos.x) {
						newBox.size.x += newBox.pos.x - mbox.pos.x;
						newBox.pos.x = mbox.pos.x;
					}
					if (mbox.pos.y < newBox.pos.y) {
						newBox.size.y += newBox.pos.y - mbox.pos.y;
						newBox.pos.y = mbox.pos.y;
					}
					if ((mbox.pos.x + mbox.size.x) > (newBox.pos.x + newBox.size.x)) {
						newBox.size.x = mbox.pos.x + mbox.size.x - newBox.pos.x;
					}
					if ((mbox.pos.y + mbox.size.y) > (newBox.pos.y + newBox.size.y)) {
						newBox.size.y = mbox.pos.y + mbox.size.y - newBox.pos.y;
					}
				}
			}
		}
 
		//find space for modules and arrange
		Rect space = FindSpace(existingWidgets, newBox);
		if (space.pos.x == -1) {
			// oooh noes!!! couldn't find space for these widgets
			warn("Module browser could not find space to load patch");
			for (const auto& kvp : moduleWidgets) {
				gRackWidget->deleteModule(kvp.second);
				kvp.second->finalizeEvents();
				delete kvp.second;
			}
			return;
		}
		// Move modules into space
		float dx = space.pos.x - newBox.pos.x;
		float dy = space.pos.y - newBox.pos.y;
		for (const auto& kvp : moduleWidgets) {
			kvp.second->box.pos.x += dx;
			kvp.second->box.pos.y += dy;
		}
		//wires
		json_t *wiresJ = json_object_get(rootJ, "wires");
		if (!wiresJ) return;
		size_t wireId;
		json_t *wireJ;
		json_array_foreach(wiresJ, wireId, wireJ) {
			int outputModuleId = json_integer_value(json_object_get(wireJ, "outputModuleId"));
			int outputId = json_integer_value(json_object_get(wireJ, "outputId"));
			int inputModuleId = json_integer_value(json_object_get(wireJ, "inputModuleId"));
			int inputId = json_integer_value(json_object_get(wireJ, "inputId"));
			ModuleWidget *outputModuleWidget = moduleWidgets[outputModuleId];
			if (!outputModuleWidget) continue;
			ModuleWidget *inputModuleWidget = moduleWidgets[inputModuleId];
			if (!inputModuleWidget) continue;
			Port *outputPort = NULL;
			Port *inputPort = NULL;
			for (Port *port : outputModuleWidget->outputs) {
				if (port->portId == outputId) {
					outputPort = port;
					break;
				}
			}
			for (Port *port : inputModuleWidget->inputs) {
				if (port->portId == inputId) {
					inputPort = port;
					break;
				}
			}
			if (!outputPort || !inputPort)
				continue;
			WireWidget *wireWidget = new WireWidget();
			wireWidget->fromJson(wireJ);
			wireWidget->outputPort = outputPort;
			wireWidget->inputPort = inputPort;
			wireWidget->updateWire();
			gRackWidget->wireContainer->addChild(wireWidget);
		}

	}
	Rect FindSpace(std::vector<Widget *> existingWidgets, Rect box) {
		int x0 = roundf(box.pos.x / RACK_GRID_WIDTH);
		int y0 = roundf(box.pos.y / RACK_GRID_HEIGHT);
		std::vector<Vec> positions;
		for (int y = max(0, y0 - 8); y < y0 + 8; y++) {
			for (int x = max(0, x0 - 400); x < x0 + 400; x++) {
				positions.push_back(Vec(x * RACK_GRID_WIDTH, y * RACK_GRID_HEIGHT));
			}
		}
		std::sort(positions.begin(), positions.end(), [box](Vec a, Vec b) {
			return a.minus(box.pos).norm() < b.minus(box.pos).norm();
		});
		for (Vec position : positions) {
			Rect newBox = box;
			newBox.pos = position;
			int collide = false;
			for (Widget *child : existingWidgets) {
				if (newBox.intersects(child->box)) {
					collide = true;
					break;
				}		
			}
			if (!collide) {
				return newBox;
			}
		}
		Rect failed;
		failed.pos.x = -1;
		return failed;
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
	mbw->Load();
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
