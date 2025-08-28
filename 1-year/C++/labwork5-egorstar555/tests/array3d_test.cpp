#include <lib/Array3D.h>

#include <gtest/gtest.h>
#include <sstream>

using namespace ArrayThreeDimensions;

TEST(Array3DTestSuite, AssignmentTest0) {
    int maxX = 5;
    int maxY = 2;
    int maxZ = 3;
    ArrayThreeDimensions::Array3D arr(maxX, maxY, maxZ);
    arr[1][1][1] = 100500;

    EXPECT_EQ(arr.GetElem(1, 1, 1), 100500);
}

TEST(Array3DTestSuite, AssignmentTest1) {
    int maxX = 5;
    int maxY = 2;
    int maxZ = 3;
    ArrayThreeDimensions::Array3D arr(maxX, maxY, maxZ);
    arr[0][0][0] = 100500;

    EXPECT_EQ(arr.GetElem(0, 0, 0), 100500);
}

TEST(Array3DTestSuite, AssignmentTest3) {
    int maxX = 5;
    int maxY = 2;
    int maxZ = 3;
    ArrayThreeDimensions::Array3D arr(maxX, maxY, maxZ);
    arr[4][1][2] = 100500;

    EXPECT_EQ(arr.GetElem(4, 1, 2), 100500);
}

TEST(Array3DTestSuite, Constructor) {
    Array3D arr(3, 4, 5);

    EXPECT_EQ(arr.SizeX(), 3);
    EXPECT_EQ(arr.SizeY(), 4);
    EXPECT_EQ(arr.SizeZ(), 5);
}

TEST(Array3DTestSuite, MakeOperator) {
//    Array3D arr(3, 4, 5);
    Array3D arr = Array3D::make_array(3, 4, 5);
    arr[0][0][0] = 1;
    arr[1][2][3] = 2;
    arr[2][3][4] = 3;

    EXPECT_EQ(arr.GetElem(0,0,0), 1);
    EXPECT_EQ(arr.GetElem(1, 2, 3), 2);
    EXPECT_EQ(arr.GetElem(2, 3, 4), 3);
}

TEST(Array3DTestSuite, PutAndGetElement) {
    Array3D arr(3, 4, 5);

    arr.PutElemInd(0, 1);
    arr.PutElemInd(31, 2);
    arr.PutElemInd(59, 3);
    arr.PutElem(0, 0, 1, 4);
    arr.PutElem(1, 1, 1, 5);
    arr.PutElem(2, 2, 2, 6);

    EXPECT_EQ(arr.GetElemInd(0), 1);
    EXPECT_EQ(arr.GetElemInd(31), 2);
    EXPECT_EQ(arr.GetElemInd(59), 3);
    EXPECT_EQ(arr.GetElem(0, 0, 1), 4);
    EXPECT_EQ(arr.GetElem(1, 1, 1), 5);
    EXPECT_EQ(arr.GetElem(2, 2, 2), 6);
}

TEST(Array3DTestSuite, MultiplyAddSub) {
    Array3D arr1(2, 2, 2);
    Array3D arr2(2, 2, 2);

    arr1[0][0][0] = 1;
    arr1[0][0][1] = 2;
    arr1[0][1][0] = 3;
    arr1[0][1][1] = 4;
    arr1[1][0][0] = 5;
    arr1[1][0][1] = 6;
    arr1[1][1][0] = 7;
    arr1[1][1][1] = 8;

    arr2[0][0][0] = 2;
    arr2[0][0][1] = 3;
    arr2[0][1][0] = 4;
    arr2[0][1][1] = 5;
    arr2[1][0][0] = 6;
    arr2[1][0][1] = 7;
    arr2[1][1][0] = 8;
    arr2[1][1][1] = 9;

    Array3D result1 = arr1 * 2;
    EXPECT_EQ(result1.GetElem(0, 0, 0), 2);
    EXPECT_EQ(result1.GetElem(0, 0, 1), 4);
    EXPECT_EQ(result1.GetElem(0, 1, 0), 6);
    EXPECT_EQ(result1.GetElem(0, 1, 1), 8);
    EXPECT_EQ(result1.GetElem(1, 0, 0), 10);
    EXPECT_EQ(result1.GetElem(1, 0, 1), 12);
    EXPECT_EQ(result1.GetElem(1, 1, 0), 14);
    EXPECT_EQ(result1.GetElem(1, 1, 1), 16);

    Array3D result2 = arr1 + arr2;
    EXPECT_EQ(result2.GetElem(0, 0, 0), 3);
    EXPECT_EQ(result2.GetElem(0, 0, 1), 5);
    EXPECT_EQ(result2.GetElem(0, 1, 0), 7);
    EXPECT_EQ(result2.GetElem(0, 1, 1), 9);
    EXPECT_EQ(result2.GetElem(1, 0, 0), 11);
    EXPECT_EQ(result2.GetElem(1, 0, 1), 13);
    EXPECT_EQ(result2.GetElem(1, 1, 0), 15);
    EXPECT_EQ(result2.GetElem(1, 1, 1), 17);

    Array3D result3 = arr2 - arr1;
    EXPECT_EQ(result3.GetElem(0, 0, 0), 1);
    EXPECT_EQ(result3.GetElem(0, 0, 1), 1);
    EXPECT_EQ(result3.GetElem(0, 1, 0), 1);
    EXPECT_EQ(result3.GetElem(0, 1, 1), 1);
    EXPECT_EQ(result3.GetElem(1, 0, 0), 1);
    EXPECT_EQ(result3.GetElem(1, 0, 1), 1);
    EXPECT_EQ(result3.GetElem(1, 1, 0), 1);
    EXPECT_EQ(result3.GetElem(1, 1, 1), 1);
}

