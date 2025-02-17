#include "OpenRelTable.h"
#include "AttrCacheTable.h"
#include <cstring>
#include <iostream>
#define ATTRCAT_RELID 1
// OpenRelTable::OpenRelTable() {

//   // initialize relCache and attrCache with nullptr
//   for (int i = 0; i < MAX_OPEN; ++i) {
//     RelCacheTable::relCache[i] = nullptr;
//     AttrCacheTable::attrCache[i] = nullptr;
//   }

//   // Setting up Relation Cache entries 
//   // (we need to populate relation cache with entries for the relation catalog
//   //  and attribute catalog.)

//   // setting up Relation Catalog relation in the Relation Cache Table
//   RecBuffer relCatBlock(RELCAT_BLOCK);

//   Attribute relCatRecord[RELCAT_NO_ATTRS];  // Buffer for Relation Catalog record
//   relCatBlock.getRecord(relCatRecord, RELCAT_SLOTNUM_FOR_RELCAT);

//   struct RelCacheEntry relCacheEntry;
//   RelCacheTable::recordToRelCatEntry(relCatRecord, &relCacheEntry.relCatEntry);
//   relCacheEntry.recId.block = RELCAT_BLOCK;
//   relCacheEntry.recId.slot = RELCAT_SLOTNUM_FOR_RELCAT;

//   // allocate this on the heap because we want it to persist outside this function
//   RelCacheTable::relCache[RELCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
//   *(RelCacheTable::relCache[RELCAT_RELID]) = relCacheEntry;

// // setting up Attribute Catalog relation in the Relation Cache Table 

//   // set up the relation cache entry for the attribute catalog similarly
//   // from the record at RELCAT_SLOTNUM_FOR_ATTRCAT
//   Attribute attrCatRecordForRel[RELCAT_NO_ATTRS];  // Buffer for Attribute Catalog record
//   relCatBlock.getRecord(attrCatRecordForRel, RELCAT_SLOTNUM_FOR_ATTRCAT);

//   struct RelCacheEntry attrCatRelCacheEntry;
//   RelCacheTable::recordToRelCatEntry(attrCatRecordForRel, &attrCatRelCacheEntry.relCatEntry);
//   attrCatRelCacheEntry.recId.block = RELCAT_BLOCK;
//   attrCatRelCacheEntry.recId.slot = RELCAT_SLOTNUM_FOR_ATTRCAT;

//   // allocate and store in relCache
//   RelCacheTable::relCache[ATTRCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
//   *(RelCacheTable::relCache[ATTRCAT_RELID]) = attrCatRelCacheEntry;

//   // Setting up Attribute cache entries 
//   // (we need to populate attribute cache with entries for the relation catalog
//   //  and attribute catalog.)

//  // setting up Relation Catalog relation in the Attribute Cache Table
//   RecBuffer attrCatBlock(ATTRCAT_BLOCK);

//   Attribute attrCatRecordForCache[ATTRCAT_NO_ATTRS];  // Buffer for each attribute record

//   // iterate through all the attributes of the relation catalog and create a linked
//   // list of AttrCacheEntry (slots 0 to 5)
//   // for each of the entries, set
//   //    attrCacheEntry.recId.block = ATTRCAT_BLOCK;
//   //    attrCacheEntry.recId.slot = i   (0 to 5)
//   //    and attrCacheEntry.next appropriately
//   // NOTE: allocate each entry dynamically using malloc

//   AttrCacheEntry* prevEntry = nullptr;  // to track the previous entry for linking
//   for (int i = 0; i < 6; ++i) {
//     attrCatBlock.getRecord(attrCatRecordForCache, i);  // Get the record for the current slot

//     AttrCacheEntry* newEntry = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
//     AttrCacheTable::recordToAttrCatEntry(attrCatRecordForCache, &newEntry->attrCatEntry);

//     newEntry->recId.block = ATTRCAT_BLOCK;
//     newEntry->recId.slot = i;
//     newEntry->next = nullptr;

//     // link the entries together
//     if (prevEntry) {
//       prevEntry->next = newEntry;
//     } else {
//       AttrCacheTable::attrCache[RELCAT_RELID] = newEntry;  // Head of the linked list
//     }

//     prevEntry = newEntry;  // Update the previous entry
//   }

