#include "BlockAccess.h"
#include "../Buffer/StaticBuffer.h"
#include <cstring>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdlib>

//STAGE 4

RecId BlockAccess::linearSearch(int relId, char attrName[ATTR_SIZE], union Attribute attrVal, int op) {
    // get the previous search index of the relation relId from the relation cache
    // (use RelCacheTable::getSearchIndex() function)
    RecId* prevRecId=(RecId*)malloc(sizeof(RecId));
    RelCacheTable::getSearchIndex(relId,prevRecId);
    

    // let block and slot denote the record id of the record being currently checked
    int block=-1;
    int slot=-1;

    // if the current search index record is invalid(i.e. both block and slot = -1)
    if (prevRecId->block == -1 && prevRecId->slot == -1)
    {
        // (no hits from previous search; search should start from the
        // first record itself)

        // get the first record block of the relation from the relation cache
        // (use RelCacheTable::getRelCatEntry() function of Cache Layer)
      RelCatEntry* relCatBuf=(RelCatEntry*)malloc(sizeof(RelCatEntry));
      RelCacheTable::getRelCatEntry(relId,relCatBuf);
        // block = first record block of the relation
        block=relCatBuf->firstBlk;
        // slot = 0
        slot=0;
    }
    else
    {
        // (there is a hit from previous search; search should start from
        // the record next to the search index record)

        // block = search index's block
        // slot = search index's slot + 1
        block=prevRecId->block;
        slot=prevRecId->slot+1;
    }

    /* The following code searches for the next record in the relation
       that satisfies the given condition
       We start from the record id (block, slot) and iterate over the remaining
       records of the relation
    */
    while (block != -1)
    {
        /* create a RecBuffer object for block (use RecBuffer Constructor for
           existing block) */
             RecBuffer RecBuffer(block);

          Attribute rec[RELCAT_NO_ATTRS];

        // get the record with id (block, slot) using RecBuffer::getRecord()
        RecBuffer.getRecord(rec,slot);
        // get header of the block using RecBuffer::getHeader() function
        HeadInfo header;
        RecBuffer.getHeader(&header);
        // get slot map of the block using RecBuffer::getSlotMap() function
        unsigned char slotMap[SLOTMAP_SIZE_RELCAT_ATTRCAT];
        RecBuffer.getSlotMap(slotMap);
        // If slot >= the number of slots per block(i.e. no more slots in this block)
        if(slot>=header.numSlots)
        {
            // update block = right block of block
            block=header.rblock;
            // update slot = 0
            slot=0;
            continue;  // continue to the beginning of this while loop
        }

        // if slot is free skip the loop
        // (i.e. check if slot'th entry in slot map of block contains SLOT_UNOCCUPIED)
        if(slotMap[slot]==SLOT_UNOCCUPIED)
        {
          slot++;
          if (slot >= header.numSlots) {  // ensure slot is within bounds
        block = header.rblock;  // move to the next block
        slot = 0;
    }
          continue;
            // increment slot and continue to the next record slot
        }

        // compare record's attribute value to the the given attrVal as below:
        /*
            firstly get the attribute offset for the attrName attribute
            from the attribute cache entry of the relation using
            AttrCacheTable::getAttrCatEntry()
        */
         struct AttrCatEntry * attrCatBuf= (struct AttrCatEntry *)malloc(sizeof(struct AttrCatEntry));

 AttrCacheTable::getAttrCatEntry(relId,attrName,attrCatBuf);

      int offset=attrCatBuf->offset;
        /* use the attribute offset to get the value of the attribute from
           current record */
           Attribute attr1=rec[offset];
           int attrType=attrCatBuf->attrType;
           


        int cmpVal;  // will store the difference between the attributes
        // set cmpVal using compareAttrs()
        cmpVal=RecBuffer.compareAttrs(attr1,attrVal,attrType);


        /* Next task is to check whether this record satisfies the given condition.
           It is determined based on the output of previous comparison and
           the op value received.
           The following code sets the cond variable if the condition is satisfied.
        */
        if (
            (op == NE && cmpVal != 0) ||    // if op is "not equal to"
            (op == LT && cmpVal < 0) ||     // if op is "less than"
            (op == LE && cmpVal <= 0) ||    // if op is "less than or equal to"
            (op == EQ && cmpVal == 0) ||    // if op is "equal to"
            (op == GT && cmpVal > 0) ||     // if op is "greater than"
            (op == GE && cmpVal >= 0)       // if op is "greater than or equal to"
        ) {
            /*
            set the search index in the relation cache as
            the record id of the record that satisfies the given condition
            (use RelCacheTable::setSearchIndex function)
            */
                RecId* SearchIndex=(RecId*)malloc(sizeof(RecId));
                SearchIndex->block=block;
                SearchIndex->slot=slot;

             RelCacheTable::setSearchIndex(relId,SearchIndex);

            return RecId{block, slot};
        }

        slot++;
    }

    // no record in the relation with Id relid satisfies the given condition

    return RecId{-1, -1};
}

