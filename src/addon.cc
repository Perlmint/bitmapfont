#include <node.h>
#include <MagickWand/MagickWand.h>
#include "bitmapfont.hpp"
#include "canvas.hpp"

namespace bf {
	using v8::Local;
	using v8::Object;

	void InitAll(Local<Object> exports) {
		BitmapFont::Init(exports);
		Canvas::Init(exports);
	}
}

NODE_MODULE(libbf, bf::InitAll)