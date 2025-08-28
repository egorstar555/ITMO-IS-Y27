#pragma once

#include <iostream>
#include <cstdint>

namespace ArrayThreeDimensions {
    const int32_t kBitsLo = 16;
    const uint32_t kHiBit = 0x10000;
    class Array3D {
    public:
        Array3D(int x, int y, int z) : _xSize(x), _ySize(y), _zSize(z) {
            int32_t size;
            _size_2_byte = x * y * z;
            size = _size_2_byte + _size_2_byte / kBitsLo + (_size_2_byte % kBitsLo > 0);
            _data = new uint16_t[size]();
        }

        ~Array3D() {
            delete[] _data;
        }

        class Array1D {
        public:
            Array1D(int index, Array3D& parent)
                    : _index(index), _parent(parent) {}

            Array1D& operator[](int z) {
                _index += z;

                return *this;
            }

            Array1D& operator=(uint32_t value) {
                _parent.PutElemInd(_index, value);

                return *this;
            }

            bool operator==(uint32_t value) {
                return _parent.GetElemInd(_index) == value;
            }

            friend std::ostream& operator<<(std::ostream& os, Array1D& arr) {
                os << arr._parent.GetElemInd(arr._index);

                return os;
            }

            friend std::istream& operator>>(std::istream& is, Array1D& arr) {
                uint32_t value;
                is >> value;
                arr._parent.PutElemInd(arr._index, value);

                return is;
            }

        private:
            int _index;
            Array3D& _parent;
        };

        class Array2D {
        public:
            Array2D(int index, Array3D& parent)
                    : _index(index), _parent(parent) {}

            Array1D operator[](int y) {
                return {_index * _parent._ySize * _parent._zSize + y * _parent._zSize, _parent};
            }

        private:
            int _index;
            Array3D& _parent;
        };

        Array2D operator[](int x) {
            return {x, *this};
        }

        static Array3D make_array(int x, int y, int z);

        int SizeX() {
            return _xSize;
        }

        int SizeY() {
            return _ySize;
        }

        int SizeZ() {
            return _zSize;
        }

        friend std::istream& operator>>(std::istream& is, Array3D& arr) {
            for (int i = 0; i < arr._xSize; ++i) {
                for (int j = 0; j < arr._ySize; ++j) {
                    for (int k = 0; k < arr._zSize; ++k) {
                        is >> arr[i][j][k];
                    }
                }
            }

            return is;
        }

        friend std::ostream& operator<<(std::ostream& os, Array3D& arr) {
            for (int i = 0; i < arr._xSize; ++i) {
                for (int j = 0; j < arr._ySize; ++j) {
                    for (int k = 0; k < arr._zSize; ++k) {
                        os << arr[i][j][k] << " ";
                    }
                    os << "\n";
                }
                os << "\n";
            }

            return os;
        }

        int IndexElem(int x, int y, int z);

        void PutElemInd(int index, uint32_t value);

        void PutElem(int x, int y, int z, uint32_t value);

        uint32_t GetElemInd(int index);

        uint32_t GetElem(int x, int y, int z);

        Array3D operator*(uint32_t multiplier);

        Array3D operator+(Array3D& rhs);

        Array3D operator-(Array3D& rhs);

    private:
        int _xSize;
        int _ySize;
        int _zSize;
        int _size_2_byte;
        uint16_t* _data;
    };
}
