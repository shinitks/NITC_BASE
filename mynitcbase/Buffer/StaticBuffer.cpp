//#include "StaticBuffer.h"

#include "StaticBuffer.h"
#include <iostream>
#include <cstring>

// Define the static members
unsigned char StaticBuffer::blocks[BUFFER_CAPACITY][BLOCK_SIZE];
struct BufferMetaInfo StaticBuffer::metainfo[BUFFER_CAPACITY];
unsigned char StaticBuffer::blockAllocMap[DISK_BLOCKS];

// Constructor and Destructor
StaticBuffer::StaticBuffer() {
   
  /*  unsigned char *blockPtr = nullptr;
    int ret = Disk::readBlock(blockPtr, 0);  // Adjust the block number for the allocation map
    if (ret != SUCCESS) {
        std::cerr << "Failed to read the block allocation map from disk." << std::endl;
        // Handle the error (you can return or throw an exception depending on your design)
    } else {
        // Copy the data from the block to the blockAllocMap
        memcpy(blockAllocMap, blockPtr, sizeof(blockAllocMap));
    }
    
*/
for (int i = 0, blockMapslot = 0; i < 4; i++) {
    unsigned char buffer[BLOCK_SIZE];
    Disk::readBlock(buffer, i);
    for (int slot = 0; slot < BLOCK_SIZE; slot++, blockMapslot++) {
      StaticBuffer::blockAllocMap[blockMapslot] = buffer[slot];
    }
  }
    // Step 2: Initialize metainfo array
    for (int i = 0; i < BUFFER_CAPACITY; i++) {
        metainfo[i].free = true;          // Mark the buffer as free
        metainfo[i].dirty = false;        // Mark the buffer as not dirty
        metainfo[i].blockNum = -1;        // No block is assigned yet
        metainfo[i].timeStamp = -1;       // No timestamp assigned
    }
}



  StaticBuffer::~StaticBuffer() {
    // Step 1: Write the blockAllocMap back to disk
    // We assume that the blockAllocMap should be written to a specific block on disk (e.g., block number 0).
    int ret = Disk::writeBlock(blockAllocMap, 0);  // Adjust the block number for the allocation map
    if (ret != SUCCESS) {
        std::cerr << "Failed to write the block allocation map to disk." << std::endl;
        // Handle the error as needed, possibly throwing an exception or returning
    }
    for (int i = 0, blockMapslot = 0; i < 4; i++) {
    unsigned char buffer[BLOCK_SIZE];
    for (int slot = 0; slot < BLOCK_SIZE; slot++, blockMapslot++) {
      buffer[slot] = blockAllocMap[blockMapslot];
    }
    Disk::writeBlock(buffer, i);
  }

    // Step 2: Iterate through the metainfo entries and write dirty buffers back to disk
    for (int i = 0; i < BUFFER_CAPACITY; i++) {
        // Check if the buffer is in use (not free) and has been modified (dirty)
        if (!metainfo[i].free && metainfo[i].dirty) {
            unsigned char *bufferPtr = blocks[i];  // Pointer to the actual buffer block

            // Write the dirty buffer block to disk using the block number stored in metainfo[i]
            ret = Disk::writeBlock(bufferPtr, metainfo[i].blockNum);
            if (ret != SUCCESS) {
                std::cerr << "Failed to write dirty block (Block " << metainfo[i].blockNum << ") to disk." << std::endl;
                // Handle the error as needed, possibly continuing or returning
            }
            // After writing the block, mark it as no longer dirty
            metainfo[i].dirty = false;  // Buffer is no longer dirty after it's written to disk
        }
    }
}

