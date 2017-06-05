#pragma once
#include "node_helper.h"
#include <MagickWand/MagickWand.h>
#include <memory>
#include <string>
#include <algorithm>

struct Metrics
{
	double charWidth;
	double charHeight;
	double ascender;
	double descender;
	double textWidth;
	double textHeight;
	double maximumHoriontalAdvance;
	double boundingBoxX1;
	double boundingBoxY1;
	double boundingBoxX2;
	double boundingBoxY2;
	double originX;
	double originY;
};

v8::Local<v8::Object> ToCharacterMetrics(v8::Isolate *isolate, const Metrics& metrics);

DECL_CLASS(BitmapFont)
{
#if NODE
public:
	// for node
	static void Init(v8::Local<v8::Object> exports);
	static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Persistent<v8::Function> constructor;

	DECL_PROPERTY(Size);
	DECL_PROPERTY(Family);
	DECL_PROPERTY(Fill);
	DECL_METHOD(Draw);
	DECL_METHOD(Glyph);
#endif
public:
	BitmapFont();

	~BitmapFont();

	void setFamily(const std::string &fontName);

	const std::string &family() const;

	void setSize(double size);

	double size() const;

	void setStrokeThickness(double thickness);
	double strokeThickness() const;

	void setStrokeColor();

	void setColor(const std::string &color);
	std::string getColor() const;

	bool strokeEnabled() const;

	void draw(MagickWand *wand, const char ch[], double x, double y);
	const Metrics *glyphSize(const char chId[]) const;

	bool isDirty() const { return dirty; }

private:
	static MagickWand *_dummyWand;
	static MagickWand *dummyWand() {
		if (_dummyWand == nullptr)
		{
			_dummyWand = NewMagickWand();
		}

		return _dummyWand;
	}

	DrawingWand *wand;
	std::string fontName;
	struct {
		uint8_t thickness;
	} stroke;
	bool dirty = false;
};