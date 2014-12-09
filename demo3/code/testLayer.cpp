//
//  testLayer.cpp
//  HelloCpp
//
//  Created by Yang Chao (wantnon) on 13-11-6.
//
//

#include "testLayer.h"

bool CtestLayer::init(){
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    float ZEye=CCDirector::sharedDirector()->getZEye();
    //enable touch
	setTouchEnabled( true );
    //enable update
    scheduleUpdate();

    //-----------------------------

    //camera
    Cc3dCamera*camera=new Cc3dCamera();
    camera->autorelease();
    camera->init();
    m_r=(winSize.height/2)/tanf(camera->getFovy()/2*M_PI/180);
    camera->setEyePos(cc3dv4(0, 500, m_r, 1));
    camera->setCenter(cc3dv4(0, 0, 0, 1));
    camera->setUp(cc3dv4(0, 1, 0, 0));
    camera->setProjectionMode(ec3dPerspectiveMode);
    
    //lightSource
    Cc3dLightSource*lightSource=new Cc3dLightSource();
    lightSource->autorelease();
    lightSource->init();
    lightSource->setAmbient(cc3dv4(0.8, 0.8, 0.8, 1));
    lightSource->setPositionC3D(cc3dv4(500, 1000, 0, 1));//note: (0,X,0,1) is illegal!!!!
    
    //cameraLightView
    Cc3dCamera*cameraLightView=new Cc3dCamera();
    cameraLightView->autorelease();
    cameraLightView->init();
    cameraLightView->setEyePos(lightSource->getPositionC3D());
    cameraLightView->setCenter(Cc3dVector4(0,0,0,1));//(cc3dv4(0,0,0,1));
    cameraLightView->setUp(cc3dv4(0, 1, 0, 0));
    cameraLightView->setFovy(30);
    cameraLightView->setProjectionMode(ec3dPerspectiveMode);
    
    
    

    
    
    //director
    Cc3dDirector::sharedDirector()->setCamera(camera);
    Cc3dDirector::sharedDirector()->setLightSource(lightSource);
    Cc3dDirector::sharedDirector()->setCameraLightView(cameraLightView);
    
    //root3d
    m_root3d=new Cc3dRoot();
    m_root3d->autorelease();
    m_root3d->init();
    this->addChild(m_root3d);
    
    

    
    
    
    //actor3D
    {
        
        //material
        Cc3dMaterial*material=new Cc3dMaterial();
        material->autorelease();
        material->init();
        material->setSpecular(cc3dv4(0.5, 0.5, 0.5, 1));
        material->setShininess(10);
        
        //texture
        Cc3dTexture*tex=Cc3dTextureCache::sharedTextureCache()->addImage("white.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
        
        Cc3dMesh*mesh_planeXZ=c3dCreatePlaneXZMesh(500,500,5,5);
        mesh_planeXZ->setTexture(tex);
        mesh_planeXZ->setMaterial(material);
        
        Cc3dActor* actor3D=new Cc3dActor();
        actor3D->autorelease();
        actor3D->init();
        actor3D->setPositionC3D(Cc3dVector4(0,0,0,1));
        actor3D->addMesh(mesh_planeXZ);
        actor3D->setPassUnifoCallback(passUnifoCallback_texOnlyAndShadow );
        actor3D->setProgram(c3dGetProgram_c3dTexOnlyAndShadow());
        m_root3d->addChild(actor3D);
        m_actor3D=actor3D;
        
        
        //submit
        actor3D->submit(GL_STATIC_DRAW);
    }
    
    
    

    //actor3D2
    {
        
        
        //material
        Cc3dMaterial*material=new Cc3dMaterial();
        material->autorelease();
        material->init();
        material->setSpecular(cc3dv4(0.5, 0.5, 0.5, 1));
        material->setShininess(10);
        
        //texture
        Cc3dTexture*tex_logo=Cc3dTextureCache::sharedTextureCache()->addImage("logo.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
        
        Cc3dMesh*mesh_box=c3dCreateBoxMesh(60,cc3dv4(1, 1, 1, 1),cc3dv4(1, 1, 1, 1),cc3dv4(1, 1, 1, 1),cc3dv4(1, 1, 1, 1),
                                           cc3dv4(1, 1, 1, 1),cc3dv4(1, 1, 1, 1),cc3dv4(1, 1, 1, 1),cc3dv4(1, 1, 1, 1));
        mesh_box->setTexture(tex_logo);
        mesh_box->setMaterial(material);
        
        Cc3dActor* actor3D=new Cc3dActor();
        actor3D->autorelease();
        actor3D->init();
        actor3D->setPositionC3D(Cc3dVector4(0,60,0,1));
        actor3D->addMesh(mesh_box);
        actor3D->setPassUnifoCallback(passUnifoCallback_classicLighting);
        actor3D->setProgram(c3dGetProgram_c3dClassicLighting());
        m_root3d->addChild(actor3D);
        m_actor3D2=actor3D;
        
        
        //submit
        actor3D->submit(GL_STATIC_DRAW);

    
    }
    
    
    //m_renderTex
  
    Cc3dDirector::sharedDirector()->setIsDirectDraw(true);
    m_renderTex=new Cc3dRenderTexture();
    m_renderTex->autorelease();
    m_renderTex->init(512, 512, false, true);
    m_root3d->addChild(m_renderTex);
    
    Cc3dDirector::sharedDirector()->setShadowMap(m_renderTex->getFBO()->getDepthTex());

    //controlButton_swithProjMode
    {
        Scale9Sprite* btnUp=Scale9Sprite::create("button.png");
        Scale9Sprite* btnDn=Scale9Sprite::create("button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("proj mode", "Helvetica", 30);
        ControlButton* controlButton=ControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,Control::State::HIGH_LIGHTED);
        controlButton->setPreferredSize(CCSize(180,80));
        controlButton->setPosition(ccp(winSize.width/2,100));
        controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CtestLayer::switchProjModeCallBack), Control::EventType::TOUCH_UP_INSIDE);
        this->addChild(controlButton);
        m_controlButton_swithProjMode=controlButton;
        
    }
    //projection mode label
    m_pLabel=CCLabelTTF::create("proj mode: Perspective", "Arial", 35);
    m_pLabel->setPosition(ccp(origin.x + visibleSize.width*(3.0/4),
                              origin.y + visibleSize.height - m_pLabel->getContentSize().height-100));
    this->addChild(m_pLabel, 1);
    
    
    return true;
}

