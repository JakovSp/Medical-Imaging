#pragma once
#include <cstdint>
#include <string>
#include <cstring>

namespace vxe::utl{

	template <typename T>
	class Array3D {

	public:
		Array3D(): _points(nullptr), _width(0), _height(0), _depth(0) { }
		Array3D(uint64_t width, uint64_t height, uint64_t depth) :
			_points(nullptr), _width(width), _height(height), _depth(depth) {
			Allocate(_width, _height, _depth);
		}

		void Allocate(uint64_t width, uint64_t height, uint64_t depth) {
			if (_points) { free(_points); }
			_points = nullptr;
			_width = width; _height = height; _depth = depth;
			_points = new T[depth * height * width];
		}

		class Proxy {
		public:
			Proxy(Array3D* enclosing, T* position) : 
				_enclosing{ enclosing },
				_position{ position }
			{};

			T* operator[](int j) { return _position + j * _enclosing->_width; }
			const T* operator[](int j) const { return _position + j * _enclosing->_width; }
			void Copy(T* from){
				std::memcpy(_position, from, _enclosing->_width*_enclosing->_height*sizeof(T));
			}

		private:
			const Array3D* _enclosing;
			T* _position;
		};

		Proxy operator[](int i) { return Proxy{ this, _points + i * _width * _height }; }
		const Proxy operator[](int i) const { return Proxy{ this, _points + i * _width * _height }; }

		//void Load(std::wstring);
		//void Store(std::wstring);

		void Copy(Array3D<T>& Volume) {
			_width = Volume.Width();
			_height = Volume.Height();
			_depth = Volume.Depth();
			Allocate(_width, _height, _depth);
			memcpy(_points, Volume.Points(), _depth*_width*_height*sizeof(T));
		}

		auto Height() const { return _height; }
		auto Width() const { return _width; }
		auto Depth() const { return _depth; }
		auto Points() const { return _points; }

		~Array3D(){}

	private:
		T* _points;
		uint64_t _width;
		uint64_t _height;
		uint64_t _depth;
	};
}
