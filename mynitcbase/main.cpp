#include "Buffer/StaticBuffer.h"
#include "Cache/OpenRelTable.h"
#include "Disk_Class/Disk.h"
#include "FrontendInterface/FrontendInterface.h"
#include <cstring>
#include <iostream>
 #include <readline/history.h>

//STAGE 1
//int main(int argc, char *argv[]) {
  //Disk disk_run;

  //unsigned char buffer[BLOCK_SIZE];
  //Disk::readBlock(buffer, 7000);
  //char message[] = "hello";
  //memcpy(buffer + 20, message, 6);
  //Disk::writeBlock(buffer, 7000);

 // unsigned char buffer2[BLOCK_SIZE];
  //char message2[6];
  //Disk::readBlock(buffer2, 7000);
  //memcpy(message2, buffer2 + 20, 6);
  //std::cout << message2;

  //return 0;
//}



//STAGE 1  EXERCISE QUESTION-1

/*int main(int argc, char *argv[]) {
  Disk disk_run;

  unsigned char buffer[BLOCK_SIZE];
  Disk::readBlock(buffer, 0);
  //  unsigned char buffer2[BLOCK_SIZE];
 char message2[10];
  memcpy(message2, buffer, 10);

   for (int i = 0; i < 10; ++i) {
       std::cout << (int)(unsigned char)message2[i] << " ";  // Casting to unsigned char for safe conversion to int
    }
 return 0;
}
*/

// STAGE 2

/*int main(int argc, char *argv[]) {
  Disk disk_run;

  // create objects for the relation catalog and attribute catalog
  RecBuffer relCatBuffer(RELCAT_BLOCK);
  RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

  HeadInfo relCatHeader;
  HeadInfo attrCatHeader;

  // load the headers of both the blocks into relCatHeader and attrCatHeader.
  // (we will implement these functions later)
  relCatBuffer.getHeader(&relCatHeader);
  attrCatBuffer.getHeader(&attrCatHeader);

  for (int i=0;i<relCatHeader.numEntries;i++) {

    Attribute relCatRecord[RELCAT_NO_ATTRS]; // will store the record from the relation catalog

    relCatBuffer.getRecord(relCatRecord, i);

    printf("Relation: %s\n", relCatRecord[RELCAT_REL_NAME_INDEX].sVal);

    for (int j=0;j<attrCatHeader.numEntries;j++) {

      // declare attrCatRecord and load the attribute catalog entry into it
        Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
            attrCatBuffer.getRecord(attrCatRecord, j);

                  if (strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal,relCatRecord[RELCAT_REL_NAME_INDEX].sVal) == 0) {
        const char *attrType = attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER ? "NUM" : "STR";
        printf("  %s: %s\n", attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, attrType);
      }
    }
    printf("\n");
  }

  return 0;
}*/

//STAGE 2 EXERCISE QUES 1

/*int main(int argc, char *argv[]) {
  Disk disk_run;
  StaticBuffer buffer;

  // Create objects for the relation catalog and attribute catalog
  RecBuffer relCatBuffer(RELCAT_BLOCK);
  RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

  HeadInfo relCatHeader;
  HeadInfo attrCatHeader;

  // Load the headers of both the blocks into relCatHeader and attrCatHeader.
  relCatBuffer.getHeader(&relCatHeader);
  attrCatBuffer.getHeader(&attrCatHeader);
int atrinum=0;
  for (int i=0;i<relCatHeader.numEntries;i++) {
    Attribute relCatRecord[RELCAT_NO_ATTRS]; // will store the record from the relation catalog

    relCatBuffer.getRecord(relCatRecord, i);

    printf("Relation: %s\n", relCatRecord[RELCAT_REL_NAME_INDEX].sVal);

    for (int j=0;j<attrCatHeader.numEntries;j++) {

      // declare attrCatRecord and load the attribute catalog entry into it
        Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
            attrCatBuffer.getRecord(attrCatRecord, j);

                  if (strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal,relCatRecord[RELCAT_REL_NAME_INDEX].sVal) == 0) {
                  atrinum++;
        const char *attrType = attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER ? "NUM" : "STR";
        printf("  %s: %s\n", attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, attrType);
      }
    }
    if (attrCatHeader.rblock != -1&&atrinum>=attrCatHeader.numEntries) {
        // Debugging: Print the next block to load
        atrinum=0;

        // If the next block exists, create a new RecBuffer for that block
        attrCatBuffer = RecBuffer(attrCatHeader.rblock);  // Load the next block
        attrCatBuffer.getHeader(&attrCatHeader);  // Reload the header for the new block
        
        for (int j=0;j<attrCatHeader.numEntries;j++) {

      // declare attrCatRecord and load the attribute catalog entry into it
        Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
            attrCatBuffer.getRecord(attrCatRecord, j);

                  if (strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal,relCatRecord[RELCAT_REL_NAME_INDEX].sVal) == 0) {
                  atrinum++;
        const char *attrType = attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER ? "NUM" : "STR";
        printf("  %s: %s\n", attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, attrType);
      }
    }
      }
         printf("\n");
  
  
 }
  return 0;}

*/
//STAGE 2 EXERCISE QUES 2


