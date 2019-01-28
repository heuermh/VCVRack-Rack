#include "app/LedDisplay.hpp"
#include "asset.hpp"
#include "window.hpp"
#include "event.hpp"
#include "app.hpp"


namespace rack {


void LedDisplay::draw(const DrawContext &ctx) {
	nvgBeginPath(ctx.vg);
	nvgRoundedRect(ctx.vg, 0, 0, box.size.x, box.size.y, 5.0);
	nvgFillColor(ctx.vg, nvgRGB(0x00, 0x00, 0x00));
	nvgFill(ctx.vg);

	Widget::draw(ctx);
}


LedDisplaySeparator::LedDisplaySeparator() {
	box.size = math::Vec();
}

void LedDisplaySeparator::draw(const DrawContext &ctx) {
	nvgBeginPath(ctx.vg);
	nvgMoveTo(ctx.vg, 0, 0);
	nvgLineTo(ctx.vg, box.size.x, box.size.y);
	nvgStrokeWidth(ctx.vg, 1.0);
	nvgStrokeColor(ctx.vg, nvgRGB(0x33, 0x33, 0x33));
	nvgStroke(ctx.vg);
}


LedDisplayChoice::LedDisplayChoice() {
	box.size = mm2px(math::Vec(0, 28.0 / 3));
	font = Font::load(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
	color = nvgRGB(0xff, 0xd7, 0x14);
	textOffset = math::Vec(10, 18);
}

void LedDisplayChoice::draw(const DrawContext &ctx) {
	nvgScissor(ctx.vg, 0, 0, box.size.x, box.size.y);

	if (font->handle >= 0) {
		nvgFillColor(ctx.vg, color);
		nvgFontFaceId(ctx.vg, font->handle);
		nvgTextLetterSpacing(ctx.vg, 0.0);

		nvgFontSize(ctx.vg, 12);
		nvgText(ctx.vg, textOffset.x, textOffset.y, text.c_str(), NULL);
	}

	nvgResetScissor(ctx.vg);
}

void LedDisplayChoice::onButton(const event::Button &e) {
	if (e.action == GLFW_PRESS && (e.button == GLFW_MOUSE_BUTTON_LEFT || e.button == GLFW_MOUSE_BUTTON_RIGHT)) {
		event::Action eAction;
		onAction(eAction);
		e.consume(this);
	}
}


LedDisplayTextField::LedDisplayTextField() {
	font = Font::load(asset::system("res/fonts/ShareTechMono-Regular.ttf"));
	color = nvgRGB(0xff, 0xd7, 0x14);
	textOffset = math::Vec(5, 5);
}


void LedDisplayTextField::draw(const DrawContext &ctx) {
	nvgScissor(ctx.vg, 0, 0, box.size.x, box.size.y);

	// Background
	nvgBeginPath(ctx.vg);
	nvgRoundedRect(ctx.vg, 0, 0, box.size.x, box.size.y, 5.0);
	nvgFillColor(ctx.vg, nvgRGB(0x00, 0x00, 0x00));
	nvgFill(ctx.vg);

	// Text
	if (font->handle >= 0) {
		bndSetFont(font->handle);

		NVGcolor highlightColor = color;
		highlightColor.a = 0.5;
		int begin = std::min(cursor, selection);
		int end = (this == app()->event->selectedWidget) ? std::max(cursor, selection) : -1;
		bndIconLabelCaret(ctx.vg, textOffset.x, textOffset.y,
			box.size.x - 2*textOffset.x, box.size.y - 2*textOffset.y,
			-1, color, 12, text.c_str(), highlightColor, begin, end);

		bndSetFont(app()->window->uiFont->handle);
	}

	nvgResetScissor(ctx.vg);
}

int LedDisplayTextField::getTextPosition(math::Vec mousePos) {
	bndSetFont(font->handle);
	int textPos = bndIconLabelTextPosition(app()->window->vg, textOffset.x, textOffset.y,
		box.size.x - 2*textOffset.x, box.size.y - 2*textOffset.y,
		-1, 12, text.c_str(), mousePos.x, mousePos.y);
	bndSetFont(app()->window->uiFont->handle);
	return textPos;
}


} // namespace rack
