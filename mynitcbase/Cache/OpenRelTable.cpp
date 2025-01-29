#include "OpenRelTable.h"
#include "AttrCacheTable.h"
#include <cstring>
#include <iostream>
#define ATTRCAT_RELID 1
OpenRelTable::OpenRelTable() {

  // initialize relCache and attrCache with nullptr
  for (int i = 0; i < MAX_OPEN; ++i) {
    RelCacheTable::relCache[i] = nullptr;
    AttrCacheTable::attrCache[i] = nullptr;
  }

  // Setting up Relation Cache entries 
  // (we need to populate relation cache with entries for the relation catalog
  //  and attribute catalog.)

  // setting up Relation Catalog relation in the Relation Cache Table
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

// setting up Attribute Catalog relation in the Relation Cache Table 

  // set up the relation cache entry for the attribute catalog similarly
  // from the record at RELCAT_SLOTNUM_FOR_ATTRCAT
  Attribute attrCatRecordForRel[RELCAT_NO_ATTRS];  // Buffer for Attribute Catalog record
  relCatBlock.getRecord(attrCatRecordForRel, RELCAT_SLOTNUM_FOR_ATTRCAT);

  struct RelCacheEntry attrCatRelCacheEntry;
  RelCacheTable::recordToRelCatEntry(attrCatRecordForRel, &attrCatRelCacheEntry.relCatEntry);
  attrCatRelCacheEntry.recId.block = RELCAT_BLOCK;
  attrCatRelCacheEntry.recId.slot = RELCAT_SLOTNUM_FOR_ATTRCAT;

  // allocate and store in relCache
  RelCacheTable::relCache[ATTRCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
  *(RelCacheTable::relCache[ATTRCAT_RELID]) = attrCatRelCacheEntry;

  // Setting up Attribute cache entries 
  // (we need to populate attribute cache with entries for the relation catalog
  //  and attribute catalog.)

 // setting up Relation Catalog relation in the Attribute Cache Table
  RecBuffer attrCatBlock(ATTRCAT_BLOCK);

  Attribute attrCatRecordForCache[ATTRCAT_NO_ATTRS];  // Buffer for each attribute record

  // iterate through all the attributes of the relation catalog and create a linked
  // list of AttrCacheEntry (slots 0 to 5)
  // for each of the entries, set
  //    attrCacheEntry.recId.block = ATTRCAT_BLOCK;
  //    attrCacheEntry.recId.slot = i   (0 to 5)
  //    and attrCacheEntry.next appropriately
  // NOTE: allocate each entry dynamically using malloc

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

 //setting up Attribute Catalog relation in the Attribute Cache Table 
  // set up the attributes of the attribute cache similarly.
  // read slots 6-11 from attrCatBlock and initialise recId appropriately
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
    //printf("rel-name:%s\n",newEntry->attrCatEntry.attrName);
    // Update the previous entry
  }
  //STAGE 3 EXERCISE

  Attribute stuCatRecord[4];  // Buffer for Relation Catalog record
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
    //printf("rel-name:%s\n",newEntry->attrCatEntry.attrName);
    // Update the previous entry
  }
  

}
OpenRelTable::~OpenRelTable() {
  // free all the memory that you allocated in the constructor
   if (RelCacheTable::relCache[RELCAT_RELID]) {
    free(RelCacheTable::relCache[RELCAT_RELID]);
  }

  // Free memory for Attribute Cache entries for RELCAT_RELID
  struct AttrCacheEntry* current = AttrCacheTable::attrCache[RELCAT_RELID];
  while (current) {
    struct AttrCacheEntry* next = current->next;
    free(current);
    current = next;
  }

  // Free memory for Attribute Cache entries for ATTRCAT_RELID
  current = AttrCacheTable::attrCache[ATTRCAT_RELID];
  while (current) {
    struct AttrCacheEntry* next = current->next;
    free(current);
    current = next;
  }
}
//STAGE 4

