#include "canvas.hpp"

#if NODE
using v8::Exception;
using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;
using v8::ArrayBuffer;

Persistent<Function> Canvas::constructor;

void Canvas::Init(Local<Object> exports) {
	Isolate* isolate = exports->GetIsolate();

	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(String::NewFromUtf8(isolate, "Canvas"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	Nan::SetPrototypeMethod(tpl, "blob", &Canvas::GetBlob);

	constructor.Reset(isolate, tpl->GetFunction());
	exports->Set(String::NewFromUtf8(isolate, "Canvas"),
		tpl->GetFunction());
}

void Canvas::New(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall()) {
		if (args.Length() != 2) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Canvas constructor allow only 2 arguments - width, height")
			));
		}
		const auto &width = args[0].As<Number>();
		const auto &height = args[1].As<Number>();
		Canvas* obj = new Canvas(width->Int32Value(), height->Int32Value());
		obj->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	}
	else {
		const int argc = 2;
		Local<Value> argv[argc] = { args[0], args[1] };
		Local<Context> context = isolate->GetCurrentContext();
		Local<Function> cons = Local<Function>::New(isolate, constructor);
		Local<Object> result =
			cons->NewInstance(context, argc, argv).ToLocalChecked();
		args.GetReturnValue().Set(result);
	}
}

void Canvas::GetBlob(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
	if (info.Length() != 1 || !info[0]->IsNumber())
	{
		isolate->ThrowException(
			Exception::TypeError(String::NewFromUtf8(isolate, "GetBlob need one parameter - image format(number)"))
		);
		return;
	}
	auto format = info[0]->Int32Value();
	auto obj = Unwrap<Canvas>(info.Holder());
	auto blob = obj->getBlob(static_cast<ImageFormat>(format));
	auto buffer = node::Buffer::Copy(isolate, reinterpret_cast<const char *>(blob->data()), blob->size());
	info.GetReturnValue().Set(buffer.ToLocalChecked());
}
#endif

Canvas::Canvas(uint32_t width, uint32_t height)
	: width(width)
	, height(height)
	, wand(NewMagickWand())
{
	auto pixel = NewPixelWand();
	PixelSetColor(pixel, "none");
	MagickNewImage(wand, width, height, pixel);
	DestroyPixelWand(pixel);
}

Canvas::~Canvas()
{
	DestroyMagickWand(wand);
}

std::unique_ptr<Blob> Canvas::getBlob(ImageFormat format)
{
	const char *formatStr = ImageFormatToStr(format);
	auto ret = std::unique_ptr<Blob>();
	
	if (formatStr != nullptr)
	{
		MagickSetImageFormat(wand, formatStr);
		size_t size;
		auto data = MagickGetImageBlob(wand, &size);
		ret.reset(new Blob(data, size));
	}

	return std::move(ret);
}

MagickWand * const Canvas::getWand()
{
	return this->wand;
}

const char *ImageFormatToStr(ImageFormat format)
{
	switch (format)
	{
	case ImageFormat::GIF:
		return "GIF";
	case ImageFormat::JPEG:
		return "JPEG";
	case ImageFormat::PNG:
		return "PNG";
	}

	return nullptr;
}