void CtestLayer::switchProjModeCallBack(Ref *senderz, cocos2d::extension::Control::EventType controlEvent){
    Cc3dCamera*camera=Cc3dDirector::sharedDirector()->getCamera();
    switch(camera->getProjectionMode())
    {
        case ec3dPerspectiveMode:{
            camera->setProjectionMode(ec3dOrthographicMode);
            m_pLabel->setString("proj mode: Orthographic");
        }break;
        case ec3dOrthographicMode:{
            camera->setProjectionMode(ec3dPerspectiveMode);
            m_pLabel->setString("proj mode: Perspective");
        }break;
            
    }
}
void CtestLayer::update(float dt){
    bool oldIsDirectDraw=Cc3dDirector::sharedDirector()->getIsDirectDraw();
    
    m_renderTex->begin();
    {
        Cc3dCamera*oldCamera=Cc3dDirector::sharedDirector()->getCamera();
        oldCamera->retain();
        oldCamera->autorelease();
        Cc3dDirector::sharedDirector()->setCamera(Cc3dDirector::sharedDirector()->getCameraLightView());
        Cc3dProgram*oldProgramOfActor3D=m_actor3D->getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getProgram();
        oldProgramOfActor3D->retain();
        oldProgramOfActor3D->autorelease();
        c3dPassUnifoCallbackPtr oldPassUnifoCallbackPtrOfActor3D=m_actor3D->getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getPassUnifoCallback();
        m_actor3D->setProgram(c3dGetProgram_c3dRenderDepth());
        m_actor3D->setPassUnifoCallback(passUnifoCallback_renderDepth);
        Cc3dProgram*oldProgramOfActor3D2=m_actor3D2->getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getProgram();
        oldProgramOfActor3D2->retain();
        oldProgramOfActor3D2->autorelease();
        c3dPassUnifoCallbackPtr oldPassUnifoCallbackPtrOfActor3D2=m_actor3D2->getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->getPassUnifoCallback();
        m_actor3D2->setProgram(c3dGetProgram_c3dRenderDepth());
        m_actor3D2->setPassUnifoCallback(passUnifoCallback_renderDepth);
        
        m_root3d->cocos2d::Node::visit();
        
        Cc3dDirector::sharedDirector()->setCamera(oldCamera);
        m_actor3D->setProgram(oldProgramOfActor3D);
        m_actor3D->setPassUnifoCallback(oldPassUnifoCallbackPtrOfActor3D);
        m_actor3D2->setProgram(oldProgramOfActor3D2);
        m_actor3D2->setPassUnifoCallback(oldPassUnifoCallbackPtrOfActor3D2);
    }
    m_renderTex->end();
    Cc3dDirector::sharedDirector()->setIsDirectDraw(oldIsDirectDraw);
    
    //debug
    //m_actor3D2->getModel()->getMeshByIndex(0)->getSubMeshByIndex(0)->setTexture(m_renderTex->getFBO()->getDepthTex());
    // m_actor3D2->setPassUnifoCallback(passUnifoCallback_showDepth);
    // m_actor3D2->setProgram(c3dGetProgram_c3dShowDepth());
    
    

}

void CtestLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCTouch* touch;
    
    for(auto  it = touches.begin(); it != touches.end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint pointInWinSpace = touch->getLocationInView();
        //note: for 3d mode, CCDirector::convertToGL() not works as we expected
        //  CCPoint pointInWinSpace = CCDirector::sharedDirector()->convertToGL(pointInWinSpace);
        
        //----update mos
        m_mosPosf=m_mosPos;
        m_mosPos=pointInWinSpace;
        
        break;
        
    }
    
    
    
}

void CtestLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCTouch* touch;
    for(auto it = touches.begin(); it != touches.end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        CCPoint loc_winSpace = touch->getLocationInView();
        CCPoint loc_GLSpace = CCDirector::sharedDirector()->convertToGL(loc_winSpace);
        
        
        CCPoint pointInWinSpace = touch->getLocationInView();
        
        //----update mos
        m_mosPosf=m_mosPos;
        m_mosPos=pointInWinSpace;
        //----update eyePos
        m_A+=-(m_mosPos.x-m_mosPosf.x)*0.4;
        m_B+=(m_mosPos.y-m_mosPosf.y)*0.4;
        
  
        m_actor3D2->setRotationC3D(Cc3dVector4(0,1,0,0), -m_A);
        
        break;
        
    }
    
}

void CtestLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCTouch* touch;
    for( auto it = touches.begin(); it != touches.end(); it++)
    {
        touch = (CCTouch*)(*it);
        if(!touch)
            break;
        
        CCPoint pointInWinSpace = touch->getLocationInView();
        
        //----update mos
        m_mosPosf=m_mosPos;
        m_mosPos=pointInWinSpace;
        break;
        
    }
}




