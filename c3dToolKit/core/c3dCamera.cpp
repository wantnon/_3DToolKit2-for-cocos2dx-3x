//
//  c3dCamera.cpp
//  HelloCpp
//
//  Created by Yang Chao (wantnon) on 14-1-7.
//
//

#include "c3dCamera.h"

Cc3dVector4 Cc3dCamera::getEyePos()const{
    return m_eyePos;
}
Cc3dVector4 Cc3dCamera::getCenter()const{
    return m_center;
}
Cc3dVector4 Cc3dCamera::getUp()const{
    return m_up;
}
void Cc3dCamera::setEyePos(const Cc3dVector4&eyePos){
    m_eyePos=eyePos;
    m_isViewMatDirty=true;
    m_isViewMatInverseDirty=true;
    
}
void Cc3dCamera::setCenter(const Cc3dVector4&center){
    m_center=center;
    m_isViewMatDirty=true;
    m_isViewMatInverseDirty=true;
    
}
void Cc3dCamera::setUp(const Cc3dVector4&up){
    m_up=up;
    m_isViewMatDirty=true;
    m_isViewMatInverseDirty=true;
}

Cc3dMatrix4 Cc3dCamera::calculateViewMat(){
    //why we not just return CCCamera::m_lookupMatrix?
    //because m_lookupMatrix may be dirty (m_lookupMatrix got updated only when locate is called)
    //so we calculate view matrix ourselves.
    Cc3dMatrix4 ret;
    if(m_isViewMatDirty){//dirty
        //calculate and cache
        ret=::calculateViewMatrix(getEyePos(), getCenter(), getUp());
        m_viewMatCache=ret;
    }else{//not dirty
        //get from cache
        ret=m_viewMatCache;
    }
    return ret;
    
};
Cc3dMatrix4 Cc3dCamera::calculateViewMatInverse(){
    Cc3dMatrix4 ret;
    if(m_isViewMatInverseDirty){
        ret=::calculateViewMatrixInverse(getEyePos(), getCenter(), getUp());
        m_viewMatInverseCache=ret;
    }else{
        ret=m_viewMatInverseCache;
    }
    return ret;
}
Cc3dMatrix4 Cc3dCamera::calculateProjectionMat(){
    Cc3dMatrix4 projectionMat;
    switch (m_projectionMode) {
        case ec3dPerspectiveMode:
            projectionMat=::calculatePerspectiveProjectionMatrix(m_fovy, m_aspect, m_zNear, m_zFar);
            break;
        case ec3dOrthographicMode:
            projectionMat=::calculateOrthoProjectionMatrix(m_range.getMinX(), m_range.getMaxX(), m_range.getMinY(), m_range.getMaxY(), m_range.getMinZ(), m_range.getMaxZ());
            break;
        default:
            assert(false);
            break;
    }
    return projectionMat;
}
void Cc3dCamera::printProjectionMode()const{
    if(m_projectionMode==ec3dPerspectiveMode){
        C3DLOG("projectionMode: perspectiveMode");
    }else if(m_projectionMode==ec3dOrthographicMode){
        C3DLOG("projectionMode: orthographic");
    }else{
        assert(false);
    }
}
