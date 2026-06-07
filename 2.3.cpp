#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool broadcastShape(const vector<long long>& shapeA,
                    const vector<long long>& shapeB,
                    vector<long long>& broadcastedShape) {
    
    for (long long dimension : shapeA) {
        if (dimension <= 0) {
            return false;
        }
    }

    for (long long dimension : shapeB) {
        if (dimension <= 0) {
            return false;
        }
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

void printShape(const vector<long long>& shape) {
    cout << "[";
    for (int i = 0; i < (int)shape.size(); i++) {
        cout << shape[i];
        if (i != (int)shape.size() - 1) {
            cout << ", ";
        }
    }
    cout << "]";
}

int main() {
    vector<long long> shapeA = {2, 3, 4};
    vector<long long> shapeB = {3, 4};
    vector<long long> result;

    bool valid = broadcastShape(shapeA, shapeB, result);

    if (valid) {
        cout << "Broadcasting is valid" << endl;
        cout << "Output shape = ";
        printShape(result);
        cout << endl;
    } else {
        cout << "Broadcasting is invalid" << endl;
    }

    return 0;
}