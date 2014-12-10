//
//  c3dRoot.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-1-4.
//
//

#ifndef __HelloCpp__c3dRoot__
#define __HelloCpp__c3dRoot__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;

#include "c3dCommonFunc.h"
#include "c3dMatrix.h"
#include "c3dMath.h"
#include "c3dNode.h"
#include "c3dCamera.h"

class Cc3dRoot:public Cc3dNode
{
public:
    Cc3dRoot(){
        m_isDoDepthTestOld=false;
    }
    virtual~Cc3dRoot(){
    
    }

    bool init();
    virtual void visitC3D(Renderer *renderer){assert(false);};
    virtual bool isSceneNodeC3D(){return true;}
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
protected:
    void onBegin();
    void onEnd();
    cocos2d::CustomCommand _beginCommand;
    cocos2d::CustomCommand _endCommand;
    GLboolean m_isDoDepthTestOld;

};
#endif /* defined(__HelloCpp__c3dRoot__) */
