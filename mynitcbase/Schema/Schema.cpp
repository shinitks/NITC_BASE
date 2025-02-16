#include "Schema.h"

#include <cmath>
#include <cstring>


int Schema::openRel(char relName[ATTR_SIZE])
{
    int ret = OpenRelTable::openRel(relName);

    // the OpenRelTable::openRel() function returns the rel-id if successful
    // a valid rel-id will be within the range 0 <= relId < MAX_OPEN and any
    // error codes will be negative
    if (ret >= 0)
    {
        return SUCCESS;
    }

    // otherwise it returns an error message
    return ret;
}

int Schema::closeRel(char relName[ATTR_SIZE])
{
    if (strcmp(relName, RELCAT_RELNAME) == 0 || strcmp(relName, ATTRCAT_RELNAME) == 0)
    {
        return E_NOTPERMITTED;
    }

    // this function returns the rel-id of a relation if it is open or
    // E_RELNOTOPEN if it is not. we will implement this later.
    int relId = OpenRelTable::getRelId(relName);

    if (relId == E_RELNOTOPEN)
    {
        return E_RELNOTOPEN;
    }

    return OpenRelTable::closeRel(relId);
}

//int Schema::renameAttr(char *relName, char *oldAttrName, char *newAttrName) {
    // if the relName is either Relation Catalog or Attribute Catalog,
    //if(strcmp(relName,RELCAT_RELNAME)==0||strcmp(relName,ATTRCAT_RELNAME)==0){
      //  return E_NOTPERMITTED;
   // }
        
        //if(OpenRelTable::getRelId(relName)==E_RELNOTOPEN){
           //  return E_RELOPEN;
        //}
    // if the relation is open
        //    (check if OpenRelTable::getRelId() returns E_RELNOTOPEN)
        //    return E_RELOPEN

    // Call BlockAccess::renameAttribute with appropriate arguments.
//return BlockAccess::renameAttribute(relName,oldAttrName,newAttrName);
//    // return the value returned by the above renameAttribute() call
//}
