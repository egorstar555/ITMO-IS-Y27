#include "Array3D.h"

using namespace ArrayThreeDimensions;

Array3D Array3D::make_array(int x, int y, int z) {
    return {x, y, z};
}

int Array3D::IndexElem(int x, int y, int z) {
    return x * _ySize * _zSize + y * _zSize + z;
}

void Array3D::PutElemInd(int index, uint32_t value) {
    _data[index] = value;
    _data[_size_2_byte + (index / kBitsLo)] |= ((value >> kBitsLo) & 1) << (index % kBitsLo);
}

void Array3D::PutElem(int x, int y, int z, uint32_t value) {
    int ind = IndexElem(x, y, z);
    PutElemInd(ind, value);
}

uint32_t Array3D::GetElemInd(int index) {
    uint32_t value = _data[index];
    int i = _size_2_byte + (index / kBitsLo);
    uint32_t mask = 1 << (index % kBitsLo);
    if (_data[i] & (mask)) {
        value |= kHiBit;
    }

    return value;
}

uint32_t Array3D::GetElem(int x, int y, int z) {
    int ind = IndexElem(x, y, z);

    return GetElemInd(ind);
}

Array3D Array3D::operator*(uint32_t multiplier) {
    Array3D res = make_array(_xSize, _ySize, _zSize);
    uint32_t prod;
    for (int i = 0; i < _xSize; ++i) {
        for (int j = 0; j < _ySize; ++j) {
            for (int k = 0; k < _zSize; ++k) {
                prod = GetElem(i, j, k);
                prod *= multiplier;
                res[i][j][k] = prod;
            }
        }
    }

    return res;
}

Array3D Array3D::operator+(Array3D& rhs) {
    Array3D res = make_array(_xSize, _ySize, _zSize);
    uint32_t summa;
    for (int i = 0; i < _xSize; ++i) {
        for (int j = 0; j < _ySize; ++j) {
            for (int k = 0; k < _zSize; ++k) {
                summa = GetElem(i, j, k);
                summa += rhs.GetElem(i, j, k);
                res[i][j][k] = summa;
            }
        }
    }

    return res;
}

Array3D Array3D::operator-(Array3D& rhs) {
    Array3D res = make_array(_xSize, _ySize, _zSize);
    uint32_t diff;
    for (int i = 0; i < _xSize; ++i) {
        for (int j = 0; j < _ySize; ++j) {
            for (int k = 0; k < _zSize; ++k) {
                diff = GetElem(i, j, k);
                diff -= rhs.GetElem(i, j, k);
                res[i][j][k] = diff;
            }
        }
    }

    return res;
}