//STAGE 5
int BlockAccess::renameRelation(char oldName[ATTR_SIZE], char newName[ATTR_SIZE]){
   
RelCacheTable::resetSearchIndex(0);
    Attribute newRelationName;    

    // search the relation catalog for an entry with "RelName" = newRelationName

    // If relation with name newName already exists (result of linearSearch
    //                                               is not {-1, -1})
    //    return E_RELEXIST;
     Attribute searchValue;
    strcpy((char*)searchValue.sVal, (char*)newName);
    RecId relcatRecId = BlockAccess::linearSearch(RELCAT_RELID, RELCAT_ATTR_RELNAME, searchValue, EQ);

if (relcatRecId.block != -1 && relcatRecId.slot != -1) {
        return E_RELEXIST;
    }

    /* reset the searchIndex of the relation catalog using
       RelCacheTable::resetSearchIndex() */
       RelCacheTable::resetSearchIndex(0);

    Attribute oldRelationName;    // set oldRelationName with oldName

    // search the relation catalog for an entry with "RelName" = oldRelationName

    // If relation with name oldName does not exist (result of linearSearch is {-1, -1})
    //    return E_RELNOTEXIST;
Attribute newsearchValue;
    strcpy((char*)newsearchValue.sVal, (char*)oldName);
     relcatRecId = BlockAccess::linearSearch(RELCAT_RELID, RELCAT_ATTR_RELNAME, newsearchValue, EQ);

if (relcatRecId.block == -1 && relcatRecId.slot == -1) {
        return E_RELEXIST;
    }
    /* get the relation catalog record of the relation to rename using a RecBuffer
       on the relation catalog [RELCAT_BLOCK] and RecBuffer.getRecord function
    */
   RecBuffer relCatBuffer(RELCAT_BLOCK);
      Attribute relCatRecord[RELCAT_NO_ATTRS];  // Buffer for Relation Catalog record

    relCatBuffer.getRecord(relCatRecord, relcatRecId.slot);
    /* update the relation name attribute in the record with newName.
       (use RELCAT_REL_NAME_INDEX) */
    // set back the record value using RecBuffer.setRecord

    /*
    update all the attribute catalog entries in the attribute catalog corresponding
    to the relation with relation name oldName to the relation name newName
    */

    /* reset the searchIndex of the attribute catalog using
       RelCacheTable::resetSearchIndex() */

    //for i = 0 to numberOfAttributes :
    //    linearSearch on the attribute catalog for relName = oldRelationName
    //    get the record using RecBuffer.getRecord
    //
    //    update the relName field in the record to newName
    //    set back the record using RecBuffer.setRecord
     HeadInfo relCatHeader;
  HeadInfo attrCatHeader;
    RecBuffer attrCatBuffer(ATTRCAT_BLOCK);

  attrCatBuffer.getHeader(&attrCatHeader);

     strcpy((char*)relCatRecord[RELCAT_REL_NAME_INDEX].sVal, (char*)newName);
    relCatBuffer.setRecord(relCatRecord, relcatRecId.slot);
relCatBuffer.getHeader(&relCatHeader);
    // Reset the search index for the attribute catalog
    RelCacheTable::resetSearchIndex(0);

    // Update all attribute catalog entries for the relation with oldName to newName
    for (int i = 0; i < relCatHeader.numEntries; ++i) {
        RecBuffer attrCatBuffer(ATTRCAT_BLOCK + i);
        for (int j = 0; j < attrCatHeader.numEntries; ++j) {
            Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
            attrCatBuffer.getRecord(attrCatRecord, j);

            // Check if the relName in the attribute catalog matches oldName
            if (strcmp((char*)attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal, (char*)oldName) == 0) {
                // Update the relName field in the record to newName
                strcpy((char*)attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal, (char*)newName);
                attrCatBuffer.setRecord(attrCatRecord, j);
            }
        }
    }


    return SUCCESS;
}


