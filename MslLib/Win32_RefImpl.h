#ifndef _MslLib_Win32_RefImpl_H
#define _MslLib_Win32_RefImpl_H

#include "Win32.h"

namespace Msl
{
	namespace Win32
	{
		// 引用计数实现
		class RefImpl
		{
		protected:
			volatile LONG _val;

		public:
			RefImpl() : _val(0) {}
			RefImpl(int init_val) : _val(init_val) {}

		public:
			int Increment() {
				return InterlockedIncrement(&this->_val);
			}

			int Decrement() {
				return InterlockedDecrement(&this->_val);
			}
		};
	}
}

#endif