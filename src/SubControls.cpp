#include "SubControls.hpp"
#include "window.hpp"

namespace SubControls {

SizeableModuleWidget::SizeableModuleWidget(Module *module) : ModuleWidget(module) {
	box.size.x = moduleWidth;
	box.size.y = 380;
	handle = Widget::create<ModuleDragHandle>(Vec(box.size.x - 10, 175));
	handle->smw = this;
	addChild(handle);

	backPanel = Widget::create<BackPanel>(Vec(10, 15));
	backPanel->box.size.x = box.size.x = 20;
	backPanel->box.size.y = box.size.y - 30;
	addChild(backPanel);

	minimizeLogo = Widget::create<SubLogo>(Vec(0,0));
	minimizeLogo->setSVG(SVG::load(assetPlugin(plugin, "res/Sub2.svg")));
	minimizeLogo->visible = false;
	addChild(minimizeLogo);

	maximizeLogo = Widget::create<SubLogo>(Vec(moduleWidth - 20, 365));
	maximizeLogo->setSVG(SVG::load(assetPlugin(plugin, "res/Sub1.svg")));
	addChild(maximizeLogo);

	maximizeButton = Widget::create<MaximizeButton>(Vec(0, 175));
	maximizeButton->smw = this;
	maximizeButton->visible = false;
	addChild(maximizeButton);
}

void SizeableModuleWidget::Resize() {
	backPanel->box.size.x = box.size.x - 20;
	handle->box.pos.x = box.size.x - 10;
	maximizeLogo->box.pos.x = box.size.x - 20;
	onResize();
}

void SizeableModuleWidget::draw(NVGcontext *vg) {
	nvgBeginPath(vg);
	nvgRect(vg,0,0,box.size.x, box.size.y);
	nvgFillColor(vg,nvgRGB(0x29, 0x4f, 0x77));
	nvgFill(vg);
	nvgBeginPath(vg);
	nvgMoveTo(vg, 0, 0);
	nvgLineTo(vg, box.size.x, 0);
	nvgLineTo(vg, box.size.x - 1, 1);
	nvgLineTo(vg, 1, 1);
	nvgLineTo(vg, 1, box.size.y - 1);
	nvgLineTo(vg, 0, box.size.y);
	nvgClosePath(vg);
	nvgFillColor(vg, nvgRGB(0x31, 0xbe, 0xa5));
	nvgFill(vg);
	nvgMoveTo(vg, box.size.x, 0);
	nvgLineTo(vg, box.size.x, box.size.y);
	nvgLineTo(vg, 0, box.size.y);
	nvgLineTo(vg, 1, box.size.y - 1);
	nvgLineTo(vg, box.size.x - 1, box.size.y - 1);
	nvgLineTo(vg, box.size.x - 1, 1);
	nvgClosePath(vg);
	nvgFillColor(vg, nvgRGB(0x18, 0x2d, 0x44));
	nvgFill(vg);
	if (moduleWidth > 0) {
		nvgFontSize(vg, 14);
		nvgFontFaceId(vg, font->handle);
		nvgFillColor(vg, nvgRGBA(0x71, 0x9f, 0xcf, 0xff));
		nvgTextAlign(vg, NVG_ALIGN_LEFT);
		nvgText(vg, 3, 378, "submarine", NULL);
		nvgTextAlign(vg, NVG_ALIGN_CENTER);
		nvgText(vg, box.size.x / 2, 12, moduleName.c_str(), NULL);
	}
	else {
		nvgSave(vg);
		nvgRotate(vg, -M_PI / 2);
		nvgFontSize(vg, 14);
		nvgFontFaceId(vg, font->handle);
		nvgFillColor(vg, nvgRGBA(0x71, 0x9f, 0xcf, 0xff));
		nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
		nvgText(vg, -97.5, 7.5, moduleName.c_str(), NULL);
		nvgText(vg, -277.5, 7.5, "submarine", NULL);
		nvgRestore(vg);
	}
	ModuleWidget::draw(vg);
}

void SizeableModuleWidget::ShiftOthers(float delta) {
	if (!stabilized)
		return;
	if (delta == 0.0f)
		return;
	for (Widget *w : gRackWidget->moduleContainer->children) {
		if (this->box.pos.y != w->box.pos.y)
			continue;
		if (this->box.pos.x > w->box.pos.x)
			continue;
		if (this == w)
			continue;
		w->box.pos.x += delta;
	}
}

void SizeableModuleWidget::Minimize(unsigned int minimize) {
	float oldSize = box.size.x;
	if (minimize) {
		if (moduleWidth > 0)
			moduleWidth = -moduleWidth;
		box.size.x = 15;
		backPanel->visible = false;
		maximizeButton->visible = true;
		maximizeLogo->visible = false;
		minimizeLogo->visible = true;
		handle->visible = false;
		ShiftOthers(box.size.x - oldSize);
	}
	else {
		if (moduleWidth < 0)
			moduleWidth = -moduleWidth;
		ShiftOthers(moduleWidth - oldSize);
		box.size.x = moduleWidth;
		backPanel->visible = true;
		maximizeButton->visible = false;
		maximizeLogo->visible = true;
		minimizeLogo->visible = false;
		handle->visible = true;
		Resize();
	}
}

void ModuleDragHandle::onDragStart(EventDragStart &e) {
	dragX = gRackWidget->lastMousePos.x;
	originalBox = smw->box;
}

void ModuleDragHandle::onDragMove(EventDragMove &e) {

	float newDragX = gRackWidget->lastMousePos.x;
	float deltaX = newDragX - dragX;

	Rect newBox = originalBox;
	const float minWidth = 13 * RACK_GRID_WIDTH;
	newBox.size.x += deltaX;
	newBox.size.x = fmaxf(newBox.size.x, minWidth);
	newBox.size.x = roundf(newBox.size.x / RACK_GRID_WIDTH) * RACK_GRID_WIDTH;
	gRackWidget->requestModuleBox(smw, newBox);
	smw->moduleWidth = smw->box.size.x;
	smw->Resize();
}

void ModuleDragHandle::draw(NVGcontext *vg) {
	for (float x = 2.0; x <= 8.0; x += 2.0) {
		nvgBeginPath(vg);
		const float margin = 5.0;
		nvgMoveTo(vg, x + 0.5, margin + 0.5);
		nvgLineTo(vg, x + 0.5, box.size.y - margin + 0.5);
		nvgStrokeWidth(vg, 1.0);
		nvgStrokeColor(vg, nvgRGBAf(0.5, 0.5, 0.5, 0.5));
		nvgStroke(vg);
	}
}

void MaximizeButton::onAction(EventAction &e) {
	smw->Minimize(false);
}

} // SubControls
