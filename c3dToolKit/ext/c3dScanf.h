//
//  c3dScanf.h
//  demo5
//
//  Created by 杨超 on 15/1/7.
//
//

#ifndef __demo5__c3dScanf__
#define __demo5__c3dScanf__

#include <stdio.h>
#include <iostream>
#include <assert.h>
using namespace std;
#include "c3dVector.h"

static char tCharBuffer[1024]={0};
static void fskipOneStr(FILE * fp,bool valueOnly){
    assert(fp);
    if(valueOnly==false){
        fscanf(fp,"%s",tCharBuffer);
    }
}
static void fprintKey(FILE * fp,bool valueOnly,const string&key){
    assert(fp);
    if(valueOnly==false){
        fprintf(fp,"%s",key.c_str());
    }
}
static Cc3dVector4 fscanVector4(FILE * fp){
    assert(fp);
    float arr[4];
    for(int i=0;i<4;i++){
        fscanf(fp,"%f",&arr[i]);
    }
    return Cc3dVector4(arr);
}
#endif /* defined(__demo5__c3dScanf__) */
