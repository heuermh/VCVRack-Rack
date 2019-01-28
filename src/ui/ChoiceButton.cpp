#include "ui/ChoiceButton.hpp"


namespace rack {


void ChoiceButton::draw(const DrawContext &ctx) {
	bndChoiceButton(ctx.vg, 0.0, 0.0, box.size.x, box.size.y, BND_CORNER_NONE, state, -1, text.c_str());
}


} // namespace rack
