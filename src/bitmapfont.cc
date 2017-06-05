#include "bitmapfont.hpp"
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

MagickWand *BitmapFont::_dummyWand = nullptr;
Persistent<Function> BitmapFont::constructor;

void BitmapFont::Init(Local<Object> exports) {
	Isolate* isolate = exports->GetIsolate();

	// Prepare constructor template
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(String::NewFromUtf8(isolate, "BitmapFont"));
	auto ipl = tpl->InstanceTemplate();
	ipl->SetInternalFieldCount(1);

	Nan::SetAccessor(ipl, String::NewFromUtf8(isolate, "size"), &BitmapFont::GetSize, &BitmapFont::SetSize);
	Nan::SetAccessor(ipl, String::NewFromUtf8(isolate, "family"), &BitmapFont::GetFamily, &BitmapFont::SetFamily);
	Nan::SetAccessor(ipl, String::NewFromUtf8(isolate, "fill"), &BitmapFont::GetFill, &BitmapFont::SetFill);
	Nan::SetPrototypeMethod(tpl, "draw", &BitmapFont::Draw);
	Nan::SetPrototypeMethod(tpl, "glyph", &BitmapFont::Glyph);
		
constructor.Reset(isolate, tpl->GetFunction());
exports->Set(String::NewFromUtf8(isolate, "BitmapFont"),
	tpl->GetFunction());
}

void BitmapFont::New(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall()) {
		if (args.Length() != 0) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "BitmapFont constructor allow only 0 arguments")
			));
		}
		BitmapFont* obj = new BitmapFont();
		obj->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	}
	else {
		Local<Context> context = isolate->GetCurrentContext();
		Local<Function> cons = Local<Function>::New(isolate, constructor);
		Local<Object> result =
			cons->NewInstance(context).ToLocalChecked();
		args.GetReturnValue().Set(result);
	}
}

void BitmapFont::GetSize(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info)
{
	info.GetReturnValue().Set(Number::New(info.GetIsolate(), Unwrap<BitmapFont>(info.Holder())->size()));
}

void BitmapFont::SetSize(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info)
{
	Unwrap<BitmapFont>(info.Holder())->setSize(value.As<v8::Number>()->Int32Value());
}

void BitmapFont::GetFamily(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info)
{
	info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), Unwrap<BitmapFont>(info.Holder())->family().c_str()));
}

void BitmapFont::SetFamily(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info)
{
	Nan::Utf8String str{ value->ToString() };
	Unwrap<BitmapFont>(info.Holder())->setFamily(*str);
}

void BitmapFont::GetFill(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info)
{
	info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), Unwrap<BitmapFont>(info.Holder())->getColor().c_str()));
}

void BitmapFont::SetFill(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info)
{
	Nan::Utf8String str{ value->ToString() };
	Unwrap<BitmapFont>(info.Holder())->setColor(*str);
}

void BitmapFont::Draw(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
	if (info.Length() != 4)
	{
		isolate->ThrowException(Exception::TypeError(
			String::NewFromUtf8(isolate, "draw need 3 arguments. canvas, character(string), x(number), y(number)")
		));
	}

	if (!info[0]->IsObject())
	{
		isolate->ThrowException(Exception::TypeError(
			String::NewFromUtf8(isolate, "argument canvas must be object")
		));
	}
	auto wand = Unwrap<Canvas>(info[0]->ToObject())->getWand();

	if (!info[1]->IsString())
	{
		isolate->ThrowException(Exception::TypeError(
			String::NewFromUtf8(isolate, "argument character(string) must be string")
		));
	}

	char buffer[8] = { 0 };
	info[1].As<String>()->WriteUtf8(buffer, 1);

	if (!info[2]->IsNumber() || !info[3]->IsNumber())
	{
		isolate->ThrowException(Exception::TypeError(
			String::NewFromUtf8(isolate, "argument x and y must be number")
		));
	}

	auto x = info[2]->NumberValue(), y = info[3]->NumberValue();
	auto unwrapped = Unwrap<BitmapFont>(info.Holder());
	unwrapped->draw(wand, buffer, x, y);
}

void BitmapFont::Glyph(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
	if (info.Length() != 1)
	{
		isolate->ThrowException(Exception::TypeError(
			String::NewFromUtf8(isolate, "glyph need 2 argument. character(string)")
		));
	}

	if (!info[0]->IsString())
	{
		isolate->ThrowException(Exception::TypeError(
			String::NewFromUtf8(isolate, "argument character(string) must be string")
		));
	}

	char buffer[8] = { 0 };
	info[0].As<String>()->WriteUtf8(buffer, 1);
	auto unwrapped = Unwrap<BitmapFont>(info.Holder());
	auto metrics = unwrapped->glyphSize(buffer);
	info.GetReturnValue().Set(ToCharacterMetrics(isolate, *metrics));
}
#endif

BitmapFont::BitmapFont()
	: wand(NewDrawingWand())
	, stroke{ 0 }
{
	DrawSetFillOpacity(wand, 1.0);
}

BitmapFont::~BitmapFont()
{
	DestroyDrawingWand(wand);
}

void BitmapFont::setFamily(const std::string &fontName)
{
	if (fontName.compare(this->fontName) == 0)
	{
		return;
	}

	this->fontName = fontName;
	dirty = true;
	DrawSetFontFamily(wand, fontName.c_str());
}

const std::string &BitmapFont::family() const
{
	return fontName;
}

void BitmapFont::setSize(double size)
{
	DrawSetFontSize(wand, size);
}

double BitmapFont::size() const
{
	return DrawGetFontSize(wand);
}

void BitmapFont::setStrokeThickness(double thickness)
{
	DrawSetStrokeWidth(wand, thickness);
}

void BitmapFont::setStrokeColor()
{

}

double BitmapFont::strokeThickness() const
{
	return DrawGetStrokeWidth(wand);
}

bool BitmapFont::strokeEnabled() const
{
	return stroke.thickness != 0;
}

void BitmapFont::setColor(const std::string &color)
{
	auto pw = NewPixelWand();
	PixelSetColor(pw, color.c_str());
	DrawSetFillColor(wand, pw);
	DestroyPixelWand(pw);
}

std::string BitmapFont::getColor() const
{
	auto pw = NewPixelWand();
	DrawGetFillColor(wand, pw);
	auto ret = std::string(PixelGetColorAsString(pw));
	DestroyPixelWand(pw);
	return ret;
}

void BitmapFont::draw(MagickWand *wand, const char ch[], double x, double y)
{
	MagickAnnotateImage(wand, this->wand, x, y, 0, ch);

	if (strokeEnabled())
	{

	}
}

const Metrics * BitmapFont::glyphSize(const char ch[]) const
{
	return reinterpret_cast<Metrics*>(MagickQueryFontMetrics(dummyWand(), this->wand, ch));
}

v8::Local<v8::Object> ToCharacterMetrics(v8::Isolate *isolate, const Metrics& metrics)
{
	auto ret = v8::Object::New(isolate);

	ret->Set(String::NewFromUtf8(isolate, "width"), v8::Number::New(isolate, metrics.charWidth));
	ret->Set(String::NewFromUtf8(isolate, "height"), v8::Number::New(isolate, metrics.charHeight));
	ret->Set(String::NewFromUtf8(isolate, "ascender"), v8::Number::New(isolate, metrics.ascender));
	ret->Set(String::NewFromUtf8(isolate, "descender"), v8::Number::New(isolate, metrics.descender));

	return ret;
}