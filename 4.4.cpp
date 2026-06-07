#include <iostream>
#include <algorithm>

void matmul_tiled(const float* matrixA,
                  const float* matrixB,
                  float* resultMatrix,
                  int numRows,
                  int numCols,
                  int sharedDimension,
                  int tileSize)
{
    for(int row = 0; row < numRows; row++)
    {
        for(int col = 0; col < numCols; col++)
        {
            resultMatrix[row * numCols + col] = 0.0f;
        }
    }

    for(int rowTileStart = 0; rowTileStart < numRows; rowTileStart += tileSize)
    {
        for(int kTileStart = 0; kTileStart < sharedDimension; kTileStart += tileSize)
        {
            for(int colTileStart = 0; colTileStart < numCols; colTileStart += tileSize)
            {
                int rowTileEnd = std::min(rowTileStart + tileSize, numRows);
                int kTileEnd = std::min(kTileStart + tileSize, sharedDimension);
                int colTileEnd = std::min(colTileStart + tileSize, numCols);

                for(int row = rowTileStart; row < rowTileEnd; row++)
                {
                    for(int k = kTileStart; k < kTileEnd; k++)
                    {
                        float valueA = matrixA[row * sharedDimension + k];

                        for(int col = colTileStart; col < colTileEnd; col++)
                        {
                            resultMatrix[row * numCols + col] += valueA * matrixB[k * numCols + col];
                        }
                    }
                }
            }
        }
    }
}

int main()
{
    float matrixA[] = {1, 2, 3, 4};
    float matrixB[] = {5, 6, 7, 8};
    float resultMatrix[4];

    matmul_tiled(matrixA, matrixB, resultMatrix, 2, 2, 2, 1);

    for(int row = 0; row < 2; row++)
    {
        for(int col = 0; col < 2; col++)
        {
            std::cout << resultMatrix[row * 2 + col] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}