#include "BitArray.h"

namespace Msl
{
	BitArray *BitArray::CreateInstance(uint32 total_bits)
	{
		BitArray *bit_array = new BitArray(total_bits);
		return bit_array;
	}

	BitArray *BitArray::CloneFrom(BitArray *const from)
	{
		BitArray *new_bit_array = BitArray::CreateInstance(from->_bit_count);
		uint32 total_units = from->_bit_count / (8 * sizeof(uint8));
		if (from->_bit_count % (8 * sizeof(uint8))) {
			total_units ++;
		}
		memcpy(new_bit_array->_bits, from->_bits, total_units * sizeof(uint8));
		return new_bit_array;
	}

	BitArray *BitArray::CreateFrom(const uint8 *const buffer, uint32 buffer_size)
	{
		if (buffer_size == 0) {
			return 0;
		}
		BitArray *bit_array = new BitArray(buffer, buffer_size);
		return bit_array;
	}

	BitArray::BitArray(uint32 total_bits)
	{
		if (total_bits != 0)
		{
			uint32 total_units = total_bits / (8 * sizeof(uint8));
			if (total_bits % (8 * sizeof(uint8))) {
				total_units ++;
			}
			this->_bits = new uint8[total_units];
			this->_bit_count = total_bits;
			memset(this->_bits, 0, total_units * sizeof(uint8));
		}
		else
		{
			this->_bits = 0;
			this->_bit_count = 0;
		}
	}

	BitArray::BitArray(const uint8 *const buffer, uint32 buffer_size)
	{
		if (buffer_size != 0)
		{
			this->_bits = new uint8[buffer_size];
			this->_bit_count = buffer_size * 8;
			memcpy(this->_bits, buffer, buffer_size);
		}
		else
		{
			this->_bits = 0;
			this->_bit_count = 0;
		}
	}

	BitArray::~BitArray()
	{
		if (this->_bits)
		{
			delete [] this->_bits;
			this->_bits = 0;
		}
	}

	uint32 BitArray::CopyTo(uint8 *const buffer, uint32 buffer_size) const
	{
		uint32 internal_buffer_size = this->GetInternalBufferSize();
		uint32 size_copied = internal_buffer_size;
		if (buffer_size < internal_buffer_size) {
			size_copied = buffer_size;
		}
		memcpy(buffer, this->_bits, size_copied);
		return size_copied;
	}

	void BitArray::CopyFrom(const uint8 *const buffer, uint32 buffer_size)
	{
		if (this->_bits)
		{
			delete [] this->_bits;
			this->_bits = 0;
			this->_bit_count = 0;
		}
		this->_bits = new uint8[buffer_size];
		memcpy(this->_bits, buffer, buffer_size);
		this->_bit_count = buffer_size * 8;
	}

