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
	DECL_PROPERTY(StrokeColor);
	DECL_PROPERTY(StrokeThickness);
	DECL_PROPERTY(ShadowEnabled);
	DECL_PROPERTY(ShadowColor);
	DECL_PROPERTY(ShadowDistance);
	DECL_PROPERTY(ShadowAngle);
	DECL_PROPERTY(Weight);
	DECL_METHOD(Draw);
	DECL_METHOD(Glyph);
#endif
public:
	BitmapFont();

	~BitmapFont();

	void setFamily(const std::string &fontName);

	std::string family() const;

	void setSize(double size);

	double size() const;

	void setStrokeThickness(double thickness);
	double getStrokeThickness() const;

	void setStrokeColor(const std::string &color);
	std::string getStrokeColor() const;

	void setColor(const std::string &color);
	std::string getColor() const;

	bool strokeEnabled() const;

	void setShadowEnable(bool enable);
	bool getShadowEnabled() const;

	void setShadowColor(const std::string &color);
	std::string getShadowColor() const;

	void setShadowAngle(double angle);
	double getShadowAngle() const;

	void setShadowDistance(double distance);
	double getShadowDistance() const;

	void draw(MagickWand *wand, const char ch[], double x, double y);
	const Metrics *glyphSize(const char chId[]) const;

	bool isDirty() const { return dirty; }

	void setWeight(uint32_t weight);
	uint32_t getWeight() const;

private:
	static MagickWand *_dummyWand;
	static MagickWand *dummyWand() {
		if (_dummyWand == nullptr)
		{
			_dummyWand = NewMagickWand();
			MagickReadImage(_dummyWand, "xc:");
		}

		return _dummyWand;
	}

	void setStroke(const std::string &color, double thickness) const;
	void unsetStroke() const;

	DrawingWand *wand;
	struct {
		double thickness;
		std::string color;
	} stroke;
	struct {
		bool enabled;
		double angle;
		double distance;
		std::string color;
	} shadow;
	bool dirty = false;
};