/* This function will open a relation having name `relName`.
Since we are currently only working with the relation and attribute catalog, we
will just hardcode it. In subsequent stages, we will loop through all the relations
and open the appropriate one.
*/
int OpenRelTable::getRelId(char relName[ATTR_SIZE]) {

  // if relname is RELCAT_RELNAME, return RELCAT_RELID
  // if relname is ATTRCAT_RELNAME, return ATTRCAT_RELID
   if(strcmp(relName,RELCAT_RELNAME)==0){
   return RELCAT_RELID;}
   if(strcmp(relName,ATTRCAT_RELNAME)==0){
   return ATTRCAT_RELID;}
if (strcmp(relName, "Students") == 0) {
        return 2;  // "Students" relation has rel-id 2, assuming it's set up
    }

  return E_RELNOTOPEN;
}


//STAGE 4 EXERCISE
/*OpenRelTable::OpenRelTable() {
    // initialize relCache and attrCache with nullptr
    for (int i = 0; i < MAX_OPEN; ++i) {
        RelCacheTable::relCache[i] = nullptr;
        AttrCacheTable::attrCache[i] = nullptr;
    }


    // Set up Relation Catalog and Attribute Catalog entries
    RecBuffer relCatBlock(RELCAT_BLOCK);
    Attribute relCatRecord[RELCAT_NO_ATTRS];  // Buffer for Relation Catalog record
    relCatBlock.getRecord(relCatRecord, RELCAT_SLOTNUM_FOR_RELCAT);

    struct RelCacheEntry relCacheEntry;
    RelCacheTable::recordToRelCatEntry(relCatRecord, &relCacheEntry.relCatEntry);
    relCacheEntry.recId.block = RELCAT_BLOCK;
    relCacheEntry.recId.slot = RELCAT_SLOTNUM_FOR_RELCAT;

    // Allocate this on the heap because we want it to persist outside this function
    RelCacheTable::relCache[RELCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
    *(RelCacheTable::relCache[RELCAT_RELID]) = relCacheEntry;

    // Set up Attribute Catalog in Relation Cache Table
    Attribute attrCatRecordForRel[RELCAT_NO_ATTRS];  // Buffer for Attribute Catalog record
    relCatBlock.getRecord(attrCatRecordForRel, RELCAT_SLOTNUM_FOR_ATTRCAT);

    struct RelCacheEntry attrCatRelCacheEntry;
    RelCacheTable::recordToRelCatEntry(attrCatRecordForRel, &attrCatRelCacheEntry.relCatEntry);
    attrCatRelCacheEntry.recId.block = RELCAT_BLOCK;
    attrCatRelCacheEntry.recId.slot = RELCAT_SLOTNUM_FOR_ATTRCAT;

    // Allocate and store in relCache
    RelCacheTable::relCache[ATTRCAT_RELID] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
    *(RelCacheTable::relCache[ATTRCAT_RELID]) = attrCatRelCacheEntry;


    // Set up attributes for Relation Catalog in the Attribute Cache Table
    RecBuffer attrCatBlock(ATTRCAT_BLOCK);
    Attribute attrCatRecordForCache[ATTRCAT_NO_ATTRS];  // Buffer for each attribute record

    struct AttrCacheEntry* prevEntry = nullptr;
    for (int i = 0; i < 6; ++i) {
        attrCatBlock.getRecord(attrCatRecordForCache, i);  // Get the record for the current slot

        AttrCacheEntry* newEntry = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
        AttrCacheTable::recordToAttrCatEntry(attrCatRecordForCache, &newEntry->attrCatEntry);

        newEntry->recId.block = ATTRCAT_BLOCK;
        newEntry->recId.slot = i;
        newEntry->next = nullptr;

        // Link the entries together
        if (prevEntry) {
            prevEntry->next = newEntry;
        } else {
            AttrCacheTable::attrCache[RELCAT_RELID] = newEntry;  // Head of the linked list
        }

        prevEntry = newEntry;  // Update the previous entry
    }

    // Set up attributes for Attribute Catalog in the Attribute Cache Table
    prevEntry = nullptr;
    for (int i = 6; i < 12; ++i) {
        attrCatBlock.getRecord(attrCatRecordForCache, i);  // Get the record for the current slot

        AttrCacheEntry* newEntry = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
        AttrCacheTable::recordToAttrCatEntry(attrCatRecordForCache, &newEntry->attrCatEntry);

        newEntry->recId.block = ATTRCAT_BLOCK;
        newEntry->recId.slot = i;
        newEntry->next = nullptr;

        // Link the entries together
        if (prevEntry) {
            prevEntry->next = newEntry;
        } else {
            AttrCacheTable::attrCache[ATTRCAT_RELID] = newEntry;  // Head of the linked list
        }

        prevEntry = newEntry;  // Update the previous entry
    }


    // Set up Relation Catalog and Attribute Catalog entries (already done)
    // ... (other relation setup code for RELCAT and ATTRCAT)


    // Set up Relation Catalog entry for the "Students" relation
    Attribute stuCatRecord[4];  // Buffer for the "Students" relation record
    relCatBlock.getRecord(stuCatRecord, 2);  // Get the record for "Students" relation
    char srcRel[ATTR_SIZE] = "Students";  // Relation name
    int srcRelId = OpenRelTable::getRelId(srcRel);  // Get the relation ID

    // Check if the relation ID is valid
    if (srcRelId == E_RELNOTOPEN) {
        std::cerr << "Error: The relation " << srcRel << " is not open." << std::endl;
        return;
    }

    struct RelCacheEntry stuCacheEntry;
    RelCacheTable::recordToRelCatEntry(stuCatRecord, &stuCacheEntry.relCatEntry);
    stuCacheEntry.recId.block = RELCAT_BLOCK;
    stuCacheEntry.recId.slot = 2;

    // Allocate and store in relCache
    RelCacheTable::relCache[2] = (struct RelCacheEntry*)malloc(sizeof(RelCacheEntry));
    *(RelCacheTable::relCache[2]) = stuCacheEntry;

    // Set up the attribute cache for "Students" relation (slots 12-15)
    prevEntry = nullptr;
    for (int i = 12; i < 16; ++i) {
        attrCatBlock.getRecord(attrCatRecordForCache, i);  // Get the record for the current slot

        AttrCacheEntry* newEntry = (AttrCacheEntry*)malloc(sizeof(AttrCacheEntry));
        AttrCacheTable::recordToAttrCatEntry(attrCatRecordForCache, &newEntry->attrCatEntry);

        newEntry->recId.block = ATTRCAT_BLOCK;
        newEntry->recId.slot = i;
        newEntry->next = nullptr;

        // Link the entries together
        if (prevEntry) {
            prevEntry->next = newEntry;
        } else {
            AttrCacheTable::attrCache[2] = newEntry;  // Head of the linked list for "Students"
        }

        prevEntry = newEntry;  // Update the previous entry
    }

}
int OpenRelTable::getRelId( char relName[ATTR_SIZE]) {
    // if relName is RELCAT_RELNAME, return RELCAT_RELID
    if (strcmp(relName, RELCAT_RELNAME) == 0) {
        return RELCAT_RELID;
    }
    // if relName is ATTRCAT_RELNAME, return ATTRCAT_RELID
    if (strcmp(relName, ATTRCAT_RELNAME) == 0) {
        return ATTRCAT_RELID;
    }


    // Return rel-id for "Students"
    if (strcmp(relName, "Students") == 0) {
        return 2;  // "Students" relation has rel-id 2, assuming it's set up
    }

    return E_RELNOTOPEN;  // If the relation is not open, return an error code
}
OpenRelTable::~OpenRelTable() {
  // free all the memory that you allocated in the constructor
   if (RelCacheTable::relCache[RELCAT_RELID]) {
    free(RelCacheTable::relCache[RELCAT_RELID]);
  }

  // Free memory for Attribute Cache entries for RELCAT_RELID
  struct AttrCacheEntry* current = AttrCacheTable::attrCache[RELCAT_RELID];
  while (current) {
    struct AttrCacheEntry* next = current->next;
    free(current);
    current = next;
  }

  // Free memory for Attribute Cache entries for ATTRCAT_RELID
  current = AttrCacheTable::attrCache[ATTRCAT_RELID];
  while (current) {
    struct AttrCacheEntry* next = current->next;
    free(current);
    current = next;
  }
}*/

