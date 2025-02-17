#include "BlockBuffer.h"
//#include "StaticBuffer.h"

#include <cstdlib>
#include <cstring>

#include <iostream>


// the declarations for these functions can be found in "BlockBuffer.h"

BlockBuffer::BlockBuffer(int blockNum) {
  // initialise this.blockNum with the argument
  this->blockNum = blockNum;
}

// calls the parent class constructor
RecBuffer::RecBuffer(int blockNum) : BlockBuffer::BlockBuffer(blockNum) {}


//STAGE 2
/*int BlockBuffer::getHeader(struct HeadInfo *head) {
  unsigned char buffer[BLOCK_SIZE];

  // read the block at this.blockNum into the buffer
  Disk::readBlock(buffer, this->blockNum);

  // populate the numEntries, numAttrs and numSlots fields in *head
  memcpy(&head->numSlots, buffer + 24, 4);
  memcpy(&head->numEntries, buffer+16, 4);
  memcpy(&head->numAttrs, buffer+20, 4);
  memcpy(&head->rblock, buffer+12, 4);
  memcpy(&head->lblock, buffer+8, 4);
   memcpy(&head->lblock, buffer+4, 4);

  return SUCCESS;
}

// load the record at slotNum into the argument pointer
int RecBuffer::getRecord(union Attribute *rec, int slotNum) {
  struct HeadInfo head;

  // get the header using this->getHeader() function
  this->getHeader(&head);

  int attrCount = head.numAttrs;
  int slotCount = head.numSlots;
unsigned char buffer[BLOCK_SIZE];
  // read the block at this.blockNum into a buffer
    Disk::readBlock(buffer, this->blockNum);

  //record at slotNum will be at offset HEADER_SIZE + slotMapSize + (recordSize * slotNum)
     //- each record will have size attrCount * ATTR_SIZE
    // - slotMap will be of size slotCount
  
  int recordSize = attrCount * ATTR_SIZE;
  int slotMapSize =slotCount;

  unsigned char *slotPointer = buffer+(HEADER_SIZE+slotMapSize+(recordSize*slotNum));

  // load the record into the rec data structure
  memcpy(rec, slotPointer, recordSize);

  return SUCCESS;
}
*/
// int RecBuffer::setRecord(union Attribute *record, int slotNum)
// {
//     // get the header using this.getHeader() function
//     HeadInfo head;
//     BlockBuffer::getHeader(&head);

//     int attrCount = head.numAttrs;
//     int slotCount = head.numSlots;

//     // read the block at this.blockNum into a buffer
//     unsigned char buffer[BLOCK_SIZE];
//     Disk::readBlock(buffer, this->blockNum);

//     /* record at slotNum will be at offset HEADER_SIZE + slotMapSize + (recordSize * slotNum)
//        - each record will have size attrCount * ATTR_SIZE
//        - slotMap will be of size slotCount
//     */
    
//     int recordSize = attrCount * ATTR_SIZE;
//     unsigned char *slotPointer = buffer + (32 + slotCount + (recordSize * slotNum)); // calculate buffer + offset

//     // load the record into the rec data structure
//     memcpy(slotPointer, record, recordSize);

//     Disk::writeBlock(buffer, this->blockNum);

//     return SUCCESS;
// }

//STAGE 3

int BlockBuffer::getHeader(struct HeadInfo *head) {

  unsigned char *bufferPtr;
  int ret = loadBlockAndGetBufferPtr(&bufferPtr);
  if (ret != SUCCESS) {
    return ret;   // return any errors that might have occured in the process
  }

  // ... (the rest of the logic is as in stage 2)

  memcpy(&head->numSlots, bufferPtr + 24, 4);
  memcpy(&head->numEntries, bufferPtr+16, 4);
  memcpy(&head->numAttrs, bufferPtr+20, 4);
  memcpy(&head->rblock, bufferPtr+12, 4);
  memcpy(&head->lblock,bufferPtr+8, 4);
   memcpy(&head->pblock, bufferPtr+4, 4);

  return SUCCESS;

}