/*
int main(int argc, char *argv[]) {
  Disk disk_run;

  // Create objects for the relation catalog and attribute catalog
  RecBuffer relCatBuffer(RELCAT_BLOCK);
  RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

  HeadInfo relCatHeader;
  HeadInfo attrCatHeader;

  // Load the headers of both the blocks into relCatHeader and attrCatHeader.
  relCatBuffer.getHeader(&relCatHeader);
  attrCatBuffer.getHeader(&attrCatHeader);
int atrinum=0;
  for (int i=0;i<relCatHeader.numEntries;i++) {
    Attribute relCatRecord[RELCAT_NO_ATTRS]; // will store the record from the relation catalog

    relCatBuffer.getRecord(relCatRecord, i);

    printf("Relation: %s\n", relCatRecord[RELCAT_REL_NAME_INDEX].sVal);

    for (int j=0;j<attrCatHeader.numEntries;j++) {

      // declare attrCatRecord and load the attribute catalog entry into it
        Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
            attrCatBuffer.getRecord(attrCatRecord, j);

                  if (strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal,relCatRecord[RELCAT_REL_NAME_INDEX].sVal) == 0) {
                  atrinum++;
        const char *attrType = attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER ? "NUM" : "STR";
        if (strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal, "Students") == 0 &&
    strcmp(attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, "Class") == 0) {
    printf("  Batch: %s\n", attrType);
    memcpy(attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, "Batch", strlen("Batch") + 1);
    attrCatBuffer.setRecord(attrCatRecord, j);



}
else{
        printf("  %s: %s\n", attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, attrType);
      }
    }}
    if (attrCatHeader.rblock != -1&&atrinum>=attrCatHeader.numEntries) {
        // Debugging: Print the next block to load
        atrinum=0;

        // If the next block exists, create a new RecBuffer for that block
        attrCatBuffer = RecBuffer(attrCatHeader.rblock);  // Load the next block
        attrCatBuffer.getHeader(&attrCatHeader);  // Reload the header for the new block
        
        for (int j=0;j<attrCatHeader.numEntries;j++) {

      // declare attrCatRecord and load the attribute catalog entry into it
        Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
            attrCatBuffer.getRecord(attrCatRecord, j);

                  if (strcmp(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal,relCatRecord[RELCAT_REL_NAME_INDEX].sVal) == 0) {
                  atrinum++;
        const char *attrType = attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal == NUMBER ? "NUM" : "STR";
        printf("  %s: %s\n", attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, attrType);
      }
    }
      }
         printf("\n");
  
  
 }
  return 0;
}
*/



//STAGE 3

/*int main(int argc, char *argv[]) {
  Disk disk_run;
  StaticBuffer buffer;
  OpenRelTable cache;

  

for(int i=0;i<2;i++){
struct  RelCatEntry* relCatBuf= (struct RelCatEntry*)malloc(sizeof(struct RelCatEntry));
RelCacheTable::getRelCatEntry(i,relCatBuf);
 printf("Relation: %s\n",relCatBuf->relName);
 for(int j=0;j<relCatBuf->numAttrs;j++){
 struct AttrCatEntry * attrCatBuf= (struct AttrCatEntry *)malloc(sizeof(struct AttrCatEntry));
 AttrCacheTable::getAttrCatEntry(i,j,attrCatBuf);
 printf("  %s:", attrCatBuf->attrName);
 if(attrCatBuf->attrType==0){
 printf("NUM\n");}
 else{
 printf("STR\n");}
 
 
 }
 
}
  return 0;
}
*/

//STAGE 3 EXERCISE

/*int main(int argc, char *argv[]) {
  Disk disk_run;
  StaticBuffer buffer;
  OpenRelTable cache;

  

for(int i=0;i<3;i++){
struct  RelCatEntry* relCatBuf= (struct RelCatEntry*)malloc(sizeof(struct RelCatEntry));
RelCacheTable::getRelCatEntry(i,relCatBuf);
 printf("Relation: %s\n",relCatBuf->relName);
 for(int j=0;j<relCatBuf->numAttrs;j++){
 struct AttrCatEntry * attrCatBuf= (struct AttrCatEntry *)malloc(sizeof(struct AttrCatEntry));
 AttrCacheTable::getAttrCatEntry(i,j,attrCatBuf);
 printf("  %s:", attrCatBuf->attrName);
 if(attrCatBuf->attrType==0){
 printf("NUM\n");}
 else{
 printf("STR\n");}
 
 
 }
 
}
  return 0;
}*/

//STAGE 4

int main(int argc, char *argv[]) {
  Disk disk_run;
  StaticBuffer buffer;
  OpenRelTable cache;

  return FrontendInterface::handleFrontend(argc, argv);
}


