#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
using namespace std;

using Shape = vector<long long>;

bool broadcastShape(const Shape& shapeA, const Shape& shapeB, Shape& broadcastedShape) {
    for (long long dimension : shapeA) {
        if (dimension <= 0) return false;
    }

    for (long long dimension : shapeB) {
        if (dimension <= 0) return false;
    }

    int outputRank = max((int)shapeA.size(), (int)shapeB.size());
    broadcastedShape.assign(outputRank, 1);

    for (int index = 0; index < outputRank; index++) {
        long long dimensionA = 1;
        long long dimensionB = 1;

        if (index < (int)shapeA.size()) {
            dimensionA = shapeA[shapeA.size() - 1 - index];
        }

        if (index < (int)shapeB.size()) {
            dimensionB = shapeB[shapeB.size() - 1 - index];
        }

        if (dimensionA == dimensionB) {
            broadcastedShape[outputRank - 1 - index] = dimensionA;
        } else if (dimensionA == 1) {
            broadcastedShape[outputRank - 1 - index] = dimensionB;
        } else if (dimensionB == 1) {
            broadcastedShape[outputRank - 1 - index] = dimensionA;
        } else {
            return false;
        }
    }

    return true;
}

bool sameShape(bool actualValid, const Shape& actualShape, bool expectedValid, const Shape& expectedShape) {
    if (actualValid != expectedValid) return false;
    if (!actualValid) return true;
    return actualShape == expectedShape;
}

void runTest(const Shape& shapeA, const Shape& shapeB, bool expectedValid, const Shape& expectedShape, string testName) {
    Shape result;
    bool valid = broadcastShape(shapeA, shapeB, result);

    if (sameShape(valid, result, expectedValid, expectedShape)) {
        cout << "[PASS] " << testName << endl;
    } else {
        cout << "[FAIL] " << testName << endl;
    }
}

int main() {
    cout << "Test Results:" << endl;

    runTest({2, 3}, {2, 3}, true, {2, 3}, "same rank valid");
    runTest({2, 3}, {2, 4}, false, {}, "same rank invalid mismatch");
    runTest({2, 3, 4}, {3, 4}, true, {2, 3, 4}, "different rank valid");
    runTest({2, 3}, {2}, false, {}, "different rank invalid due to right alignment");
    runTest({}, {4, 5}, true, {4, 5}, "scalar broadcasts to tensor");
    runTest({1, 5, 1}, {3, 1, 7}, true, {3, 5, 7}, "broadcasting in multiple dimensions");
    runTest({1, 2, 1, 4, 1}, {3, 1, 5, 1, 6}, true, {3, 2, 5, 4, 6}, "high dimensional valid shape");
    runTest({6, 8}, {6}, false, {}, "wrong-side padding trap");
    runTest({6, 8}, {8}, true, {6, 8}, "right-aligned vector valid");
    runTest({2, 0, 3}, {2, 1, 3}, false, {}, "zero dimension rejected");
    runTest({2, -1, 3}, {2, 1, 3}, false, {}, "negative dimension rejected");

    cout << endl;
    cout << "Benchmark Results:" << endl;
    cout << "Rank\tAverage Time (ns)" << endl;

    vector<int> ranks = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};
    int iterations = 100000;
    volatile long long checksum = 0;

    for (int rank : ranks) {
        Shape a(rank, 64);
        Shape b(rank);

        for (int i = 0; i < rank; i++) {
            b[i] = (i % 2 == 0) ? 1 : 64;
        }

        Shape result;

        auto start = chrono::high_resolution_clock::now();

        for (int i = 0; i < iterations; i++) {
            bool valid = broadcastShape(a, b, result);
            if (valid) checksum += result.size();
        }

        auto end = chrono::high_resolution_clock::now();

        double totalTime = chrono::duration<double, nano>(end - start).count();
        double averageTime = totalTime / iterations;

        cout << rank << "\t" << averageTime << endl;
    }

    return 0;
}