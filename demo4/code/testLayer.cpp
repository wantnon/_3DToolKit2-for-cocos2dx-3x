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
 
    updateEyePos();   //update eye pos
    camera->setEyePos(m_eyePos);
    
    
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
    Cc3dTexture*tex=Cc3dTextureCache::sharedTextureCache()->addImage("white_outline.png");

    

  
    //actors
    float r=40;
    m_nRow=3;
    m_nCol=4;
    m_nDeep=2;
    m_actor3dMat3d.resize(m_nRow);
    for(int i=0;i<m_nRow;i++){
        m_actor3dMat3d[i].resize(m_nCol);
        for(int j=0;j<m_nCol;j++){
            m_actor3dMat3d[i][j].resize(m_nDeep);
        }
    }
    
    
    float xspan=m_nCol*2*r;
    float yspan=m_nRow*2*r;
    float zspan=m_nDeep*2*r;
    float xmin=-xspan/2;
    float ymin=-yspan/2;
    float zmin=-zspan/2;
    for(int i=0;i<m_nRow;i++){
        for(int j=0;j<m_nCol;j++){
            for(int k=0;k<m_nDeep;k++){
                //mesh
                Cc3dMesh*mesh_box=c3dCreateBoxMesh(r,cc3dv4(1, 1, 1, 1),cc3dv4(1, 1, 1, 1),cc3dv4(1, 1, 1, 1),cc3dv4(1, 1, 1, 1),
                                                   cc3dv4(1, 1, 1, 1),cc3dv4(1, 1, 1, 1),cc3dv4(1, 1, 1, 1),cc3dv4(1, 1, 1, 1));
                mesh_box->setTexture(tex);
                mesh_box->setMaterial(material);
                //actor
                Cc3dActor* actor3D=new Cc3dActor();
                actor3D->autorelease();
                actor3D->init();
                float x=xmin+r+j*2*r;
                float y=ymin+r+(m_nRow-1-i)*2*r;
                float z=zmin+r+(m_nDeep-1-k)*2*r;
                actor3D->setPositionC3D(cc3dv4(x, y, z, 1));
                actor3D->addMesh(mesh_box);
                actor3D->setPassUnifoCallback(passUnifoCallback_classicLighting);
                actor3D->setProgram(program);
                m_root3d->addChild(actor3D,0);
                m_actor3dMat3d[i][j][k]=actor3D;
                
            }
        }
    }
    

    //submit actors
    for(int i=0;i<m_nRow;i++){
        for(int j=0;j<m_nCol;j++){
            for(int k=0;k<m_nDeep;k++){
                Cc3dActor*actor3D=m_actor3dMat3d[i][j][k];
                actor3D->submit(GL_STATIC_DRAW);
            }
        }
    }
    

    

    //controlButton_swithProjMode
    {
        Scale9Sprite* btnUp=Scale9Sprite::create("button.png");
        Scale9Sprite* btnDn=Scale9Sprite::create("button_dn.png");
        CCLabelTTF*title=CCLabelTTF::create("clear", "Helvetica", 30);
        ControlButton* controlButton=ControlButton::create(title, btnUp);
        controlButton->setBackgroundSpriteForState(btnDn,Control::State::HIGH_LIGHTED);
        controlButton->setPreferredSize(CCSize(180,80));
        controlButton->setPosition(ccp(400,100));
        controlButton->addTargetWithActionForControlEvents(this,  cccontrol_selector(CtestLayer::clearCallBack), Control::EventType::TOUCH_UP_INSIDE);
        this->addChild(controlButton);
        m_controlButton_clear=controlButton;
        
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

void CtestLayer::clearCallBack(Ref *senderz, cocos2d::extension::Control::EventType controlEvent){
    for(int i=0;i<m_nRow;i++){
        for(int j=0;j<m_nCol;j++){
            for(int k=0;k<m_nDeep;k++){
                Cc3dActor*actor=m_actor3dMat3d[i][j][k];
                int meshCount=actor->getModel()->getMeshCount();
                for(int u=0;u<meshCount;u++){
                    Cc3dMesh*mesh=actor->getModel()->getMeshByIndex(u);
                    int subMeshCount=mesh->getSubMeshCount();
                    for(int v=0;v<subMeshCount;v++){
                        Cc3dSubMesh*subMesh=mesh->getSubMeshByIndex(v);
                        Cc3dSubMeshData*subMeshData=subMesh->getSubMeshData();
                        vector<Cc3dVector4>&colorList=subMeshData->m_colorList;
                        for(int k=0;k<(int)colorList.size();k++){
                            colorList[k]=Cc3dVector4(1,1,1,1);
                        }
                        subMesh->submitColor(GL_STREAM_DRAW);
                    }
                }
            }
        }
    }
}

void CtestLayer::update(float dt){
    
    if(m_isDoUpdate==false)return;
    
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
        //-------
        CCRect viewportRect=Director::getInstance()->getOpenGLView()->getViewPortRect();
        float viewport[4]={viewportRect.getMinX(),viewportRect.getMinY(),viewportRect.size.width,viewportRect.size.height};
        
        Cc3dCamera*camera=Cc3dDirector::sharedDirector()->getCamera();
        Cc3dVector4 eyePos=camera->getEyePos();
        Cc3dVector4 winPos=Cc3dVector4(m_mosPos.x,viewport[3]-m_mosPos.y,0,1);
        Cc3dMatrix4 projMat=camera->calculateProjectionMat();
        Cc3dMatrix4 viewMat=camera->calculateViewMat();
        Cc3dMatrix4 projViewInverse=inverse(projMat*viewMat);
        Cc3dVector4 worldPos= convertFromViewportSpaceToWorldSpace(winPos, projMat, viewMat, viewport);
        
        
        int i_firstIntersectActor=-1;
        int j_firstIntersectActor=-1;
        int k_firstIntersectActor=-1;
        float minDisIntersectPointToEyePos=INFINITY;
        
        for(int i=0;i<m_nRow;i++){
            for(int j=0;j<m_nCol;j++){
                for(int k=0;k<m_nDeep;k++){
                    Cc3dActor*actor=m_actor3dMat3d[i][j][k];
                    int meshCount=actor->getModel()->getMeshCount();
                    for(int u=0;u<meshCount;u++){
                        Cc3dMesh*mesh=actor->getModel()->getMeshByIndex(u);
                        int subMeshCount=mesh->getSubMeshCount();
                        for(int v=0;v<subMeshCount;v++){
                            Cc3dSubMesh*subMesh=mesh->getSubMeshByIndex(v);
                            Cc3dSubMeshData*subMeshData=subMesh->getSubMeshData();
                            const vector<Cc3dVector4>&positionList=subMeshData->m_positionList;
                            const vector<Cc3dIDTriangle>&IDtriList=subMeshData->m_IDtriList;
                            int nIDtri=(int)IDtriList.size();
                            for(int w=0;w<nIDtri;w++){
                                const Cc3dIDTriangle&IDtri=IDtriList[w];
                                int ID0=IDtri.vID(0);
                                int ID1=IDtri.vID(1);
                                int ID2=IDtri.vID(2);
                                Cc3dVector4 p0=subMesh->convertToWorldSpaceC3D(positionList[ID0]);
                                Cc3dVector4 p1=subMesh->convertToWorldSpaceC3D(positionList[ID1]);
                                Cc3dVector4 p2=subMesh->convertToWorldSpaceC3D(positionList[ID2]);
                                if(isLine3dIntersectWithTriangle3d(eyePos, worldPos-eyePos, p0, p1, p2)){//intersect
                                    Cc3dVector4 intersectPos=getIntersectPointOfLine3dIntersectWithPlane3d(eyePos, worldPos-eyePos, p0, cross(p1-p0, p2-p1));
                                    float disIntersectPointToEyePos=getLength(intersectPos-eyePos);
                                    if(disIntersectPointToEyePos<minDisIntersectPointToEyePos){
                                        minDisIntersectPointToEyePos=disIntersectPointToEyePos;
                                        i_firstIntersectActor=i;
                                        j_firstIntersectActor=j;
                                        k_firstIntersectActor=k;
                                    }
                                    
                                }
                            }
                        }
                    }
                }
            }
        }//got i_firstIntersectActor, j_firstIntersectActor, k_firstIntersectActor, minDisIntersectPointToEyePos
        if(i_firstIntersectActor!=-1){//have intersect
            Cc3dActor*firstIntersectActor=m_actor3dMat3d[i_firstIntersectActor][j_firstIntersectActor][k_firstIntersectActor];
            int meshCount=firstIntersectActor->getModel()->getMeshCount();
            for(int i=0;i<meshCount;i++){
                Cc3dMesh*mesh=firstIntersectActor->getModel()->getMeshByIndex(i);
                int subMeshCount=mesh->getSubMeshCount();
                for(int j=0;j<subMeshCount;j++){
                    Cc3dSubMesh*subMesh=mesh->getSubMeshByIndex(j);
                    Cc3dSubMeshData*subMeshData=subMesh->getSubMeshData();
                    vector<Cc3dVector4>&colorList=subMeshData->m_colorList;
                    for(int k=0;k<(int)colorList.size();k++){
                        colorList[k]=Cc3dVector4(0,0,1,1);
                    }
                    subMesh->submitColor(GL_STREAM_DRAW);
                    
                }
            }
            
        }
        
        
        break;//only consider the first touch
        
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












