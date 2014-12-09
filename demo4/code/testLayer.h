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
class CtestLayer : public CCLayer
{
public:
	CtestLayer() {

        m_r=6;
        m_A=30;
        m_B=30;
        m_eyePos.init(0, 0, m_r, 1);
        m_eyeTg.init(0, 0, 0, 1);
        
        m_mosPos=CCPoint(0,0);
        m_mosPosf=CCPoint(0,0);
        
        m_root3d=NULL;
        m_isDoUpdate=false;
        m_pLabel=NULL;
        
        m_nRow=1;
        m_nCol=1;
        m_nDeep=1;
        
    }
	virtual ~CtestLayer(){
    };
    bool init();
    void update(float dt);
    void updateEyePos();
public:
    //touch
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
public:
    //controlButton and its callback
    ControlButton* m_controlButton_clear;
    void clearCallBack(Ref *senderz, Control::EventType controlEvent);
public:
    //label show current projection mode
    CCLabelTTF* m_pLabel;
    //whether to do update
    bool m_isDoUpdate;
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
    //3d actors
    vector<vector<vector<Cc3dActor*> > > m_actor3dMat3d;
    int m_nRow;
    int m_nCol;
    int m_nDeep;
    
};

#endif /* defined(__HelloCpp___testLayer__) */
