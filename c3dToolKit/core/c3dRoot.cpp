//
//  c3dRoot.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-1-4.
//
//

#include "c3dRoot.h"
#include "c3dIndexVBO3d.h"
bool Cc3dRoot::init(){
    this->Cc3dNode::init();
    return true;
}
void Cc3dRoot::onBegin(){
    //store depthTest state
    glGetBooleanv(GL_DEPTH_TEST,&m_isDoDepthTestOld);
 ////   glEnable(GL_CULL_FACE);
    //    glCullFace(GL_FRONT_FACE);
    
}
void Cc3dRoot::onEnd(){
    //resotre depthTest state
    CCDirector::sharedDirector()->setDepthTest(m_isDoDepthTestOld);
  ////  glDisable(GL_CULL_FACE);
    //disable array attribute
    ////	Cc3dIndexVBO3d::disableAttribArrays();
}
void Cc3dRoot::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags){
    
    
    _beginCommand.init(_globalZOrder);
    _beginCommand.func = CC_CALLBACK_0(Cc3dRoot::onBegin,this);
    renderer->addCommand(&_beginCommand);
    
    int i = 0;
    if(!_children.empty())
    {
        sortAllChildren();
        // draw children zOrder < 0
        for( ; i < _children.size(); i++ )
        {
            Cc3dNode* node = (Cc3dNode*)_children.at(i);
            
            if ( node && node->getLocalZOrder() < 0 )
                node->visitC3D(renderer);
            else
                break;
        }
        
        for(auto it=_children.cbegin()+i; it != _children.cend(); ++it)
            ((Cc3dNode*)(*it))->visitC3D(renderer);
    }else{
        
    }
    
    _endCommand.init(_globalZOrder);
    _endCommand.func = CC_CALLBACK_0(Cc3dRoot::onBegin,this);
    renderer->addCommand(&_endCommand);
    
    
    
    
}