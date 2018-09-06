#pragma once
#include "Rect.h"

using namespace std;

enum SpriteType
{
	Sprite = 0,
	NineSlice = 1
};

struct SpriteReference
{
public:
	SpriteReference()
	{
	}

	SpriteReference(unsigned long const& id)
	{
		m_id = id;
	}

	~SpriteReference()
	{
	}

	unsigned long getId() const { return m_id; }
	SpriteType getType() const { return m_type; }
	float getSliceTop() const { return m_sliceTop; }
	float getSliceRight() const { return m_sliceRight; }
	float getSliceBottom() const { return m_sliceBottom; }
	float getSliceLeft() const { return m_sliceLeft; }
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	Rect getTexPosition() const { return m_texPosition; }
	string getTextureId() const { return m_textureId; }

	void setType(SpriteType const& type) { m_type = type; }
	void setSliceTop(float const& sliceTop) { m_sliceTop = sliceTop; }
	void setSliceRight(float const& sliceRight) { m_sliceRight = sliceRight; }
	void setSliceBottom(float const& sliceBottom) { m_sliceBottom = sliceBottom; }
	void setSliceLeft(float const& sliceLeft) { m_sliceLeft = sliceLeft; }
	void setWidth(int const& width) { m_width = width; }
	void setHeight(int const& height) { m_height = height; }
	void setTexPosition(float const& x, float const& y, float const& dx, float const& dy) { m_texPosition.set(x, y, dx, dy); }
	void setTextureId(string const& textureId) { m_textureId = textureId; }
private:
	unsigned long m_id;
	SpriteType m_type = SpriteType::Sprite;
	float m_sliceTop = 0.0f;
	float m_sliceRight = 1.0f;
	float m_sliceBottom = 1.0f;
	float m_sliceLeft = 0.0f;
	int m_width;
	int m_height;
	Rect m_texPosition;
	string m_textureId;
};
