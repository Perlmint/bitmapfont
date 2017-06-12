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
using v8::Boolean;
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
	Nan::SetAccessor(ipl, String::NewFromUtf8(isolate, "weight"), &BitmapFont::GetWeight, &BitmapFont::SetWeight);
	Nan::SetAccessor(ipl, String::NewFromUtf8(isolate, "strokeThickness"), &BitmapFont::GetStrokeThickness, &BitmapFont::SetStrokeThickness);
	Nan::SetAccessor(ipl, String::NewFromUtf8(isolate, "strokeColor"), &BitmapFont::GetStrokeColor, &BitmapFont::SetStrokeColor);
	Nan::SetAccessor(ipl, String::NewFromUtf8(isolate, "shadowEnabled"), &BitmapFont::GetShadowEnabled, &BitmapFont::SetShadowEnabled);
	Nan::SetAccessor(ipl, String::NewFromUtf8(isolate, "shadowAngle"), &BitmapFont::GetShadowAngle, &BitmapFont::SetShadowAngle);
	Nan::SetAccessor(ipl, String::NewFromUtf8(isolate, "shadowDistance"), &BitmapFont::GetShadowDistance, &BitmapFont::SetShadowDistance);
	Nan::SetAccessor(ipl, String::NewFromUtf8(isolate, "shadowColor"), &BitmapFont::GetShadowColor, &BitmapFont::SetShadowColor);
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
	Unwrap<BitmapFont>(info.Holder())->setSize(value.As<v8::Number>()->NumberValue());
}

void BitmapFont::GetWeight(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info)
{
	info.GetReturnValue().Set(Number::New(info.GetIsolate(), Unwrap<BitmapFont>(info.Holder())->getWeight()));
}

void BitmapFont::SetWeight(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info)
{
	Unwrap<BitmapFont>(info.Holder())->setWeight(value.As<v8::Number>()->Int32Value());
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

void BitmapFont::GetStrokeThickness(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info)
{
	info.GetReturnValue().Set(Number::New(info.GetIsolate(), Unwrap<BitmapFont>(info.Holder())->getStrokeThickness()));
}

void BitmapFont::SetStrokeThickness(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info)
{
	Unwrap<BitmapFont>(info.Holder())->setStrokeThickness(value.As<v8::Number>()->Int32Value());
}

void BitmapFont::GetStrokeColor(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info)
{
	info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), Unwrap<BitmapFont>(info.Holder())->getStrokeColor().c_str()));
}

void BitmapFont::SetStrokeColor(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info)
{
	Nan::Utf8String str{ value->ToString() };
	Unwrap<BitmapFont>(info.Holder())->setStrokeColor(*str);
}

void BitmapFont::GetShadowColor(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info)
{
	info.GetReturnValue().Set(String::NewFromUtf8(info.GetIsolate(), Unwrap<BitmapFont>(info.Holder())->getShadowColor().c_str()));
}

void BitmapFont::SetShadowColor(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info)
{
	Nan::Utf8String str{ value->ToString() };
	Unwrap<BitmapFont>(info.Holder())->setShadowColor(*str);
}

void BitmapFont::SetShadowAngle(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info)
{
	Unwrap<BitmapFont>(info.Holder())->setShadowAngle(value.As<v8::Number>()->NumberValue());
}

void BitmapFont::GetShadowAngle(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info)
{
	info.GetReturnValue().Set(Number::New(info.GetIsolate(), Unwrap<BitmapFont>(info.Holder())->getShadowAngle()));
}

void BitmapFont::GetShadowDistance(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info)
{
	info.GetReturnValue().Set(Number::New(info.GetIsolate(), Unwrap<BitmapFont>(info.Holder())->getShadowDistance()));
}

void BitmapFont::SetShadowDistance(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info)
{
	Unwrap<BitmapFont>(info.Holder())->setShadowDistance(value.As<v8::Number>()->NumberValue());
}

void BitmapFont::GetShadowEnabled(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info)
{
	info.GetReturnValue().Set(Boolean::New(info.GetIsolate(), Unwrap<BitmapFont>(info.Holder())->getShadowEnabled()));
}