int BlockAccess::renameAttribute(char relName[ATTR_SIZE], char oldName[ATTR_SIZE], char newName[ATTR_SIZE]) {
    // Reset the search index for the relation catalog
    RelCacheTable::resetSearchIndex(0);

    // Set up the relNameAttr attribute
    Attribute relNameAttr;
    strcpy((char*)relNameAttr.sVal, (char*)relName);

    // Search for the relation in the Relation Catalog
    RecId relcatRecId = BlockAccess::linearSearch(RELCAT_RELID, RELCAT_ATTR_RELNAME, relNameAttr, EQ);

    // If the relation with relName does not exist, return E_RELNOTEXIST
    if (relcatRecId.block == -1 && relcatRecId.slot == -1) {
        return E_RELNOTEXIST;
    }

    // Reset the search index for the Attribute Catalog
    RelCacheTable::resetSearchIndex(0);

    // Declare a variable to store the record ID of the attribute to rename
    RecId attrToRenameRecId{-1, -1};
    Attribute attrCatEntryRecord[ATTRCAT_NO_ATTRS];

    // Iterate over the Attribute Catalog to find the oldName and check for newName conflicts
    while (true) {
        // Perform a linear search on the attribute catalog for relName = relNameAttr
        RecId attrcatRecId = BlockAccess::linearSearch(ATTRCAT_RELID, ATTRCAT_ATTR_RELNAME, relNameAttr, EQ);

        // If no more attributes are left to check, break out of the loop
        if (attrcatRecId.block == -1 && attrcatRecId.slot == -1) {
            break;
        }

        // Fetch the record from the Attribute Catalog
        RecBuffer attrCatBuffer(attrcatRecId.block);
        attrCatBuffer.getRecord(attrCatEntryRecord, attrcatRecId.slot);

        // Check if the attribute name matches oldName
        if (strcmp((char*)attrCatEntryRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, (char*)oldName) == 0) {
            // Set the record ID of the attribute to rename
            attrToRenameRecId = attrcatRecId;
        }

        // Check if the attribute name matches newName (to prevent conflicts)
        if (strcmp((char*)attrCatEntryRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, (char*)newName) == 0) {
            return E_ATTREXIST;  // Attribute with newName already exists
        }
    }

    // If the attribute with oldName was not found, return E_ATTRNOTEXIST
    if (attrToRenameRecId.block == -1 && attrToRenameRecId.slot == -1) {
        return E_ATTRNOTEXIST;
    }

    // Update the entry for the attribute in the Attribute Catalog
    RecBuffer attrToRenameBuffer(attrToRenameRecId.block);
    attrToRenameBuffer.getRecord(attrCatEntryRecord, attrToRenameRecId.slot);

    // Update the attribute name with newName
    strcpy((char*)attrCatEntryRecord[ATTRCAT_ATTR_NAME_INDEX].sVal, (char*)newName);

    // Save the updated record back to the Attribute Catalog
    attrToRenameBuffer.setRecord(attrCatEntryRecord, attrToRenameRecId.slot);

    return SUCCESS;  // Operation successful
}