	void BitArray::SetBits(uint32 from, uint32 bit_count, uint32 val)
	{
		if (this->_bits && bit_count > 0 && bit_count <= 32 && from < this->_bit_count)
		{
			// 在目标数组内定位
			if (from + bit_count > this->_bit_count) {
				bit_count = this->_bit_count - from;
			}
			uint32 byte_offset = from / (8 * sizeof(uint8));
			uint32 bit_offset = from % (8 * sizeof(uint8));

			// 计算写入的头部位数（第一个目标字节）、尾部位数（最后一个目标字节）、中间字节数（中间的目标字节）
			uint32 front_bits;
			if (bit_count > 8 - bit_offset) {
				front_bits = 8 - bit_offset;
			}
			else {
				front_bits = bit_count;
			}
			uint32 tail_bits;
			if (bit_count > front_bits) {
				tail_bits = (bit_count - front_bits) % 8;
			}
			else {
				tail_bits = 0;
			}
			uint32 middle_bytes = (bit_count - front_bits) / 8;

			// 写入（优先写入低位）
			switch (middle_bytes)
			{
			case 0:
				{
					uint8 mask = (0xff >> (8 - bit_offset - front_bits));
					uint8 front = (static_cast<uint8>(val << bit_offset) & mask);
					this->_bits[byte_offset] &= ~(mask << bit_offset);
					this->_bits[byte_offset] |= front;
				}
				break;
			case 1:
				{
					uint8 front = static_cast<uint8>(val << bit_offset);
					uint8 mask = ~(0xff << bit_offset);
					this->_bits[byte_offset] &= mask;
					this->_bits[byte_offset] |= front;
					this->_bits[++ byte_offset] = static_cast<uint8>(val >> front_bits);
				}
				break;
			case 2:
				{
					uint8 front = static_cast<uint8>(val << bit_offset);
					uint8 mask = ~(0xff << bit_offset);
					this->_bits[byte_offset] &= mask;
					this->_bits[byte_offset] |= front;
					this->_bits[++ byte_offset] = static_cast<uint8>(val >> front_bits);
					this->_bits[++ byte_offset] = static_cast<uint8>(val >> (8 + front_bits));
				}
				break;
			case 3:
				{
					uint8 front = static_cast<uint8>(val << bit_offset);
					uint8 mask = ~(0xff << bit_offset);
					this->_bits[byte_offset] &= mask;
					this->_bits[byte_offset] |= front;
					this->_bits[++ byte_offset] = static_cast<uint8>(val >> front_bits);
					this->_bits[++ byte_offset] = static_cast<uint8>(val >> (8 + front_bits));
					this->_bits[++ byte_offset] = static_cast<uint8>(val >> (16 + front_bits));
				}
				break;
			}
			if (tail_bits)
			{
				uint8 tail = static_cast<uint8>((val << (32 - bit_count)) >> (32 - tail_bits));
				this->_bits[++ byte_offset] &= (0xff << tail_bits);
				this->_bits[byte_offset] |= tail;
			}
		}
	}

	uint32 BitArray::GetBits(uint32 from, uint32 bit_count) const
	{
		uint32 result = 0;
		if (this->_bits &&
			(bit_count > 0 && bit_count <= 32) &&
			from < this->_bit_count &&
			from + bit_count <= this->_bit_count)
		{
			// 在源数组内定位
			uint32 byte_offset = from / (8 * sizeof(uint8));
			uint32 bit_offset = from % (8 * sizeof(uint8));

			// 计算读取的头部位数（第一个源字节）、尾部位数（最后一个源字节）、中间字节数（中间的源字节）
			uint32 front_bits;
			if (bit_count > 8 - bit_offset) {
				front_bits = 8 - bit_offset;
			}
			else {
				front_bits = bit_count;
			}
			uint32 tail_bits;
			if (bit_count > front_bits) {
				tail_bits = (bit_count - front_bits) % 8;
			}
			else {
				tail_bits = 0;
			}
			uint32 middle_bytes = (bit_count - front_bits) / 8;

			// 读取
			switch (middle_bytes)
			{
			case 0:
				{
					uint8 front = (this->_bits[byte_offset] >> bit_offset);
					result = (front & (0xff >> (8 - front_bits)));
				}
				break;
			case 1:
				{
					result = (this->_bits[byte_offset] >> bit_offset);
					result |= (static_cast<uint32>(this->_bits[++ byte_offset]) << front_bits);
				}
				break;
			case 2:
				{
					result = (this->_bits[byte_offset] >> bit_offset);
					result |= (static_cast<uint32>(this->_bits[++ byte_offset]) << front_bits);
					result |= (static_cast<uint32>(this->_bits[++ byte_offset]) << (front_bits + 8));
				}
				break;
			case 3:
				{
					result = (this->_bits[byte_offset] >> bit_offset);
					result |= (static_cast<uint32>(this->_bits[++ byte_offset]) << front_bits);
					result |= (static_cast<uint32>(this->_bits[++ byte_offset]) << (front_bits + 8));
					result |= (static_cast<uint32>(this->_bits[++ byte_offset]) << (front_bits + 16));
				}
				break;
			}
			if (tail_bits)
			{
				uint8 tail = this->_bits[++ byte_offset];
				tail &= (0xff >> (8 - tail_bits));
				result |= (static_cast<uint32>(tail) << (bit_count - tail_bits));
			}
		}
		return result;
	}

	void BitArray::Destroy()
	{
		delete this;
	}
}