#include "Schema.h"
#include <cmath>
#include <iostream>
#include <cstring>



int Schema::openRel(char relName[ATTR_SIZE]) {
  int ret = OpenRelTable::openRel(relName);

  
  if(ret >= 0){
    return SUCCESS;
  }

  return ret;
}

int Schema::closeRel(char relName[ATTR_SIZE])
{
	if (strcmp(relName, RELCAT_RELNAME) == 0 || strcmp(relName, ATTRCAT_RELNAME) == 0)
		return E_NOTPERMITTED;


	int relId = OpenRelTable::getRelId(relName);

	if (relId == E_RELNOTOPEN)
		return E_RELNOTOPEN;

	return OpenRelTable::closeRel(relId);
}

  
int Schema::renameRel(char oldRelName[ATTR_SIZE], char newRelName[ATTR_SIZE]) {
   
    if (strcmp(oldRelName, RELCAT_RELNAME) == 0 || strcmp(oldRelName, ATTRCAT_RELNAME) == 0)
		return E_NOTPERMITTED;

    if (strcmp(newRelName, RELCAT_RELNAME) == 0 || strcmp(newRelName, ATTRCAT_RELNAME) == 0)
		return E_NOTPERMITTED;

    int relId=OpenRelTable::getRelId(oldRelName);
    if(relId!=E_RELNOTOPEN)return E_RELOPEN;
    
    return BlockAccess::renameRelation(oldRelName,newRelName);
}

int Schema::renameAttr(char *relName, char *oldAttrName, char *newAttrName) {
    
    if(strcmp(relName,RELCAT_RELNAME)==0 || strcmp(relName,ATTRCAT_RELNAME)==0)return E_NOTPERMITTED;

    int relId=OpenRelTable::getRelId(relName);
    if(relId!=E_RELNOTOPEN)return E_RELOPEN;
    

    return BlockAccess::renameAttribute(relName,oldAttrName,newAttrName);

}



