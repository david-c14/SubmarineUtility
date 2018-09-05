#include "SubmarineUtility.hpp"
#include "window.hpp"

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

struct SubLogo : SVGWidget{};

struct ModuleDragHandle;
struct SizeableModuleWidget;

struct MaximizeButton : ButtonBase {
	SizeableModuleWidget *smw;
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

struct SizeableModuleWidget : ModuleWidget {
	float moduleWidth = 300.0f;
	unsigned int stabilized = false;
	ModuleDragHandle *handle;
	BackPanel *backPanel;
	SubLogo *minimizeLogo;
	SubLogo *maximizeLogo;
	MaximizeButton *maximizeButton;	
	std::shared_ptr<Font> font = Font::load(assetGlobal("res/fonts/DejaVuSans.ttf"));
	std::string moduleName;

	SizeableModuleWidget(Module *module); 
	void draw(NVGcontext *vg) override;
	void Resize();
	void Minimize(unsigned int minimize);
	void ShiftOthers(float delta);

	virtual void onResize() { }; 
};

struct ModuleDragHandle : Widget {
	SizeableModuleWidget *smw;
	float dragX;
	Rect originalBox;
	ModuleDragHandle() {
		box.size = Vec(10, 30);
	}
	void onMouseDown(EventMouseDown &e) override {
		if (e.button == 0) {
			e.consumed = true;
			e.target = this;
		}
	}
	void draw(NVGcontext *vg) override;
	void onDragStart(EventDragStart &e) override;
	void onDragMove(EventDragMove &e) override;
};

} // SubControls