int RecBuffer::getRecord(union Attribute *rec, int slotNum) {
  // ...
  unsigned char *bufferPtr;
  int ret = loadBlockAndGetBufferPtr(&bufferPtr);
  if (ret != SUCCESS) {
    return ret;
  }
 struct HeadInfo head;

  // get the header using this->getHeader() function
  this->getHeader(&head);
  //printf("head=%d\n",head.numAttrs);

  int attrCount = head.numAttrs;
  int slotCount = head.numSlots;

  // record at slotNum will be at offset HEADER_SIZE + slotMapSize + (recordSize * slotNum)
    // - each record will have size attrCount * ATTR_SIZE
   //  - slotMap will be of size slotCount
  
  int recordSize = attrCount * ATTR_SIZE;
  int slotMapSize =slotCount;

  unsigned char *slotPointer = bufferPtr+(HEADER_SIZE+slotMapSize+(recordSize*slotNum));
  

  // load the record into the rec data structure
  memcpy(rec, slotPointer, recordSize);
  return SUCCESS;
 
}

int BlockBuffer::loadBlockAndGetBufferPtr(unsigned char **buffPtr) {
  // check whether the block is already present in the buffer using StaticBuffer.getBufferNum()
  int bufferNum = StaticBuffer::getBufferNum(this->blockNum);

  if (bufferNum == E_BLOCKNOTINBUFFER) {
    bufferNum = StaticBuffer::getFreeBuffer(this->blockNum);

    if (bufferNum == E_OUTOFBOUND) {
      return E_OUTOFBOUND;
    }

    Disk::readBlock(StaticBuffer::blocks[bufferNum], this->blockNum);
  }

  // store the pointer to this buffer (blocks[bufferNum]) in *buffPtr
  *buffPtr = StaticBuffer::blocks[bufferNum];

  return SUCCESS;
}

//STAGE 4

/* used to get the slotmap from a record block
NOTE: this function expects the caller to allocate memory for `*slotMap`
*/
int RecBuffer::getSlotMap(unsigned char *slotMap) {
  unsigned char *bufferPtr;

  // get the starting address of the buffer containing the block using loadBlockAndGetBufferPtr().
  int ret = loadBlockAndGetBufferPtr(&bufferPtr);
  if (ret != SUCCESS) {
    return ret;
  }

  struct HeadInfo head;
  // get the header of the block using getHeader() function
    this->getHeader(&head);

  int slotCount = head.numSlots;

  // get a pointer to the beginning of the slotmap in memory by offsetting HEADER_SIZE
  unsigned char *slotMapInBuffer = bufferPtr + HEADER_SIZE;

  // copy the values from `slotMapInBuffer` to `slotMap` (size is `slotCount`)
    memcpy(slotMap, slotMapInBuffer, slotCount);

  return SUCCESS;
}

