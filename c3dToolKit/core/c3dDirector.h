//
//  c3dDirector.h
//  HelloWorld
//
//  Created by apple on 14/12/7.
//
//

#ifndef __HelloWorld__c3dDirector__
#define __HelloWorld__c3dDirector__

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;
#include "c3dCamera.h"
#include "c3dLightSource.h"
#include "c3dTexture.h"
class Cc3dDirector:public CCObject{
public:
    Cc3dDirector(){
        m_camera=NULL;
        m_cameraLightView=NULL;
        m_lightSource=NULL;
        m_shadowMap=NULL;
    
    }
    virtual ~Cc3dDirector(){
        if(m_camera)m_camera->release();
        if(m_cameraLightView)m_cameraLightView->release();
        if(m_lightSource)m_lightSource->release();
        if(m_shadowMap)m_shadowMap->release();
    }
    static Cc3dDirector*sharedDirector();
    
    Cc3dCamera* getCamera(){return m_camera;}
    Cc3dCamera* getCameraLightView(){return m_cameraLightView;}
    Cc3dLightSource* getLightSource(){return m_lightSource;}
    void setCamera(Cc3dCamera*camera){setRCObject(m_camera,camera);}
    void setCameraLightView(Cc3dCamera*camera){setRCObject(m_cameraLightView,camera);}
    void setLightSource(Cc3dLightSource*lightSource){setRCObject(m_lightSource,lightSource);}
    void setShadowMap(Cc3dTexture*shadowMap){m_shadowMap=shadowMap;}
    Cc3dTexture*getShadowMap(){return m_shadowMap;}
protected:
    Cc3dCamera* m_camera;
    Cc3dCamera* m_cameraLightView;
    Cc3dLightSource* m_lightSource;
    Cc3dTexture* m_shadowMap;

};
#endif /* defined(__HelloWorld__c3dDirector__) */
