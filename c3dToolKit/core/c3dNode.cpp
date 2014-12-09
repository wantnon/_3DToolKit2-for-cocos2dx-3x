//
//  c3dNode.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-1-4.
//
//

#include "c3dNode.h"
#include "c3dLightSource.h"
#include "c3dDirector.h"
Cc3dNode::~Cc3dNode(){
    if(m_program)m_program->release();
    
}
bool Cc3dNode::init(){
    return true;
};

Cc3dVector4 Cc3dNode::getPositionC3D()const {
    Cc3dVector4 position(m_mat.getAt(12),m_mat.getAt(13),m_mat.getAt(14),1);
    return position;
}
Cc3dVector4 Cc3dNode::getWorldPositionC3D() {
    return convertToWorldSpaceC3D(cc3dv4(0, 0, 0, 1));
}
void Cc3dNode::setPositionC3D(const Cc3dVector4&position){
    m_mat.setAt(12, position.x());
    m_mat.setAt(13, position.y());
    m_mat.setAt(14, position.z());
}
Cc3dMatrix4 Cc3dNode::nodeToWorldTransformC3D()
{
    Cc3dMatrix4 thisSMat=calculateScaleMatrix(m_kx, m_ky, m_kz);
    Cc3dMatrix4 t = this->m_mat*thisSMat;
    for (Cc3dNode *p = (Cc3dNode*)this->getParent(); p != NULL; p = (Cc3dNode*)p->getParent()){
        t = p->nodeToParentTransformC3D()*t;
        if(p->isSceneNodeC3D())break;//seneNode's matrix already count in
    }
    return t;
}
Cc3dMatrix4 Cc3dNode::worldToNodeTransformC3D(){
    Cc3dMatrix4 nodeToWorld=nodeToWorldTransformC3D();
    Cc3dMatrix4 worldToNode=inverse(nodeToWorld);
    return worldToNode;
}
Cc3dVector4 Cc3dNode::convertToWorldSpaceC3D(const Cc3dVector4&v)
{
    Cc3dMatrix4 nodeToWorldTransform=nodeToWorldTransformC3D();
    Cc3dVector4 rs=nodeToWorldTransform*v;
    return rs;
}
Cc3dVector4 Cc3dNode::convertToNodeSpaceC3D(const Cc3dVector4&v){
    Cc3dMatrix4 worldToNodeTransform=worldToNodeTransformC3D();
    Cc3dVector4 rs=worldToNodeTransform*v;
    return rs;
}

Cc3dMatrix4 Cc3dNode::nodeToParentTransformC3D(){
    Cc3dMatrix4 SMat=calculateScaleMatrix(m_kx, m_ky, m_kz);
    return m_mat*SMat;
}

void Cc3dNode::rotateC3D(const Cc3dVector4&axis,float cosA,float sinA){
    //axis must be a unit vector
    assert(axis.w()==0);
    assert(getLength2(axis)==1);
    Cc3dMatrix4 RMat=calculateRotationMatrix(axis, cosA, sinA);
    m_mat=m_mat*RMat;
};
void Cc3dNode::rotateRelativeToParentC3D(const Cc3dVector4&axis,float cosA,float sinA){
    //axis must be a unit vector
    assert(axis.w()==0);
    assert(getLength2(axis)==1);
    Cc3dMatrix4 RMat=calculateRotationMatrix(axis, cosA, sinA);
    m_mat=RMat*m_mat;
    
}
void Cc3dNode::translateC3D(const Cc3dVector4&translate){
    assert(translate.w()==0);
    Cc3dMatrix4 TMat=calculateTranslateMatrix(translate);
    m_mat=m_mat*TMat;
}
void Cc3dNode::translateRelativeToParentC3D(const Cc3dVector4&translate){
    assert(translate.w()==0);
    Cc3dMatrix4 TMat=calculateTranslateMatrix(translate);
    m_mat=TMat*m_mat;
    
}
void Cc3dNode::setRotationC3D(const Cc3dVector4&axis,float cosA,float sinA){
    assert(axis.w()==0);
    Cc3dMatrix4 RMat=calculateRotationMatrix(axis, cosA, sinA);
    //replace left up 3x3 sub matrix of m_mat with RMat
    const int indexs[9]={0,1,2,4,5,6,8,9,10};
    for(int i=0;i<9;i++){
        int index=indexs[i];
        m_mat.setAt(index, RMat.getAt(index));
    }
}
void Cc3dNode::rotateC3D(const Cc3dVector4&axis,float A){
    rotateC3D(axis, cosf(A*M_PI/180), sinf(A*M_PI/180));
}
void Cc3dNode::setRotationC3D(const Cc3dVector4&axis,float A){
    setRotationC3D(axis, cosf(A*M_PI/180), sinf(A*M_PI/180));
}
void Cc3dNode::rotateRelativeToParentC3D(const Cc3dVector4&axis,float A){
    rotateRelativeToParentC3D(axis, cosf(A*M_PI/180), sinf(A*M_PI/180));
}

