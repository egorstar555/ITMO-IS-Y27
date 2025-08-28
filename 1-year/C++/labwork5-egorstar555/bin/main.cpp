#include <lib/Array3D.h>

#include <iostream>

int main() {
    int maxX = 5;
    int maxY = 2;
    int maxZ = 3;
    ArrayThreeDimensions::Array3D arr = ArrayThreeDimensions::Array3D::make_array(maxX, maxY, maxZ);

    int n = 0;
    for (int i = 0; i < maxX; ++i) {
        for (int j = 0; j < maxY; ++j) {
            for (int k = 0; k < maxZ; ++k) {
                arr[i][j][k] = n;
                ++n;
            }
        }
    }
    ArrayThreeDimensions::Array3D product = ArrayThreeDimensions::Array3D::make_array(maxX, maxY, maxZ);
    product = arr * 2;
    std::cout << arr;
    std::cout << product;

    ArrayThreeDimensions::Array3D difference = ArrayThreeDimensions::Array3D::make_array(maxX, maxY, maxZ);
    difference = product - arr;
    std::cout << difference;

    ArrayThreeDimensions::Array3D summa = ArrayThreeDimensions::Array3D::make_array(maxX, maxY, maxZ);
    summa = product + arr;
    std::cout << summa;

    return 0;
}