int Schema::createRel(char relName[],int nAttrs, char attrs[][ATTR_SIZE],int attrtype[]){

    // declare variable relNameAsAttribute of type Attribute
    Attribute relNameAsAttribute;
    // copy the relName into relNameAsAttribute.sVal
    strcpy(relNameAsAttribute.sVal,relName);

    // declare a variable targetRelId of type RecId
    RecId targetRelId;
    // Reset the searchIndex using RelCacheTable::resetSearhIndex()
    RelCacheTable::resetSearchIndex(0);

    // Search the relation catalog (relId given by the constant RELCAT_RELID)
    // for attribute value attribute "RelName" = relNameAsAttribute using
    // BlockAccess::linearSearch() with OP = EQ

    // if a relation with name `relName` already exists  ( linearSearch() does
    //                                                     not return {-1,-1} )
    //     return E_RELEXIST;
     RecId relcatRecId = BlockAccess::linearSearch(RELCAT_RELID, RELCAT_ATTR_RELNAME, relNameAsAttribute, EQ);


if (relcatRecId.block != -1 && relcatRecId.slot != -1) {
        return E_RELEXIST;
    }

    // compare every pair of attributes of attrNames[] array
    // if any attribute names have same string value,
    //     return E_DUPLICATEATTR (i.e 2 attributes have same value)
  for(int i=0;i<nAttrs;i++){
      for(int j=i+1;j<nAttrs;j++){
        if(strcmp(attrs[i],attrs[j])==0)return E_DUPLICATEATTR;

      }
    }

    /* declare relCatRecord of type Attribute which will be used to store the
       record corresponding to the new relation which will be inserted
       into relation catalog */
    Attribute relCatRecord[RELCAT_NO_ATTRS];
    // fill relCatRecord fields as given below
    // offset RELCAT_REL_NAME_INDEX: relName
    // offset RELCAT_NO_ATTRIBUTES_INDEX: numOfAttributes
    // offset RELCAT_NO_RECORDS_INDEX: 0
    // offset RELCAT_FIRST_BLOCK_INDEX: -1
    // offset RELCAT_LAST_BLOCK_INDEX: -1
    // offset RELCAT_NO_SLOTS_PER_BLOCK_INDEX: floor((2016 / (16 * nAttrs + 1)))
    // (number of slots is calculated as specified in the physical layer docs)
 strcpy(relCatRecord[RELCAT_REL_NAME_INDEX].sVal,relName);
    relCatRecord[RELCAT_NO_ATTRIBUTES_INDEX].nVal=nAttrs;
    relCatRecord[RELCAT_NO_RECORDS_INDEX].nVal=0;
    relCatRecord[RELCAT_FIRST_BLOCK_INDEX].nVal=-1;
    relCatRecord[RELCAT_LAST_BLOCK_INDEX].nVal=-1;
    int numofslots=floor(2016*1.00/(16*nAttrs+1));
    relCatRecord[RELCAT_NO_SLOTS_PER_BLOCK_INDEX].nVal=numofslots;
    // retVal = BlockAccess::insert(RELCAT_RELID(=0), relCatRecord);
    // if BlockAccess::insert fails return retVal
    // (this call could fail if there is no more space in the relation catalog)
int retVal=BlockAccess::insert(RELCAT_RELID,relCatRecord);
    if(retVal!=SUCCESS)return retVal;
    // iterate through 0 to numOfAttributes - 1 :
    {
        /* declare Attribute attrCatRecord[6] to store the attribute catalog
           record corresponding to i'th attribute of the argument passed*/
        // (where i is the iterator of the loop)
        // fill attrCatRecord fields as given below
        // offset ATTRCAT_REL_NAME_INDEX: relName
        // offset ATTRCAT_ATTR_NAME_INDEX: attrNames[i]
        // offset ATTRCAT_ATTR_TYPE_INDEX: attrTypes[i]
        // offset ATTRCAT_PRIMARY_FLAG_INDEX: -1
        // offset ATTRCAT_ROOT_BLOCK_INDEX: -1
        // offset ATTRCAT_OFFSET_INDEX: i

        // retVal = BlockAccess::insert(ATTRCAT_RELID(=1), attrCatRecord);
        /* if attribute catalog insert fails:
             delete the relation by calling deleteRel(targetrel) of schema layer
             return E_DISKFULL
             // (this is necessary because we had already created the
             //  relation catalog entry which needs to be removed)
        */
    }
     for(int attrIndex=0;attrIndex<nAttrs;attrIndex++)
    {
      Attribute attrCatRecord[ATTRCAT_NO_ATTRS];
        
        strcpy(attrCatRecord[ATTRCAT_REL_NAME_INDEX].sVal,relName);
        strcpy(attrCatRecord[ATTRCAT_ATTR_NAME_INDEX].sVal,attrs[attrIndex]);
        attrCatRecord[ATTRCAT_ATTR_TYPE_INDEX].nVal=attrtype[attrIndex];
        attrCatRecord[ATTRCAT_PRIMARY_FLAG_INDEX].nVal=-1;
        attrCatRecord[ATTRCAT_ROOT_BLOCK_INDEX].nVal=-1;
        attrCatRecord[ATTRCAT_OFFSET_INDEX].nVal=attrIndex;
        retVal=BlockAccess::insert(ATTRCAT_RELID,attrCatRecord);
        if(retVal!=SUCCESS){
          Schema::deleteRel(relName);
          return E_DISKFULL;
        }
       
       
    }

     return SUCCESS;
}

int Schema::deleteRel(char *relName) {
    
    if (strcmp(relName, RELCAT_RELNAME) == 0 || strcmp(relName, ATTRCAT_RELNAME) == 0)
		return E_NOTPERMITTED;
        
    int relId=OpenRelTable::getRelId(relName);
    if(relId>0 and relId<MAX_OPEN)return E_RELOPEN;

    int retVal=BlockAccess::deleteRelation(relName);
    return retVal;
    
}