int RecBuffer::compareAttrs( Attribute attr1,  Attribute attr2, int attrType) {


    double diff;
    // if attrType == STRING
      //     diff = strcmp(attr1.sval, attr2.sval)
      // else
    //     diff = attr1.nval - attr2.nval

    if (attrType == STRING){
     diff = strcmp(attr1.sVal, attr2.sVal);
    }
    else{
    diff = attr1.nVal - attr2.nVal;
    }


    /*
    if diff > 0 then return 1
    if diff < 0 then return -1
    if diff = 0 then return 0
    */
    if(diff>0){
    return 1;}
    if(diff<0){
    return -1;}
    if(diff=0){
    return 0;}
    return 0;
    
}
//STAGE 5
int RecBuffer::setRecord(union Attribute *rec, int slotNum) {
    unsigned char *bufferPtr;
    /* get the starting address of the buffer containing the block
       using loadBlockAndGetBufferPtr(&bufferPtr). */
    int bufferNum=BlockBuffer::loadBlockAndGetBufferPtr(&bufferPtr);
    if(bufferNum!=SUCCESS){
      return bufferNum;
    }


    // if loadBlockAndGetBufferPtr(&bufferPtr) != SUCCESS
        // return the value returned by the call.

    /* get the header of the block using the getHeader() function */
    HeadInfo head;
    BlockBuffer::getHeader(&head);
  
    // get number of attributes in the block.
    int attrCount=head.numAttrs;
    int slotCount=head.numSlots;
    // get the number of slots in the block.
    if(slotNum>slotCount or slotNum<0){
      return E_OUTOFBOUND;
    }// if input slotNum is not in the permitted range return E_OUTOFBOUND.

    /* offset bufferPtr to point to the beginning of the record at required
       slot. the block contains the header, the slotmap, followed by all
       the records. so, for example,
       record at slot x will be at bufferPtr + HEADER_SIZE + (x*recordSize)
       copy the record from `rec` to buffer using memcpy
       (hint: a record will be of size ATTR_SIZE * numAttrs)
    */
  //  unsigned char buffer[BLOCK_SIZE];
  // Disk::readBlock(buffer, this->blockNum);
   int recordSize=attrCount*ATTR_SIZE;
   unsigned char *slotPointer =bufferPtr +(32 + slotCount + (recordSize * slotNum));

   memcpy(slotPointer,rec,recordSize);
    // update dirty bit using setDirtyBit()
    int ret=StaticBuffer::setDirtyBit(this->blockNum);
    if(ret!=SUCCESS){
      std::cout<<"something srong with the setDirty function";
    }
    /* (the above function call should not fail since the block is already
       in buffer and the blockNum is valid. If the call does fail, there
       exists some other issue in the code) */

    // return SUCCESS
    return SUCCESS;
}


//STAGE 6

BlockBuffer::BlockBuffer(char blocktype){
    // allocate a block on the disk and a buffer in memory to hold the new block of
    // given type using getFreeBlock function and get the return error codes if any.
	// * NOTE: this line should be changed later
	int blockType = blocktype == 'R' ? REC : UNUSED_BLK; 

	int blockNum = getFreeBlock(blockType);
	if (blockNum < 0 || blockNum >= DISK_BLOCKS) {
		std::cout << "Error: Block is not available\n";
		this->blockNum = blockNum;
		return;
	}

	// // int bufferIndex = StaticBuffer::getFreeBuffer(blockNum);
	// // if (bufferIndex < 0 || bufferIndex >= BUFFER_CAPACITY) {
	// // 	std::cout << "Error: Buffer is not available\n";
	// // 	return;
	// // }
		
    // set the blockNum field of the object to that of the allocated block
    // number if the method returned a valid block number,
    // otherwise set the error code returned as the block number.

	this->blockNum = blockNum;

    // (The caller must check if the constructor allocatted block successfully
    // by checking the value of block number field.)
}

RecBuffer::RecBuffer() : BlockBuffer('R') {}



int BlockBuffer::setHeader(struct HeadInfo *head) {

  unsigned char *bufferPtr;
  int bufferreturn = loadBlockAndGetBufferPtr(&bufferPtr);
  if (bufferreturn != SUCCESS) {
    return bufferreturn;
  }
  // get the starting address of the buffer containing the block using
  // loadBlockAndGetBufferPtr(&bufferPtr).

  // if loadBlockAndGetBufferPtr(&bufferPtr) != SUCCESS
  // return the value returned by the call.

  // cast bufferPtr to type HeadInfo*
  struct HeadInfo *bufferHeader = (struct HeadInfo *)bufferPtr;

  // copy the fields of the HeadInfo pointed to by head (except reserved) to
  // the header of the block (pointed to by bufferHeader)
  //(hint: bufferHeader->numSlots = head->numSlots )
  bufferHeader->numSlots = head->numSlots;
  bufferHeader->lblock = head->lblock;
  bufferHeader->numEntries = head->numEntries;
  bufferHeader->pblock = head->pblock;
  bufferHeader->rblock = head->rblock;
  bufferHeader->blockType = head->blockType;
  bufferHeader->numAttrs=head->numAttrs;
  

  // update dirty bit by calling StaticBuffer::setDirtyBit()
  // if setDirtyBit() failed, return the error code
  int setDirty = StaticBuffer::setDirtyBit(this->blockNum);
  return setDirty;

  // return SUCCESS;
}

