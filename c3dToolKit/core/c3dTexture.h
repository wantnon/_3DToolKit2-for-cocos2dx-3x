//
//  c3dTexture.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 14-2-20.
//
//

#ifndef __HelloOpenGL__c3dTexture__
#define __HelloOpenGL__c3dTexture__

#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "cocos2d.h"
using namespace cocos2d;

class Cc3dTexture:public CCObject
{
protected:
    GLuint m_texture;
    float m_width,m_height;
    string m_filePath;//for the textures not load from file (created by program), m_filePath is ""
    bool m_isCubeTexture;
protected:
    CCTexture2D*m_cctex2d;
public:
    Cc3dTexture(){
        m_texture=0;
        m_width=m_height=0;
        m_isCubeTexture=false;
        m_cctex2d=NULL;
    }
    ~Cc3dTexture(){
        if(m_cctex2d){
            m_cctex2d->release();
        }
        if(m_texture!=0
           &&m_texture!=m_cctex2d->getName()//m_texture is not ref m_cctex2d
           ){
            glDeleteTextures(1, &m_texture);
        }
    }
    bool getIsInvalid()const {
        return m_texture==0;
    }
    
    GLuint getID()const {
        return m_texture;
    }
    float getWidth()const {
        return m_width;
    }
    float getHeight()const {
        return m_height;
    }
    string getFilePath()const{
        return m_filePath;
    }
    bool init( const string&filePath,int wrapS=GL_REPEAT,int wrapT=GL_REPEAT,GLint minFilter=GL_LINEAR,GLint magFilter=GL_LINEAR);
    bool initDepthTexture(int width,int height,int wrapS,int wrapT,GLint minFilter,GLint magFilter);
    bool initColorTexture(int width,int height,int wrapS,int wrapT,GLint minFilter,GLint magFilter);
};


#endif /* defined(__HelloOpenGL__c3dTexture__) */
