#include "ModularFungi.hpp"

struct MEBitMap : TransparentWidget {
	std::string path1;
	std::string path2;
	std::string fontPath;
	int loaded = false;
	int bitmap1 = 0;
	int bitmap2 = 0;
	NVGcontext *storedVG;
	int font;
	void DrawImage(NVGcontext *vg) {
		storedVG = vg;	
		if (!loaded) {
			loaded = true;
			bitmap1 = nvgCreateImage(vg, path1.c_str(), NVG_IMAGE_GENERATE_MIPMAPS | NVG_IMAGE_REPEATX | NVG_IMAGE_REPEATY | NVG_IMAGE_NEAREST);
			bitmap2 = nvgCreateImage(vg, path2.c_str(), NVG_IMAGE_GENERATE_MIPMAPS | NVG_IMAGE_REPEATX | NVG_IMAGE_REPEATY | NVG_IMAGE_NEAREST);
			if (!bitmap1)
				warn("ModularFungi: Unable to load %s", path1.c_str());
			font = nvgCreateFont(vg, "DJ", fontPath.c_str());
		}
		if (!bitmap1)
			return;	
		NVGpaint paint1 = nvgImagePattern(vg, 0, 0, 140, 256, 0.0f, bitmap1, 1.0f);
		NVGpaint paint2 = nvgImagePattern(vg, 0, 0, 130, 256, 0.0f, bitmap2, 1.0f);
		nvgSave(vg);

		nvgFontSize(vg, 150);
		nvgFontFaceId(vg, font);
		nvgFillColor(vg, nvgRGBA(0xff, 0xff, 0xff, 0xff));
		nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
		nvgText(vg, box.size.x / 2, box.size.y / 2 - 70, "OMRI", NULL);
		nvgText(vg, box.size.x / 2, box.size.y / 2 + 70, "COHEN", NULL);
		

		nvgGlobalCompositeBlendFuncSeparate(vg, NVG_ONE, NVG_ZERO, NVG_DST_ALPHA, NVG_ZERO);
		nvgBeginPath(vg);
		nvgRect(vg, 0, 0, box.size.x, box.size.y);
		nvgFillPaint(vg, paint2);
//		nvgFillColor(vg, nvgRGB(0xff, 0, 0));
		nvgFill(vg);

		nvgGlobalCompositeBlendFuncSeparate(vg, NVG_ONE_MINUS_DST_ALPHA, NVG_DST_ALPHA, NVG_ONE, NVG_ONE);
		nvgBeginPath(vg);
		nvgRect(vg, 0, 0, box.size.x, box.size.y);
		nvgFillPaint(vg, paint1);
//		nvgFillColor(vg, nvgRGB(0, 0xff, 0));
		nvgFill(vg);

		nvgRestore(vg);
		
	}
	void draw(NVGcontext *vg) override {
		DrawImage(vg);
		TransparentWidget::draw(vg);
	}
	~MEBitMap() {
		if (!bitmap1)
			nvgDeleteImage(storedVG, bitmap1);
		if (!bitmap2)
			nvgDeleteImage(storedVG, bitmap2);
	}
};

struct MEWidget : ModuleWidget {

	MEWidget(Module *module) : ModuleWidget(module) {
		box.size = Vec(RACK_GRID_WIDTH * 40, RACK_GRID_HEIGHT * 1);
		FramebufferWidget *fbw = Widget::create<FramebufferWidget>(Vec(0,0));
		fbw->box.size.x = box.size.x;
		fbw->box.size.y = box.size.y;
		addChild(fbw);
		MEBitMap * bmp = Widget::create<MEBitMap>(Vec(0,0));
		bmp->box.size.x = box.size.x;
		bmp->box.size.y = box.size.y;
		bmp->path1 = assetPlugin(plugin, "res/Noise.png");
		bmp->path2 = assetPlugin(plugin, "res/Noise2.png");
		bmp->fontPath = assetPlugin(plugin, "res/Montserrat-Black.ttf");
		fbw->addChild(bmp);
		fbw->dirty = true;
	}
};

Model *modelME = Model::create<Module, MEWidget>("Modular Fungi", "Test", "Test Blanking Place", BLANK_TAG);
