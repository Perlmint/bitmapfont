#pragma once
#include "node_helper.h"
#include "blob.hpp"
#include <memory>
#include <MagickWand/MagickWand.h>

enum class ImageFormat
{
	PNG,
	JPEG,
	GIF
};
const char *ImageFormatToStr(ImageFormat format);

DECL_CLASS(Canvas)
{
#if NODE
public:
	static void Init(v8::Local<v8::Object> exports);
	static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Persistent<v8::Function> constructor;

	DECL_METHOD(GetBlob);
#endif
public:
	Canvas(uint32_t width, uint32_t height);
	virtual ~Canvas();
	uint32_t width, height;

	std::unique_ptr<Blob> getBlob(ImageFormat format);
	MagickWand * const getWand();
protected:
	MagickWand *wand;
};