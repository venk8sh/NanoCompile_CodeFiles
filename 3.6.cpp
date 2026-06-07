#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

struct TensorInfo {
    std::string name;
    int size;
    int firstCreated;
    int lastUsed;
};

struct Buffer {
    std::string id;
    int capacity;
    bool free;
};

int main() {
    std::vector<TensorInfo> tensors = {
        {"A",24,1,2},
        {"B",24,2,8},
        {"C",32,3,5},
        {"S",32,4,5},
        {"D",32,5,6},
        {"E",32,6,7},
        {"F",24,7,8},
        {"G",24,8,9},
        {"Y",24,9,9}
    };

    std::vector<Buffer> buffers;
    std::unordered_map<std::string,std::string> assignedBuffer;

    int peakMemory = 0;
    int bufferCount = 0;

    for(const auto& tensor : tensors) {

        for(const auto& oldTensor : tensors) {
            if(oldTensor.lastUsed < tensor.firstCreated &&
               assignedBuffer.count(oldTensor.name))
            {
                std::string bufferId = assignedBuffer[oldTensor.name];

                for(auto& buffer : buffers) {
                    if(buffer.id == bufferId)
                        buffer.free = true;
                }
            }
        }

        int chosen = -1;

        for(int bufferIndex = 0; bufferIndex < buffers.size(); bufferIndex++) {
            if(buffers[bufferIndex].free &&
               buffers[bufferIndex].capacity >= tensor.size)
            {
                if(chosen == -1 ||
                   buffers[bufferIndex].capacity < buffers[chosen].capacity)
                {
                    chosen = bufferIndex;
                }
            }
        }

        if(chosen == -1) {
            Buffer newBuffer;
            newBuffer.id = "B" + std::to_string(bufferCount++);
            newBuffer.capacity = tensor.size;
            newBuffer.free = false;

            buffers.push_back(newBuffer);

            assignedBuffer[tensor.name] = newBuffer.id;
        }
        else {
            buffers[chosen].free = false;
            assignedBuffer[tensor.name] = buffers[chosen].id;
        }

        int currentMemory = 0;

        for(const auto& buffer : buffers)
            currentMemory += buffer.capacity;

        peakMemory = std::max(peakMemory, currentMemory);
    }

    std::cout << "Peak Memory = "
              << peakMemory
              << " KB" << std::endl;

    return 0;
}