//  //setting up Attribute Catalog relation in the Attribute Cache Table 
//   // set up the attributes of the attribute cache similarly.
//   // read slots 6-11 from attrCatBlock and initialise recId appropriately
//   prevEntry = nullptr;
//   for (int i = 6; i < 12; i++) {
//     attrCatBlock.getRecord(attrCatRecordForCache, i);  // Get the record for the current slot

//     AttrCacheEntry* newEntry = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
//     AttrCacheTable::recordToAttrCatEntry(attrCatRecordForCache, &newEntry->attrCatEntry);

//     newEntry->recId.block = ATTRCAT_BLOCK;
//     newEntry->recId.slot = i;
//     newEntry->next = nullptr;

//     // Link the entries in the attribute cache
//     if (prevEntry) {
//       prevEntry->next = newEntry;
//     } else {
//       AttrCacheTable::attrCache[ATTRCAT_RELID] = newEntry;
//     // Head of the linked list
//     }

//     prevEntry = newEntry; 
//     //printf("rel-name:%s\n",newEntry->attrCatEntry.attrName);
//     // Update the previous entry
//   }
//   //STAGE 3 EXERCISE

//   Attribute stuCatRecord[4];  // Buffer for Relation Catalog record
//   relCatBlock.getRecord(stuCatRecord,2);
// char srcRel[ATTR_SIZE] = "Students";
//   int srcRelId = OpenRelTable::getRelId(srcRel);      // we'll implement this later

//   struct RelCacheEntry stuCacheEntry;
//   RelCacheTable::recordToRelCatEntry(stuCatRecord, &stuCacheEntry.relCatEntry);
//   stuCacheEntry.recId.block = RELCAT_BLOCK;
//   stuCacheEntry.recId.slot = 2;

//   // allocate this on the heap because we want it to persist outside this function
//   RelCacheTable::relCache[2] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
//   *(RelCacheTable::relCache[2]) = stuCacheEntry;
//     prevEntry = nullptr;
//   for (int i = 12; i < 16; i++) {
//     attrCatBlock.getRecord(attrCatRecordForCache, i);  // Get the record for the current slot

//     AttrCacheEntry* newEntry = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
//     AttrCacheTable::recordToAttrCatEntry(attrCatRecordForCache, &newEntry->attrCatEntry);

//     newEntry->recId.block = ATTRCAT_BLOCK;
//     newEntry->recId.slot = i;
//     newEntry->next = nullptr;

//     // Link the entries in the attribute cache
//     if (prevEntry) {
//       prevEntry->next = newEntry;
//     } else {
//       AttrCacheTable::attrCache[2] = newEntry;
//     // Head of the linked list
//     }

//     prevEntry = newEntry; 
//     //printf("rel-name:%s\n",newEntry->attrCatEntry.attrName);
//     // Update the previous entry
//   }
  

// }
// OpenRelTable::~OpenRelTable() {
//   // free all the memory that you allocated in the constructor
//    if (RelCacheTable::relCache[RELCAT_RELID]) {
//     free(RelCacheTable::relCache[RELCAT_RELID]);
//   }

//   // Free memory for Attribute Cache entries for RELCAT_RELID
//   struct AttrCacheEntry* current = AttrCacheTable::attrCache[RELCAT_RELID];
//   while (current) {
//     struct AttrCacheEntry* next = current->next;
//     free(current);
//     current = next;
//   }

//   // Free memory for Attribute Cache entries for ATTRCAT_RELID
//   current = AttrCacheTable::attrCache[ATTRCAT_RELID];
//   while (current) {
//     struct AttrCacheEntry* next = current->next;
//     free(current);
//     current = next;
//   }
// }
// //STAGE 4

// /* This function will open a relation having name `relName`.
// Since we are currently only working with the relation and attribute catalog, we
// will just hardcode it. In subsequent stages, we will loop through all the relations
// and open the appropriate one.
// */
// int OpenRelTable::getRelId(char relName[ATTR_SIZE]) {

//   // if relname is RELCAT_RELNAME, return RELCAT_RELID
//   // if relname is ATTRCAT_RELNAME, return ATTRCAT_RELID
//    if(strcmp(relName,RELCAT_RELNAME)==0){
//    return RELCAT_RELID;}
//    if(strcmp(relName,ATTRCAT_RELNAME)==0){
//    return ATTRCAT_RELID;}
// if (strcmp(relName, "Students") == 0) {
//         return 2;  // "Students" relation has rel-id 2, assuming it's set up
//     }

