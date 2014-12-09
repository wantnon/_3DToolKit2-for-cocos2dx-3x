//
//  fbo.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-6.
//
//

#ifndef __HelloOpenGL__fbo__
#define __HelloOpenGL__fbo__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "c3dTexture.h"

class Cc3dFbo:public CCObject
{
protected:
    Cc3dTexture*m_depthTex;
    Cc3dTexture*m_colorTex;
    GLuint m_fbo;
   

public:
    
    Cc3dFbo(){
        m_depthTex=NULL;
        m_colorTex=NULL;
        m_fbo=0;
        init_dft();
    }
    ~Cc3dFbo(){
        if(m_fbo!=0)glDeleteFramebuffers(1, &m_fbo);
        if(m_depthTex)m_depthTex->release();
        if(m_colorTex)m_colorTex->release();
    }
    void init_dft(){
        glGenFramebuffers(1, &m_fbo);
		assert(m_fbo>0);
    }
    Cc3dTexture* getDepthTex(){return m_depthTex;}
    Cc3dTexture* getColorTex(){return m_colorTex;}
    GLuint getID()const{return m_fbo;}
    void attachDepthTex(Cc3dTexture*depthTexture);
    void attachColorTex(Cc3dTexture*colorTexture);
 
    
};
#endif /* defined(__HelloOpenGL__fbo__) */