Cc3dMatrix4 Cc3dNode::getRotationC3D(){
    Cc3dMatrix4 RMat=unitMat();
    //replace left up 3x3 sub matrix of RMat with m_mat
    const int indexs[9]={0,1,2,4,5,6,8,9,10};
    for(int i=0;i<9;i++){
        int index=indexs[i];
        RMat.setAt(index, m_mat.getAt(index));
    }
    return RMat;
}
void Cc3dNode::setTransformC3D(const Cc3dMatrix4&mat){
    m_mat=mat;
}
Cc3dMatrix4 Cc3dNode::getTransformC3D(){
    return m_mat;
}
void Cc3dNode::setScaleC3D(float kx,float ky,float kz){
    m_kx=kx;
    m_ky=ky;
    m_kz=kz;
}
void Cc3dNode::setProgram(Cc3dProgram*program){
    assert(program);
    setRCObject(m_program, program);
    CCNode::setShaderProgram(m_program);
    
}
void Cc3dNode::setPassUnifoCallback(c3dPassUnifoCallbackPtr passUnifoCallback){
    m_passUnifoCallback=passUnifoCallback;
}

void Cc3dNode::drawC3D(Renderer *renderer, const Cc3dMatrix4& modelMat){

}
void Cc3dNode::onDraw(const Mat4& modelMat, uint32_t flags){

}

//for 3d drawing, fake zOrder is also useful,
//because some times we need to control the drawing order, then we can use it.
void Cc3dNode::visitC3D(Renderer *renderer){
    // quick return if not visible. children won't be drawn.
    if (!_visible)
    {
        return;
    }
    
    Cc3dModelMatStack::sharedModelMatStack()->pushMatrix();
    Cc3dMatrix4 SMat=calculateScaleMatrix(m_kx, m_ky, m_kz);
    Cc3dModelMatStack::sharedModelMatStack()->mulMatrix(this->m_mat*SMat);
    
    const Cc3dMatrix4&modelMat=Cc3dModelMatStack::sharedModelMatStack()->getTopMat();
    const bool isDirectDraw=Cc3dDirector::sharedDirector()->getIsDirectDraw();
    int i = 0;
    if(!_children.empty())
    {
        sortAllChildren();
        // draw children zOrder < 0
        for( ; i < _children.size(); i++ )
        {
            Cc3dNode* node = (Cc3dNode*)_children.at(i);
            
            if ( node && node->getLocalZOrder() < 0 )
                node->visitC3D(renderer);
            else
                break;
        }
        // self draw
        if(isDirectDraw){
            this->onDraw(Mat4(modelMat.getArray()), 0);
        }else{
            this->drawC3D(renderer,modelMat);
        }
        
        for(auto it=_children.cbegin()+i; it != _children.cend(); ++it)
            ((Cc3dNode*)(*it))->visitC3D(renderer);
    }else{
        if(isDirectDraw){
            this->onDraw(Mat4(modelMat.getArray()), 0);
        }else{
            this->drawC3D(renderer, modelMat);
        }
    }
    
    Cc3dModelMatStack::sharedModelMatStack()->popMatrix();
    

}

