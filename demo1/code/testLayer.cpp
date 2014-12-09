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
    //enable touch
	setTouchEnabled( true );
    //enable update
    scheduleUpdate();
    //update eye pos
    updateEyePos();
    //-----------------------------
    //root3d
    m_root3d=new Cc3dRoot();
    m_root3d->autorelease();
    m_root3d->init();
    this->addChild(m_root3d);
    
    //camera
    Cc3dCamera*camera=new Cc3dCamera();
    camera->autorelease();
    camera->init();
    m_r=(winSize.height/2)/tanf(camera->getFovy()/2*M_PI/180);
    camera->setEyePos(cc3dv4(0, 0, m_r, 1));
    camera->setCenter(cc3dv4(0, 0, 0, 1));
    camera->setUp(cc3dv4(0, 1, 0, 0));
    camera->setProjectionMode(ec3dPerspectiveMode);
    //lightSource
    Cc3dLightSource*lightSource=new Cc3dLightSource();
    lightSource->autorelease();
    lightSource->init();
    m_root3d->addChild(lightSource);
    lightSource->setAmbient(cc3dv4(0.8, 0.8, 0.8, 1));
    lightSource->setPositionC3D(cc3dv4(600, 900, 1200, 1));
    
    //director
    Cc3dDirector::sharedDirector()->setCamera(camera);
    Cc3dDirector::sharedDirector()->setLightSource(lightSource);
    
    
    
    //program
    Cc3dProgram*program=c3dGetProgram_c3dClassicLighting();
    //material
    Cc3dMaterial*material=new Cc3dMaterial();
    material->autorelease();
    material->init();
    material->setSpecular(cc3dv4(0.5, 0.5, 0.5, 1));
    material->setShininess(10);
    //texture
    Cc3dTexture*tex_logo=Cc3dTextureCache::sharedTextureCache()->addImage("logo.png");
    Cc3dTexture*tex_logo2=Cc3dTextureCache::sharedTextureCache()->addImage("logo2.png");

    //mesh
    Cc3dMesh*mesh_ball=c3dCreateBallMesh(100, 20, 10, cc3dv4(1, 0.5, 0, 1), cc3dv4(0, 1, 0, 1));
    mesh_ball->setTexture(tex_logo2);
    mesh_ball->setMaterial(material);
    Cc3dMesh*mesh_box=c3dCreateBoxMesh(100,cc3dv4(1, 0, 0, 1),cc3dv4(0, 1, 0, 1),cc3dv4(0, 0, 1, 1),cc3dv4(1, 1, 0, 1),
                                         cc3dv4(0, 0, 1, 1),cc3dv4(1, 1, 0, 1),cc3dv4(1, 0, 0, 1),cc3dv4(0, 1, 0, 1));
    mesh_box->setTexture(tex_logo);
    mesh_box->setMaterial(material);
    Cc3dMesh*mesh_cone=c3dCreateConeMesh(100, 270, 20, 10, true, cc3dv4(1, 0, 0, 1), cc3dv4(0, 1, 0, 1));
    mesh_cone->setTexture(tex_logo);
    mesh_cone->setMaterial(material);

  
    //actors
    const int actorCount=3;
    vector<Cc3dMesh*> meshList;
    meshList.push_back(mesh_box);
    meshList.push_back(mesh_cone);
    meshList.push_back(mesh_ball);
    vector<Cc3dVector4> posList;
    posList.push_back(cc3dv4(-250, 0, 0, 1));
    posList.push_back(cc3dv4(0, -100, 0, 1));
    posList.push_back(cc3dv4(250, 0, 0, 1));
    for(int i=0;i<actorCount;i++){
        Cc3dActor* actor3D=new Cc3dActor();
        actor3D->autorelease();
        actor3D->init();
        actor3D->setPositionC3D(posList[i]);
        actor3D->addMesh(meshList[i]);
        actor3D->setPassUnifoCallback(passUnifoCallback_classicLighting);
        actor3D->setProgram(program);
        m_root3d->addChild(actor3D,0);
        m_actor3dList.push_back(actor3D);
    }
    //submit actors
    for(int i=0;i<(int)m_actor3dList.size();i++){
        Cc3dActor*actor3D=m_actor3dList[i];
        actor3D->submit(GL_STATIC_DRAW);
    }

    

    //controlButton_swithProjMode
    {
        Scale9Sprite* btnUp=Scale9Sprite::create("button.png");
        Scale9Sprite* btnDn=Scale9Sprite::create("button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("proj mode", "Helvetica", 30);
        ControlButton* controlButton=ControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,Control::State::HIGH_LIGHTED);
        controlButton->setPreferredSize(CCSize(180,80));
        controlButton->setPosition(ccp(400,100));
        controlButton->addTargetWithActionForControlEvents(this, cccontrol_selector(CtestLayer::switchProjModeCallBack), Control::EventType::TOUCH_UP_INSIDE);
        this->addChild(controlButton);
        m_controlButton_swithProjMode=controlButton;
        
    }
    
    //controlButton_transform
    {
        Scale9Sprite* btnUp=Scale9Sprite::create("button.png");
        Scale9Sprite* btnDn=Scale9Sprite::create("button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("transform", "Helvetica", 30);
        ControlButton* controlButton=ControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,Control::State::HIGH_LIGHTED);
        controlButton->setPreferredSize(CCSize(180,80));
        controlButton->setPosition(ccp(700,100));
        controlButton->addTargetWithActionForControlEvents(this,  cccontrol_selector(CtestLayer::transformCallBack), Control::EventType::TOUCH_UP_INSIDE);
        this->addChild(controlButton);
        m_controlButton_transform=controlButton;
        
    }
    //projection mode label
    m_pLabel=CCLabelTTF::create("proj mode: Perspective", "Arial", 35);
    m_pLabel->setPosition(ccp(origin.x + visibleSize.width*(3.0/4),
                              origin.y + visibleSize.height - m_pLabel->getContentSize().height-100));
    this->addChild(m_pLabel, 1);

    
    return true;
}