int BlockBuffer::getFreeBlock(int blockType) {
  int blockNum;
  for (blockNum = 0; blockNum < DISK_BLOCKS; blockNum++) {
    if (StaticBuffer::blockAllocMap[blockNum] == UNUSED_BLK) {
      break;
    }
  }
  // iterate through the StaticBuffer::blockAllocMap and find the block number
  // of a free block in the disk.
  if (blockNum == DISK_BLOCKS)
    return E_DISKFULL;

  // if no block is free, return E_DISKFULL.

  // set the object's blockNum to the block number of the free block.
  this->blockNum = blockNum;

  // find a free buffer using StaticBuffer::getFreeBuffer() .
  int bufferNum = StaticBuffer::getFreeBuffer(blockNum);
  if (bufferNum < 0 or bufferNum >= BUFFER_CAPACITY) {
    printf("Error:buffer is full\n");
    return bufferNum;
  }
  // initialize the header of the block passing a struct HeadInfo with values
  // pblock: -1, lblock: -1, rblock: -1, numEntries: 0, numAttrs: 0, numSlots: 0
  // to the setHeader() function.
  struct HeadInfo header;
  header.lblock = header.pblock = header.rblock = -1;
  header.numAttrs = header.numEntries = header.numSlots = 0;
  setHeader(&header);

  // update the block type of the block to the input block type using
  // setBlockType().
  setBlockType(blockType);
  return blockNum;

  // return block number of the free block.
}

int BlockBuffer::setBlockType(int blockType) {

  unsigned char *bufferPtr;
  int ret = loadBlockAndGetBufferPtr(&bufferPtr);
  if (ret != SUCCESS) {
    return SUCCESS;
  }
  /* get the starting address of the buffer containing the block
     using loadBlockAndGetBufferPtr(&bufferPtr). */

  // if loadBlockAndGetBufferPtr(&bufferPtr) != SUCCESS
  // return the value returned by the call.

  // store the input block type in the first 4 bytes of the buffer.
  // (hint: cast bufferPtr to int32_t* and then assign it)
  *((int32_t *)bufferPtr) = blockType;

  // update the StaticBuffer::blockAllocMap entry corresponding to the
  // object's block number to `blockType`.
  StaticBuffer::blockAllocMap[this->blockNum] = blockType;
  // update dirty bit by calling StaticBuffer::setDirtyBit()
  return StaticBuffer::setDirtyBit(this->blockNum);
  // if setDirtyBit() failed
  // return the returned value from the call

  // return SUCCESS
}

int RecBuffer::setSlotMap(unsigned char *slotMap) {
  unsigned char *bufferPtr;
  /* get the starting address of the buffer containing the block using
     loadBlockAndGetBufferPtr(&bufferPtr). */
  int ret = loadBlockAndGetBufferPtr(&bufferPtr);
  if (ret != SUCCESS) {
    return ret;
  }
  // if loadBlockAndGetBufferPtr(&bufferPtr) != SUCCESS
  // return the value returned by the call.

  // get the header of the block using the getHeader() function
  HeadInfo header;
  getHeader(&header);
  int numSlots = header.numSlots; /* the number of slots in the block */
  ;
  memcpy(bufferPtr + HEADER_SIZE, slotMap, numSlots);
  // the slotmap starts at bufferPtr + HEADER_SIZE. Copy the contents of the
  // argument `slotMap` to the buffinter replacing the existing slotmap.
  // Note that size of slotmap is `numSlots`
  ret = StaticBuffer::setDirtyBit(this->blockNum);
  // update dirty bit using StaticBuffer::setDirtyBit
  // if setDirtyBit failed, return the value returned by the call
  return SUCCESS;

  // return SUCCESS
}

int BlockBuffer::getBlockNum() {
  return this->blockNum;

}