TEST(Array3DTestSuite, InputOutputOperator) {
    Array3D arr1(2, 2, 2);
    Array3D arr2(2, 2, 2);

    arr1[0][0][0] = 1;
    arr1[0][0][1] = 2;
    arr1[0][1][0] = 3;
    arr1[0][1][1] = 4;
    arr1[1][0][0] = 5;
    arr1[1][0][1] = 6;
    arr1[1][1][0] = 7;
    arr1[1][1][1] = 8;

    arr2[0][0][0] = 2;
    arr2[0][0][1] = 3;
    arr2[0][1][0] = 4;
    arr2[0][1][1] = 5;
    arr2[1][0][0] = 6;
    arr2[1][0][1] = 7;
    arr2[1][1][0] = 8;
    arr2[1][1][1] = 9;

    std::stringstream ss;
    ss << arr1;
    std::string output = ss.str();
    std::string expectedOutput = "1 2 \n3 4 \n\n5 6 \n7 8 \n\n";
    EXPECT_EQ(output, expectedOutput);

    std::stringstream ss2(expectedOutput);
    ss2 >> arr2;

    for (int i = 0; i < arr1.SizeX(); i++) {
        for (int j = 0; j < arr1.SizeY(); j++) {
            for (int k = 0; k < arr1.SizeZ(); k++) {
                EXPECT_EQ(arr2.GetElem(i, j, k), arr1.GetElem(i, j,k ));
            }
        }
    }
}

TEST(Array3DTestSuite, InputOutputOperator2) {
    Array3D arr1(2, 2, 2);
    Array3D arr2(2, 2, 2);

    arr1[0][0][0] = 1;
    arr1[0][0][1] = 2;
    arr1[0][1][0] = 3;
    arr1[0][1][1] = 4;
    arr1[1][0][0] = 5;
    arr1[1][0][1] = 6;
    arr1[1][1][0] = 7;
    arr1[1][1][1] = 8;

    arr2[0][0][0] = 2;
    arr2[0][0][1] = 3;
    arr2[0][1][0] = 4;
    arr2[0][1][1] = 5;
    arr2[1][0][0] = 6;
    arr2[1][0][1] = 7;
    arr2[1][1][0] = 8;
    arr2[1][1][1] = 9;

    std::stringstream ss;
    for (int i = 0; i < arr1.SizeX(); i++) {
        for (int j = 0; j < arr1.SizeY(); j++) {
            for (int k = 0; k < arr1.SizeZ(); k++) {
                ss << arr1[i][j][k] << " ";
            }
        }
    }
    std::string output = ss.str();
    std::string expectedOutput = "1 2 3 4 5 6 7 8 ";
    EXPECT_EQ(output, expectedOutput);

    std::stringstream ss2(expectedOutput);
    for (int i = 0; i < arr1.SizeX(); i++) {
        for (int j = 0; j < arr1.SizeY(); j++) {
            for (int k = 0; k < arr1.SizeZ(); k++) {
                ss2 >> arr2[i][j][k];
            }
        }
    }
    
    for (int i = 0; i < arr1.SizeX(); i++) {
        for (int j = 0; j < arr1.SizeY(); j++) {
            for (int k = 0; k < arr1.SizeZ(); k++) {
                EXPECT_EQ(arr2.GetElem(i, j, k), arr1.GetElem(i, j,k ));
            }
        }
    }
}

TEST(Array3DTestSuite, MaxUInt17) {
    int maxX = 5;
    int maxY = 2;
    int maxZ = 3;

    ArrayThreeDimensions::Array3D arr = ArrayThreeDimensions::Array3D::make_array(maxX, maxY, maxZ);
    const int kMaxUInt17 = 131071;

    int n = kMaxUInt17 - 14;

    for (int i = 0; i < maxX; ++i) {
        for (int j = 0; j < maxY; ++j) {
            for (int k = 0; k < maxZ; ++k) {
                arr[i][j][k] = n;
                ++n;
            }
        }
    }
    n = kMaxUInt17 - 14;
    for (int i=0; i < (maxX * maxY * maxZ); ++i) {
        EXPECT_EQ(arr.GetElemInd(i), n % (kMaxUInt17+1));
        ++n;
    }
}