void BitmapFont::SetShadowEnabled(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info)
{
	Unwrap<BitmapFont>(info.Holder())->setShadowEnable(value.As<v8::Boolean>()->BooleanValue());
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
	, shadow{ false, 0, 0 }
{
	DrawSetFillOpacity(wand, 1.0);
	DrawSetStrokeAntialias(wand, MagickTrue);
}

BitmapFont::~BitmapFont()
{
	DestroyDrawingWand(wand);
}

void BitmapFont::setFamily(const std::string &fontName)
{
	DrawSetFontFamily(wand, fontName.c_str());
}

std::string BitmapFont::family() const
{
	return DrawGetFontFamily(wand);
}

void BitmapFont::setWeight(uint32_t weight)
{
	DrawSetFontWeight(wand, weight);
}

uint32_t BitmapFont::getWeight() const
{
	return DrawGetFontWeight(wand);
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
	stroke.thickness = thickness;
}

void BitmapFont::setStrokeColor(const std::string &color)
{
	stroke.color = color;
}

std::string BitmapFont::getStrokeColor() const
{
	return stroke.color;
}

double BitmapFont::getStrokeThickness() const
{
	return stroke.thickness;
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

void BitmapFont::setStroke(const std::string &color, double thickness) const
{
	auto pw = NewPixelWand();
	PixelSetColor(pw, color.c_str());
	DrawSetStrokeColor(wand, pw);
	DestroyPixelWand(pw);
	DrawSetStrokeWidth(wand, thickness);
}

void BitmapFont::unsetStroke() const
{
	DrawSetStrokeWidth(wand, 0);
	auto pw = NewPixelWand();
	PixelSetAlpha(pw, 0);
	DrawSetStrokeColor(wand, pw);
}

void BitmapFont::setShadowEnable(bool enable)
{
	shadow.enabled = enable;
}

bool BitmapFont::getShadowEnabled() const
{
	return shadow.enabled;
}

void BitmapFont::setShadowColor(const std::string &color)
{
	shadow.color = color;
}
std::string BitmapFont::getShadowColor() const
{
	return shadow.color;
}

static const double PI = 3.1415926537;

void BitmapFont::setShadowAngle(double angle)
{
	shadow.angle = angle / 180 * PI;
}
double BitmapFont::getShadowAngle() const
{
	return shadow.angle * 180 / PI;
}

void BitmapFont::setShadowDistance(double distance)
{
	shadow.distance = distance;
}
double BitmapFont::getShadowDistance() const
{
	return shadow.distance;
}

void BitmapFont::draw(MagickWand *wand, const char ch[], double x, double y)
{
	if (shadow.enabled)
	{
		if (strokeEnabled())
		{
			setStroke(shadow.color, stroke.thickness);
		}

		MagickAnnotateImage(wand, this->wand, x + shadow.distance * cos(shadow.angle), y + shadow.distance * sin(shadow.angle), 0, ch);

		unsetStroke();
	}
	if (strokeEnabled())
	{
		setStroke(stroke.color, stroke.thickness);
		MagickAnnotateImage(wand, this->wand, x, y, 0, ch);
		unsetStroke();
	}

	MagickAnnotateImage(wand, this->wand, x, y, 0, ch);
}

const Metrics * BitmapFont::glyphSize(const char ch[]) const
{
	auto metrics = reinterpret_cast<Metrics*>(MagickQueryFontMetrics(dummyWand(), wand, ch));

	if (strokeEnabled())
	{
		metrics->boundingBoxX1 -= stroke.thickness / 2;
		metrics->boundingBoxY1 -= stroke.thickness / 2;
		metrics->boundingBoxX2 += stroke.thickness / 2;
		metrics->boundingBoxY2 += stroke.thickness / 2;
		metrics->textWidth += stroke.thickness;
		metrics->textHeight += stroke.thickness;
	}
	if (shadow.enabled)
	{
		auto x = shadow.distance * cos(shadow.angle);
		auto y = shadow.distance * sin(shadow.angle);
		metrics->boundingBoxX2 += x;
		metrics->boundingBoxY1 -= y;
		metrics->textWidth += abs(x);
		metrics->textHeight += abs(y);
	}

	return metrics;
}

v8::Local<v8::Object> ToCharacterMetrics(v8::Isolate *isolate, const Metrics& metrics)
{
	auto ret = v8::Object::New(isolate);

	ret->Set(String::NewFromUtf8(isolate, "x1"), v8::Number::New(isolate, metrics.boundingBoxX1));
	ret->Set(String::NewFromUtf8(isolate, "y1"), v8::Number::New(isolate, metrics.boundingBoxY1));
	ret->Set(String::NewFromUtf8(isolate, "x2"), v8::Number::New(isolate, metrics.boundingBoxX2));
	ret->Set(String::NewFromUtf8(isolate, "y2"), v8::Number::New(isolate, metrics.boundingBoxY2));
	ret->Set(String::NewFromUtf8(isolate, "originX"), v8::Number::New(isolate, metrics.originX));
	ret->Set(String::NewFromUtf8(isolate, "originY"), v8::Number::New(isolate, metrics.originY));
	ret->Set(String::NewFromUtf8(isolate, "width"), v8::Number::New(isolate, metrics.textWidth));
	ret->Set(String::NewFromUtf8(isolate, "height"), v8::Number::New(isolate, metrics.textHeight));
	ret->Set(String::NewFromUtf8(isolate, "ascender"), v8::Number::New(isolate, metrics.ascender));
	ret->Set(String::NewFromUtf8(isolate, "descender"), v8::Number::New(isolate, metrics.descender));

	return ret;
}