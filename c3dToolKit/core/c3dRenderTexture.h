//
//  c3dRenderTexture.h
//  HelloWorld
//
//  Created by apple on 14/12/6.
//
//

#ifndef __HelloWorld__c3dRenderTexture__
#define __HelloWorld__c3dRenderTexture__

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;
#include "c3dNode.h"
#include "c3dFbo.h"
class Cc3dRenderTexture:public Cc3dNode{
public:
    Cc3dRenderTexture(){
        //----
        m_fbo=NULL;
        m_oldFbo=0;
        m_oldViewport[0]=m_oldViewport[1]=m_oldViewport[2]=m_oldViewport[3]=0;
        //----
        init_dft();
    }
    virtual ~Cc3dRenderTexture() {
        if(m_fbo)m_fbo->release();
    }
    Cc3dFbo* getFBO(){return m_fbo;}
    bool init(float width,float height,bool isHaveColorTex,bool isHaveDepthTex){
        if(isHaveColorTex){
            Cc3dTexture*colorTex=new Cc3dTexture();
            colorTex->autorelease();
            colorTex->retain();
            colorTex->initColorTexture(width, height, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
            m_fbo->attachColorTex(colorTex);
        }
        if(isHaveDepthTex){
            Cc3dTexture*depthTex=new Cc3dTexture();
            depthTex->autorelease();
            depthTex->retain();
            depthTex->initDepthTexture(width,height,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR,GL_LINEAR);
            m_fbo->attachDepthTex(depthTex);
        }
        return true;
    }
    void begin();
    void end();
protected:
    void onBegin();
    void onEnd();
    void init_dft(){
        m_fbo=new Cc3dFbo();
        m_fbo->autorelease();
        m_fbo->retain();
    }

protected:
    Cc3dFbo* m_fbo;
    GLuint m_oldFbo;
    int m_oldViewport[4];
    GroupCommand _groupCommand;
    CustomCommand _beginCommand;
    CustomCommand _endCommand;
    
    
};
#endif /* defined(__HelloWorld__c3dRenderTexture__) */