/*int StaticBuffer::getStaticBlockType(int blockNum) {
    // Step 1: Validate blockNum
    if (blockNum <= 0 || blockNum >= DISK_BLOCKS) {
        // Return an error code if blockNum is out of bounds
        return E_OUTOFBOUND;
    }

    // Step 2: Access the blockAllocMap to get the block's type
    unsigned char blockType = blockAllocMap[blockNum];

    // Step 3: Return the block type (type cast to integer if necessary)
    return static_cast<int>(blockType);  // Assuming blockAllocMap holds block types as unsigned chars
}

int StaticBuffer::setDirtyBit(int blockNum) {
    // Step 1: Find the buffer index corresponding to the block using getBufferNum
    int bufferNum = getBufferNum(blockNum);

    // Step 2: Check if the block is not found in the buffer
    if (bufferNum == E_BLOCKNOTINBUFFER) {
        return E_BLOCKNOTINBUFFER;  // Block is not in buffer
    }

    // Step 3: Check if the block number is out of bounds
    if (bufferNum == E_OUTOFBOUND) {
        return E_OUTOFBOUND;  // Block number is out of bounds
    }

    // Step 4: Set the dirty bit of that buffer to true
    metainfo[bufferNum].dirty = true;

    // Step 5: Return SUCCESS
    return SUCCESS;
}

*/
int StaticBuffer::getBufferNum(int blockNum) {
    // Step 1: Validate the block number
    if (blockNum <= 0 || blockNum >= DISK_BLOCKS) {
        return E_OUTOFBOUND;  // Block number is out of bounds
    }

    // Step 2: Traverse through the metainfo array to find the buffer number
    for (int i = 0; i < BUFFER_CAPACITY; i++) {
        if (metainfo[i].blockNum == blockNum) {
            // Block found in buffer, return the buffer index (i)
            return i;
        }
    }

    // Step 3: Return error if block not found in any buffer
    return E_BLOCKNOTINBUFFER;
}
int StaticBuffer::getFreeBuffer(int blockNum) {
    // Step 1: Validate blockNum
    if (blockNum <= 0 || blockNum >= DISK_BLOCKS) {
        return E_OUTOFBOUND;  // Block number is out of bounds
    }

    // Step 2: Increase the timestamp of all occupied buffers
    for (int i = 0; i < BUFFER_CAPACITY; i++) {
        if (!metainfo[i].free) {
            metainfo[i].timeStamp++;  // Increment the timestamp of used buffers
        }
    }

    // Step 3: Find a free buffer
    int bufferNum = -1;
    for (int i = 0; i < BUFFER_CAPACITY; i++) {
        if (metainfo[i].free) {
            bufferNum = i;
            break;
        }
    }

    // Step 4: If no free buffer is found, evict the oldest buffer
    if (bufferNum == -1) {
        int maxTimeStamp = -1;
        int oldestBuffer = -1;
        
        // Find the buffer with the largest timestamp (oldest)
        for (int i = 0; i < BUFFER_CAPACITY; i++) {
            if (!metainfo[i].free && metainfo[i].timeStamp > maxTimeStamp) {
                maxTimeStamp = metainfo[i].timeStamp;
                oldestBuffer = i;
            }
        }

        bufferNum = oldestBuffer;

        // If the oldest buffer is dirty, write it back to disk
        if (metainfo[bufferNum].dirty) {
            Disk::writeBlock(blocks[bufferNum], metainfo[bufferNum].blockNum);
        }
    }

    // Step 5: Update the metaInfo entry for the selected buffer
    metainfo[bufferNum].free = false;  // Mark the buffer as not free
    metainfo[bufferNum].dirty = false; // Mark the buffer as not dirty (since we're loading a new block)
    metainfo[bufferNum].blockNum = blockNum;  // Set the block number to the input block number
    metainfo[bufferNum].timeStamp = 0;  // Set the timestamp to 0 (as this buffer is now used)

    // Step 6: Return the buffer index
    return bufferNum;
}

int StaticBuffer::setDirtyBit(int blockNum){
    // find the buffer index corresponding to the block using getBufferNum().
    int bufferIndex=getBufferNum(blockNum);

    // if block is not present in the buffer (bufferNum = E_BLOCKNOTINBUFFER)
    //     return E_BLOCKNOTINBUFFER
    if(bufferIndex==E_BLOCKNOTINBUFFER){
      return E_BLOCKNOTINBUFFER;
    }
    if(bufferIndex==E_OUTOFBOUND){
      return E_OUTOFBOUND;
    }else{
      metainfo[bufferIndex].dirty=true;
    }
    return SUCCESS;
    
}