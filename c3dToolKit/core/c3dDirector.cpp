//
//  c3dDirector.cpp
//  HelloWorld
//
//  Created by apple on 14/12/7.
//
//

#include "c3dDirector.h"
static Cc3dDirector*s_director=NULL;
Cc3dDirector* Cc3dDirector::sharedDirector(){
    if(s_director==NULL){
        s_director=new Cc3dDirector();
    }
    return s_director;
}