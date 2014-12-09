//
//  c3dRenderTexture.cpp
//  HelloWorld
//
//  Created by apple on 14/12/6.
//
//

#include "c3dRenderTexture.h"
#include "c3dDirector.h"
void Cc3dRenderTexture::onBegin(){
    GLint oldFrameBuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFrameBuffer);
    m_oldFbo=oldFrameBuffer;
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo->getID());
    //tile-base architecture require clear immediately after bind frame buffer
    if(m_fbo->getDepthTex()&&!m_fbo->getColorTex()){
        //we should ensoure depthMask is true before call glClear(GL_DEPTH_BUFFER_BIT...)
        //see: http://www.flipcode.com/forums/thread/9101
        //http://stackoverflow.com/questions/12475369/does-glcolormask-affect-glclear
        //http://www.opengl.org/sdk/docs/man2/xhtml/glClear.xml
        glDepthMask(GL_TRUE);
        glClear(GL_DEPTH_BUFFER_BIT);
    }else if(!m_fbo->getDepthTex()&&m_fbo->getColorTex()){
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT);
    }else if(m_fbo->getDepthTex()&&m_fbo->getColorTex()){
        glDepthMask(GL_TRUE);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    }else{//!m_depthTex&&!m_colorTex
        assert(false);
    }
    //set viewport
    glGetIntegerv(GL_VIEWPORT, m_oldViewport);
    if(m_fbo->getDepthTex()&&m_fbo->getColorTex()){
        assert(m_fbo->getDepthTex()->getWidth()==m_fbo->getColorTex()->getWidth());
        assert(m_fbo->getDepthTex()->getHeight()==m_fbo->getColorTex()->getHeight());
    }
    
    glViewport(0, 0, m_fbo->getDepthTex()->getWidth() ,m_fbo->getDepthTex()->getHeight());
    
}
void Cc3dRenderTexture::onEnd(){
    glBindFramebuffer(GL_FRAMEBUFFER, m_oldFbo);
    glViewport(m_oldViewport[0], m_oldViewport[1], m_oldViewport[2], m_oldViewport[3]);
    
}
void Cc3dRenderTexture::begin(){
    if(Cc3dDirector::sharedDirector()->getIsDirectDraw()){
        onBegin();
    }else{
        Director* director = Director::getInstance();
        
        _groupCommand.init(_globalZOrder);
        
        Renderer *renderer =  Director::getInstance()->getRenderer();
        renderer->addCommand(&_groupCommand);
        renderer->pushGroup(_groupCommand.getRenderQueueID());
        
        _beginCommand.init(_globalZOrder);
        _beginCommand.func = CC_CALLBACK_0(Cc3dRenderTexture::onBegin, this);
        
        Director::getInstance()->getRenderer()->addCommand(&_beginCommand);
    
    }
    
    
}
void Cc3dRenderTexture::end(){
    if(Cc3dDirector::sharedDirector()->getIsDirectDraw()){
        onEnd();
    }else{
        _endCommand.init(_globalZOrder);
        _endCommand.func = CC_CALLBACK_0(Cc3dRenderTexture::onEnd, this);
        
        Director* director = Director::getInstance();
        
        Renderer *renderer = director->getRenderer();
        renderer->addCommand(&_endCommand);
        renderer->popGroup();
    
    }
    
    
}