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
void Cc3dRoot::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags){
    
    //store depthTest state
    GLboolean isDoDepthTestOld;
    glGetBooleanv(GL_DEPTH_TEST,&isDoDepthTestOld);
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
    //resotre depthTest state
    CCDirector::sharedDirector()->setDepthTest(isDoDepthTestOld);
    //disable array attribute
    ////	Cc3dIndexVBO3d::disableAttribArrays();



}