//   return E_RELNOTOPEN;
// }





//STAGE 5
// OpenRelTable.cpp
OpenRelTableMetaInfo OpenRelTable::tableMetaInfo[MAX_OPEN];


OpenRelTable::OpenRelTable() {


  // initialize relCache and attrCache with nullptr
  for (int i = 0; i < MAX_OPEN; ++i) {
    RelCacheTable::relCache[i] = nullptr;
    AttrCacheTable::attrCache[i] = nullptr;
     tableMetaInfo[i].free=false;
  }

  RecBuffer relCatBlock(RELCAT_BLOCK);

  Attribute relCatRecord[RELCAT_NO_ATTRS];  // Buffer for Relation Catalog record
  relCatBlock.getRecord(relCatRecord, RELCAT_SLOTNUM_FOR_RELCAT);

  struct RelCacheEntry relCacheEntry;
  RelCacheTable::recordToRelCatEntry(relCatRecord, &relCacheEntry.relCatEntry);
  relCacheEntry.recId.block = RELCAT_BLOCK;
  relCacheEntry.recId.slot = RELCAT_SLOTNUM_FOR_RELCAT;

  // allocate this on the heap because we want it to persist outside this function
  RelCacheTable::relCache[RELCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
  *(RelCacheTable::relCache[RELCAT_RELID]) = relCacheEntry;


  Attribute attrCatRecordForRel[RELCAT_NO_ATTRS];  // Buffer for Attribute Catalog record
  relCatBlock.getRecord(attrCatRecordForRel, RELCAT_SLOTNUM_FOR_ATTRCAT);

  struct RelCacheEntry attrCatRelCacheEntry;
  RelCacheTable::recordToRelCatEntry(attrCatRecordForRel, &attrCatRelCacheEntry.relCatEntry);
  attrCatRelCacheEntry.recId.block = RELCAT_BLOCK;
  attrCatRelCacheEntry.recId.slot = RELCAT_SLOTNUM_FOR_ATTRCAT;

  // allocate and store in relCache
  RelCacheTable::relCache[ATTRCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
  *(RelCacheTable::relCache[ATTRCAT_RELID]) = attrCatRelCacheEntry;

  
  RecBuffer attrCatBlock(ATTRCAT_BLOCK);

  Attribute attrCatRecordForCache[ATTRCAT_NO_ATTRS];  // Buffer for each attribute record


  AttrCacheEntry* prevEntry = nullptr;  // to track the previous entry for linking
  for (int i = 0; i < 6; ++i) {
    attrCatBlock.getRecord(attrCatRecordForCache, i);  // Get the record for the current slot

    AttrCacheEntry* newEntry = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
    AttrCacheTable::recordToAttrCatEntry(attrCatRecordForCache, &newEntry->attrCatEntry);

    newEntry->recId.block = ATTRCAT_BLOCK;
    newEntry->recId.slot = i;
    newEntry->next = nullptr;

    // link the entries together
    if (prevEntry) {
      prevEntry->next = newEntry;
    } else {
      AttrCacheTable::attrCache[RELCAT_RELID] = newEntry;  // Head of the linked list
    }

    prevEntry = newEntry;  // Update the previous entry
  }

 
  prevEntry = nullptr;
  for (int i = 6; i < 12; i++) {
    attrCatBlock.getRecord(attrCatRecordForCache, i);  // Get the record for the current slot

    AttrCacheEntry* newEntry = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
    AttrCacheTable::recordToAttrCatEntry(attrCatRecordForCache, &newEntry->attrCatEntry);

    newEntry->recId.block = ATTRCAT_BLOCK;
    newEntry->recId.slot = i;
    newEntry->next = nullptr;

    // Link the entries in the attribute cache
    if (prevEntry) {
      prevEntry->next = newEntry;
    } else {
      AttrCacheTable::attrCache[ATTRCAT_RELID] = newEntry;
    // Head of the linked list
    }

    prevEntry = newEntry; 
   
  }
  //STAGE 3 EXERCISE

  /*Attribute stuCatRecord[4];  // Buffer for Relation Catalog record
  relCatBlock.getRecord(stuCatRecord,2);
char srcRel[ATTR_SIZE] = "Students";
  int srcRelId = OpenRelTable::getRelId(srcRel);      // we'll implement this later

  struct RelCacheEntry stuCacheEntry;
  RelCacheTable::recordToRelCatEntry(stuCatRecord, &stuCacheEntry.relCatEntry);
  stuCacheEntry.recId.block = RELCAT_BLOCK;
  stuCacheEntry.recId.slot = 2;

  // allocate this on the heap because we want it to persist outside this function
  RelCacheTable::relCache[2] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
  *(RelCacheTable::relCache[2]) = stuCacheEntry;
    prevEntry = nullptr;
  for (int i = 12; i < 16; i++) {
    attrCatBlock.getRecord(attrCatRecordForCache, i);  // Get the record for the current slot

    AttrCacheEntry* newEntry = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
    AttrCacheTable::recordToAttrCatEntry(attrCatRecordForCache, &newEntry->attrCatEntry);

    newEntry->recId.block = ATTRCAT_BLOCK;
    newEntry->recId.slot = i;
    newEntry->next = nullptr;

    // Link the entries in the attribute cache
    if (prevEntry) {
      prevEntry->next = newEntry;
    } else {
      AttrCacheTable::attrCache[2] = newEntry;
    // Head of the linked list
    }

    prevEntry = newEntry; 
    
  }*/
  
  //STAGE 5


       tableMetaInfo[RELCAT_RELID].free=false;
       strcpy(tableMetaInfo[RELCAT_RELID].relName, "RELATIONCAT");
       tableMetaInfo[ATTRCAT_RELID].free=false;
       strcpy(tableMetaInfo[ATTRCAT_RELID].relName, "ATTRIBUTECAT");


}

int OpenRelTable::getFreeOpenRelTableEntry() {

  
    for (int i = 0; i < MAX_OPEN; ++i) {
    if (tableMetaInfo[i].relName == nullptr || tableMetaInfo[i].relName[0] == '\0') {
       return i;
 }
  }
return E_CACHEFULL;
}


  
int OpenRelTable::getRelId(char relName[ATTR_SIZE]) {

    for (int i = 0; i < MAX_OPEN; ++i) {
        // Compare relName with tableMetaInfo[i].relName byte-by-byte
        if (strcmp(tableMetaInfo[i].relName, relName) == 0) {
            return i;

        }
    }

    return E_RELNOTOPEN;
}
  
 




int OpenRelTable::openRel(char relName[ATTR_SIZE]) {
    // Check if the relation relName already has an entry in the Open Relation Table
    int existingRelId = OpenRelTable::getRelId((char*)relName);
    if (existingRelId >=0) {
        return existingRelId; // Return the existing relation ID
    }

    // Find a free slot in the Open Relation Table
    int relId = OpenRelTable::getFreeOpenRelTableEntry();
    if (relId == E_CACHEFULL) {
        return E_CACHEFULL; // No free slot available
    }

    /** Setting up Relation Cache entry for the relation **/

    // Reset the search index for the Relation Catalog
    RelCacheTable::resetSearchIndex(RELCAT_RELID);

    // Search for the relation name in the Relation Catalog
    Attribute searchValue;
    strcpy((char*)searchValue.sVal, (char*)relName);
    RecId relcatRecId = BlockAccess::linearSearch(RELCAT_RELID, RELCAT_ATTR_RELNAME, searchValue, EQ);

    // If the relation is not found in the Relation Catalog, return an error
    if (relcatRecId.block == -1 && relcatRecId.slot == -1) {
        return E_RELNOTEXIST;
    }

    // Use an instance of RecBuffer to retrieve the Relation Catalog record
    RecBuffer relCatBuffer(relcatRecId.block);
      Attribute relCatRecord[RELCAT_NO_ATTRS];  // Buffer for Relation Catalog record

    relCatBuffer.getRecord(relCatRecord, relcatRecId.slot);

    // Create a Relation Cache Entry
    RelCacheEntry* relCacheEntry = (RelCacheEntry*)malloc(sizeof(RelCacheEntry));
    RelCacheTable::recordToRelCatEntry(relCatRecord, &relCacheEntry->relCatEntry);
    // relCacheEntry->recId = relcatRecId; // Set the record ID for the Relation Cache Entry
relCacheEntry->recId.block = relcatRecId.block;
	relCacheEntry->recId.slot = relcatRecId.slot;
    // Assign the Relation Cache Entry to the relId-th entry of the RelCacheTable
    RelCacheTable::relCache[relId] = relCacheEntry;

    /** Setting up Attribute Cache entry for the relation **/

    AttrCacheEntry* listHead = nullptr;  // Head of the linked list for attribute cache entries
    AttrCacheEntry* currentEntry = nullptr;

    // Reset the search index for the Attribute Catalog
    RelCacheTable::resetSearchIndex(ATTRCAT_RELID);

    while (true) {
        // Search for the next attribute of the relation in the Attribute Catalog
        RecId attrcatRecId = BlockAccess::linearSearch(ATTRCAT_RELID, RELCAT_ATTR_RELNAME, searchValue, EQ);

        // If no more attributes are found, exit the loop
        if (attrcatRecId.block == -1 && attrcatRecId.slot == -1) {
            break;
        }

        // Use an instance of RecBuffer to retrieve the Attribute Catalog record
        RecBuffer attrCatBuffer(attrcatRecId.block);
          Attribute attrCatRecordForCache[BLOCK_SIZE];  // Buffer for each attribute record

        attrCatBuffer.getRecord(attrCatRecordForCache, attrcatRecId.slot);

        // Create a new Attribute Cache Entry
        AttrCacheEntry* attrCacheEntry = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
        AttrCacheTable::recordToAttrCatEntry(attrCatRecordForCache, &attrCacheEntry->attrCatEntry);
        attrCacheEntry->recId = attrcatRecId; // Set the record ID for the Attribute Cache Entry
        attrCacheEntry->next = nullptr;

        // Add the new entry to the linked list
        if (listHead == nullptr) {
            listHead = attrCacheEntry;
        } else {
            currentEntry->next = attrCacheEntry;
        }
        currentEntry = attrCacheEntry;
    }

    // Set the relId-th entry of the AttrCacheTable to the head of the linked list
    AttrCacheTable::attrCache[relId] = listHead;

    /** Setting up metadata in the Open Relation Table for the relation **/

    // Update the relId-th entry of the tableMetaInfo
    OpenRelTable::tableMetaInfo[relId].free = false;
    strcpy(OpenRelTable::tableMetaInfo[relId].relName, (char*)relName);
    return relId; // Return the relation ID
}
int OpenRelTable::closeRel(int relId) {
  // Check if relId corresponds to a relation catalog or attribute catalog
  if (relId == RELCAT_RELID|| relId == ATTRCAT_RELID) {
    return E_NOTPERMITTED;  // Cannot close the system catalogs
  }

  // Check if relId is within valid bounds
  if (relId < 0 || relId >= MAX_OPEN) {
    return E_OUTOFBOUND;  // Invalid relId
  }

  // Check if the relation is not open (i.e., it corresponds to a free slot)
  if (tableMetaInfo[relId].free==true) {
    return E_RELNOTOPEN;  // Relation is not open
  }

  // Free the memory allocated for relation and attribute caches
  // Assuming relCache and attrCache are arrays or pointers
 // Free the memory for attribute cache entry
RelCacheTable::relCache[relId] = nullptr;
    AttrCacheTable::attrCache[relId] = nullptr;
  // Update tableMetaInfo to set relId as a free slot
  tableMetaInfo[relId].free=true;
  tableMetaInfo[relId].relName[0] = '\0';


  // Set the corresponding entries in the caches to nullptr

  return SUCCESS;  // Successfully closed the relation
}
OpenRelTable::~OpenRelTable() {
  // Close all open relations from rel-id = 2 onwards
  for (int i = 2; i < MAX_OPEN; ++i) {
    if (!tableMetaInfo[i].free) {
      // If the relation is open (i.e., not free), close it
      OpenRelTable::closeRel(i);
    }
  }

  // Free memory for system catalogs (rel-id 0 and 1)
  // Assuming you want to free the caches for rel-id 0 and rel-id 1 too
  delete RelCacheTable::relCache[0];
  delete AttrCacheTable::attrCache[0];
  delete RelCacheTable::relCache[1];
  delete  AttrCacheTable::attrCache[1];

  // Optionally: Reset the cache entries for rel-id 0 and rel-id 1 to nullptr
  RelCacheTable::relCache[0] = nullptr;
  AttrCacheTable::attrCache[0] = nullptr;
  RelCacheTable::relCache[1] = nullptr;
  AttrCacheTable::attrCache[1] = nullptr;
}
