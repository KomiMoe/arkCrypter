#pragma once

/*____________________________________________________________________________________________________________

Original Author: skadro
Github: https://github.com/skadro-official
License: See end of file

skCrypter
		Compile-time, Usermode + Kernelmode, safe and lightweight string crypter library for C++11+

							*Not removing this part is appreciated*
____________________________________________________________________________________________________________*/

#if defined(_MSC_VER)
#define ALWAYS_INLINE __forceinline
#else
#define ALWAYS_INLINE __attribute__((always_inline))
#endif

#ifdef _KERNEL_MODE
	namespace std
	{
		// STRUCT TEMPLATE remove_reference
		template <class _Ty>
		struct remove_reference {
			using type = _Ty;
		};

		template <class _Ty>
		struct remove_reference<_Ty&> {
			using type = _Ty;
		};

		template <class _Ty>
		struct remove_reference<_Ty&&> {
			using type = _Ty;
		};

		template <class _Ty>
		using remove_reference_t = typename remove_reference<_Ty>::type;

		// STRUCT TEMPLATE remove_const
		template <class _Ty>
		struct remove_const { // remove top-level const qualifier
			using type = _Ty;
		};

		template <class _Ty>
		struct remove_const<const _Ty> {
			using type = _Ty;
		};

		template <class _Ty>
		using remove_const_t = typename remove_const<_Ty>::type;
	}
#else
	#include <type_traits>
#endif

namespace skc
{
	template<class _Ty>
	using clean_type = typename std::remove_const_t<std::remove_reference_t<_Ty>>;

	template <int _size, char _key1, char _key2, typename T>
	class skCrypter
	{
	public:
		ALWAYS_INLINE constexpr skCrypter(T* data)
		{		
			crypt(data);
		}

		ALWAYS_INLINE T* get()
		{
			return _storage;
		}

		ALWAYS_INLINE int size() // (w)char count
		{
			return _size;
		}

		ALWAYS_INLINE char key()
		{
			return _key1;
		}

		ALWAYS_INLINE T* encrypt()
		{
			if (!isEncrypted())
				crypt(_storage);

			return _storage;
		}

		ALWAYS_INLINE T* decrypt()
		{
		        _pStorage = reinterpret_cast<void *>(reinterpret_cast<size_t>(&_storage[_size]) - _size);
			if (isEncrypted())
				crypt(static_cast<T *>(_pStorage));

			return static_cast<T *>(_pStorage);
		}

		ALWAYS_INLINE bool isEncrypted()
		{
			return _storage[_size - 1] != 0;
		}

		ALWAYS_INLINE void clear() // set full storage to 0
		{
			_pStorage = reinterpret_cast<void *>(reinterpret_cast<size_t>(&_storage[_size]) - _size);

			for (int i = 0; i < _size; i++)
			{
				static_cast<T *>(_pStorage)[i] = 0;
			}
		}

		ALWAYS_INLINE operator T* ()
		{
		         return decrypt();
		}
		
	private:
		ALWAYS_INLINE constexpr void crypt(T* data)
		{
			for (int i = 0; i < _size; i++)
			{
				_storage[i] = data[i] ^ ((_key1 + i % (1 + _key2)) + 1);
			}
		}	

		void* _pStorage = nullptr;
		T _storage[_size]{};
	};
}

#define skCrypt(str) skCrypt_key(str, __TIME__[4], __TIME__[7])
#define skCrypt_key(str, key1, key2) []() { \
			constexpr static auto crypted = skc::skCrypter \
				<sizeof(str) / sizeof(str[0]), key1, key2, skc::clean_type<decltype(str[0])>>((skc::clean_type<decltype(str[0])>*)str); \
					return crypted; }()

/*________________________________________________________________________________

MIT License

Copyright (c) 2020 skadro

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

________________________________________________________________________________*/
