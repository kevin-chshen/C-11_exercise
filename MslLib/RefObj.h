#ifndef _MslLib_RefObj_H
#define _MslLib_RefObj_H

#include "BasicType.h"

namespace Msl
{
	// 引用计数对象的接口，通常任何接口都应该从此接口虚继承
	struct IRefObj
	{
		virtual void AddRef() = 0;
		virtual void Release() = 0;
	};

	// 引用计数对象智能指针
	template <typename T>
	class RefObjPtr
	{
	private:
		typedef RefObjPtr<T> ThisType;
		typedef T *ThisType::* UnspecifiedBoolType;

	private:
		mutable T *_p;

	public:
		RefObjPtr() {
			this->_p = 0;
		}

		RefObjPtr(T *const p)	// 为了能支持 0 的赋值，取消 explicit。
		{
			if (p) {
				p->AddRef();
			}
			this->_p = p;
		}

		RefObjPtr(T *const p, bool attach)
		{
			if (p && !attach) {
				p->AddRef();
			}
			this->_p = p;
		}

		RefObjPtr(const RefObjPtr<T> &right)
		{
			if (right._p) {
				right._p->AddRef();
			}
			this->_p = right._p;
		}

		~RefObjPtr()
		{
			if (this->_p)
			{
				this->_p->Release();
				this->_p = 0;
			}
		}

		inline operator T * () {
			return this->_p;
		}

		inline operator T * () const {
			return this->_p;
		}

		void operator = (T *const right)
		{
			if (this->_p) {
				this->_p->Release();
			}
			if (right) {
				right->AddRef();
			}
			this->_p = right;
		}

		void operator = (const RefObjPtr<T> &right)
		{
			if (this->_p) {
				this->_p->Release();
			}
			if (right._p) {
				right._p->AddRef();
			}
			this->_p = right._p;
		}

		inline bool operator == (const RefObjPtr<T> &right) const {
			return this->_p == right._p ? true : false;
		}

		inline bool operator == (T *const right) const {
			return this->_p == right ? true : false;
		}

		inline bool operator != (const RefObjPtr<T> &right) const {
			return this->_p != right._p ? true : false;
		}

		inline bool operator != (T *const right) const {
			return this->_p != right ? true : false;
		}

		inline bool operator < (const RefObjPtr<T> &right) const {
			return this->_p < right._p ? true : false;
		}

		inline bool operator < (T *const right) const {
			return this->_p < right ? true : false;
		}

		inline T *operator -> () const {
			return this->_p;
		}

		//inline operator bool () const {
		//	return !this->NullPointer();
		//}

		inline operator UnspecifiedBoolType () const {
			return this->_p == 0 ? 0 : &ThisType::_p;
		}

		bool operator! () const {
			return this->_p == 0;
		}

	//// 禁止以下的类型转换。
	//private:
	//	operator char () const;
	//	operator unsigned char () const;
	//	operator short () const;
	//	operator unsigned short () const;
	//	operator int () const;
	//	operator unsigned int () const;
	//	operator long () const;
	//	operator unsigned long () const;
	//	operator long long () const;
	//	operator unsigned long long ();
	//	operator float () const;
	//	operator double () const;

	//public:
		inline bool NullPointer() const {
			return this->_p ? false : true;
		}

		//inline void SetNullPointer() {
		//	this->_p = 0;
		//}

		void Attach(T *const p)
		{
			if (this->_p) {
				this->_p->Release();
			}
			this->_p = p;
		}

		template <typename TOther>
		RefObjPtr<TOther> CastTo() const
		{
			return static_cast<TOther *>(this->_p);
		}

		template <typename TOther>
		RefObjPtr<TOther> DynamicCastTo() const
		{
			return dynamic_cast<TOther *>(this->_p);
		}
	};
}

#endif
