//
//  c3dNode.h
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-1-4.
//
//

#ifndef __HelloCpp__c3dNode__
#define __HelloCpp__c3dNode__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "c3dMatrix.h"
#include "c3dMath.h"
#include "c3dVector.h"
#include "c3dCommonFunc.h"
#include "c3dGLMath.h"
#include "c3dCamera.h"
#include "c3dDefaultPassUniformCallback.h"
#include "c3dProgram.h"
#include "c3dModelMatStack.h"
class Cc3dNode;
typedef void(*c3dPassUnifoCallbackPtr)(Cc3dNode*, Cc3dProgram*, const Cc3dMatrix4&modelMat);
class Cc3dLightSource;
class Cc3dNode:public CCNode
{
public:
    Cc3dNode(){
        m_mat=unitMat();
        m_program=NULL;
        m_passUnifoCallback=NULL;
        m_kx=1;
        m_ky=1;
        m_kz=1;
 
    }
    virtual~Cc3dNode();
    static Cc3dNode*create();
    virtual bool init();
    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags){assert(false);/*we do not use it!*/}
    virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags){assert(false);/*we do not use it!*/}
    virtual void visitC3D(Renderer *renderer);
    virtual void drawC3D(Renderer *renderer, const Cc3dMatrix4& modelMat);
    virtual void onDraw(const Mat4& modelMat, uint32_t flags);
    virtual bool isSceneNodeC3D(){return false;}
    //transform
    virtual void setTransformC3D(const Cc3dMatrix4&mat);
    virtual Cc3dMatrix4 getTransformC3D();
    virtual Cc3dMatrix4 nodeToWorldTransformC3D();
    virtual Cc3dMatrix4 worldToNodeTransformC3D();
    virtual Cc3dMatrix4 nodeToParentTransformC3D();
    virtual Cc3dVector4 convertToWorldSpaceC3D(const Cc3dVector4&v);
    virtual Cc3dVector4 convertToNodeSpaceC3D(const Cc3dVector4&v);
    //translate
    virtual void setPositionC3D(const Cc3dVector4&position);
    virtual Cc3dVector4 getPositionC3D()const;
    virtual Cc3dVector4 getWorldPositionC3D();
    virtual void translateC3D(const Cc3dVector4&translate);
    virtual void translateRelativeToParentC3D(const Cc3dVector4&translate);
    //rotate
    virtual void setRotationC3D(const Cc3dVector4&axis,float cosA,float sinA);
    virtual void setRotationC3D(const Cc3dVector4&axis,float A);
    virtual Cc3dMatrix4 getRotationC3D();
    virtual void rotateC3D(const Cc3dVector4&axis,float cosA,float sinA);
    virtual void rotateC3D(const Cc3dVector4&axis,float A);
    virtual void rotateRelativeToParentC3D(const Cc3dVector4&axis,float cosA,float sinA);
    virtual void rotateRelativeToParentC3D(const Cc3dVector4&axis,float A);
    //scale
    virtual void setScaleC3D(float kx,float ky,float kz);
    //light, passUnifoCallback, program
    void setPassUnifoCallback(c3dPassUnifoCallbackPtr passUnifoCallback);
    c3dPassUnifoCallbackPtr getPassUnifoCallback(){return m_passUnifoCallback; }
    void setProgram(Cc3dProgram*program);
    Cc3dProgram* getProgram(){return m_program;}

protected:
    float m_kx,m_ky,m_kz;
    Cc3dMatrix4 m_mat;//node to parent transform matrix
    Cc3dProgram* m_program;
    c3dPassUnifoCallbackPtr m_passUnifoCallback;
    
    cocos2d::CustomCommand _customCommand;
    
};
#endif /* defined(__HelloCpp__c3dNode__) */
