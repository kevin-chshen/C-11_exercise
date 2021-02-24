#ifndef _MslLib_BitArray_H
#define _MslLib_BitArray_H

#include "BasicType.h"
#include <memory.h>

namespace Msl
{
	class BitArray
	{
	private:
		uint8 *_bits;		// 数据
		uint32 _bit_count;	// 有效位数

	public:
		static BitArray *CreateInstance(uint32 total_bits);
		static BitArray *CloneFrom(BitArray *const from);
		static BitArray *CreateFrom(const uint8 *const buffer, uint32 buffer_size);

	private:
		BitArray(uint32 total_bits);
		BitArray(const uint8 *const buffer, uint32 buffer_size);
		~BitArray();

	public:
		inline uint32 GetInternalBufferSize() const {
			return this->_bit_count / 8 + (this->_bit_count % 8 ? 1 : 0);
		}
		inline const uint8 *GetInternalBuffer() const {
			return this->_bits;
		}
		uint32 CopyTo(uint8 *const buffer, uint32 buffer_size) const;
		void CopyFrom(const uint8 *const buffer, uint32 buffer_size);
		void SetBits(uint32 from, uint32 bit_count, uint32 val);
		uint32 GetBits(uint32 from, uint32 bit_count) const;
		void Destroy();
	};
}

#endif