void CtestLayer::updateEyePos(){
    float cosA=cosf(m_A*M_PI/180);
    float sinA=sinf(m_A*M_PI/180);
    float cosB=cosf(m_B*M_PI/180);
    float sinB=sinf(m_B*M_PI/180);
    m_eyePos.setx(m_r*cosB*sinA);
    m_eyePos.sety(m_r*sinB);
    m_eyePos.setz(m_r*cosB*cosA);
    m_eyePos.setw(1);
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
void CtestLayer::transformCallBack(Ref *senderz, cocos2d::extension::Control::EventType controlEvent){
    if(m_isDoUpdate){
        //restore inital matrix
        int nActor3D=(int)m_actor3dList.size();
        for(int i=0;i<nActor3D;i++){
            m_actor3dList[i]->setTransformC3D(initalMatList[i]);
        }
        //stop update
        m_isDoUpdate=false;
        
    }else{
        //store inital matrix
        initalMatList.clear();
        int nActor3D=(int)m_actor3dList.size();
        for(int i=0;i<nActor3D;i++){
            initalMatList.push_back(m_actor3dList[i]->getTransformC3D());
        }
        //start update
        m_isDoUpdate=true;
    }
}
void CtestLayer::update(float dt){
    if(m_isDoUpdate==false)return;
    m_actor3dList[0]->rotateRelativeToParentC3D(cc3dv4(0, 1, 0, 0), 120*dt);
    m_actor3dList[0]->rotateC3D(cc3dv4(1, 0, 0, 0), 120*dt);
    m_actor3dList[2]->rotateRelativeToParentC3D(cc3dv4(0, 1, 0, 0), 120*dt);
    m_actor3dList[2]->rotateC3D(cc3dv4(0, 1, 0, 0), 240*dt);

    
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
        if(m_B>89.9)m_B=89.9;
        if(m_B<-89.9)m_B=-89.9;
        updateEyePos();
        Cc3dDirector::sharedDirector()->getCamera()->setEyePos(m_eyePos);


        
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




