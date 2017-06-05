#pragma once
#include <memory>
#include <MagickWand/MagickWand.h>

class Blob
{
public:
	Blob(uint8_t *data, size_t size)
		: _data(data), _size(size) {}
	~Blob()
	{
		MagickRelinquishMemory(_data);
	}

	const uint8_t* data() const
	{
		return _data;
	}

	const size_t size() const
	{
		return _size;
	}

private:
	uint8_t *_data;
	size_t _size;
};