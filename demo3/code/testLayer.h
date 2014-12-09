//
//  testLayer.h
//  HelloCpp
//
//  Created by Yang Chao (wantnon) on 13-11-6.
//
//

#ifndef __HelloCpp___testLayer__
#define __HelloCpp___testLayer__

#include <iostream>
using namespace std;
#include "cocos2d.h"
using namespace cocos2d;
#include "cocos-ext.h"
using namespace cocos2d::extension;
#include "c3dToolKit.h"
class CspriteWithC3DTexture:public CCSprite{
public:
    CspriteWithC3DTexture(){
        m_c3dTextrue=NULL;
    }
    virtual~CspriteWithC3DTexture(){
        if(m_c3dTextrue)m_c3dTextrue->release();
    }
    virtual bool init(Cc3dTexture*c3dTexture){
        this->CCSprite::init();
        this->setC3DTexture(c3dTexture);
        this->setTextureRect(CCRect(0,0,c3dTexture->getWidth(),c3dTexture->getHeight()));
        return true;
    }
    
    
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
    {
        // Don't do calculate the culling if the transform was not updated
        _insideBounds = (flags & FLAGS_TRANSFORM_DIRTY) ? renderer->checkVisibility(transform, _contentSize) : _insideBounds;
        
        if(_insideBounds)
        {
            _quadCommand.init(_globalZOrder, m_c3dTextrue->getID(), getGLProgramState(), _blendFunc, &_quad, 1, transform);
            renderer->addCommand(&_quadCommand);
        }
    }
    void setC3DTexture(Cc3dTexture*c3dTexture){
        assert(c3dTexture);
        setRCObject(m_c3dTextrue, c3dTexture);
    }
protected:
    Cc3dTexture*m_c3dTextrue;

};
class CtestLayer : public CCLayer
{
public:
	CtestLayer() {

        m_r=6;
        m_A=0;
        m_B=0;
        m_eyePos.init(0, 0, m_r, 1);
        m_eyeTg.init(0, 0, 0, 1);
        
        m_mosPos=CCPoint(0,0);
        m_mosPosf=CCPoint(0,0);
        
        m_root3d=NULL;
        m_renderTex=NULL;
        
    }
	virtual ~CtestLayer(){
    };
    bool init();
    void update(float dt);
public:
    //touch
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
public:
    //controlButton and its callback
    ControlButton* m_controlButton_swithProjMode;
    void switchProjModeCallBack(Ref *senderz, Control::EventType controlEvent);
public:
    //label show current projection mode
    CCLabelTTF* m_pLabel;

    //mos
    CCPoint m_mosPos;//trace mouse position
    CCPoint m_mosPosf;//mouse position of previous frame
    //camera
    Cc3dVector4 m_eyePos;
    Cc3dVector4 m_eyeTg;
    float m_r;//distance from eyePos to eyeTg
    float m_A;//angle in XZ plane
    float m_B;//angle vertical
    //3d root
    Cc3dRoot*m_root3d;//3D root node
    //3d actor
    Cc3dActor* m_actor3D;
    Cc3dActor* m_actor3D2;
    
    Cc3dRenderTexture* m_renderTex;
    

    
    
};

#endif /* defined(__HelloCpp___testLayer__) */
