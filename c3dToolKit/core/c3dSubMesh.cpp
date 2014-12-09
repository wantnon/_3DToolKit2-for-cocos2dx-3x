//
//  c3dSubMesh.cpp
//  HelloCpp
//
//  Created by Yang Chao (wantnon) on 14-1-2.
//
//

#include "c3dSubMesh.h"
#include "c3dDirector.h"
bool Cc3dSubMesh::init(){
    this->setAnchorPoint(ccp(0,0));
    //call parent init
    Cc3dNode::init();
    //texture
    Cc3dTexture*texture=Cc3dTextureCache::sharedTextureCache()->addImage("toolKitRes/pic/defaultTexPic.png",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE);
    setTexture(texture);
    //material
    Cc3dMaterial*material=new Cc3dMaterial();
    material->autorelease();
    material->init();
    setMaterial(material);
    //subMeshData
    Cc3dSubMeshData*subMeshData=new Cc3dSubMeshData();//empty subMeshData
    subMeshData->autorelease();
    subMeshData->init();
    setSubMeshData(subMeshData);
    //program
    Cc3dProgram*program=c3dGetProgram_c3dTexOnly();
    setProgram(program);
    //uniformPassor
    m_passUnifoCallback=passUnifoCallback_texOnly;
    //indexVBO
    Cc3dIndexVBO3d*indexVBO=new Cc3dIndexVBO3d();
    indexVBO->autorelease();
    indexVBO->init();
    setIndexVBO(indexVBO);
    return true;
}
void Cc3dSubMesh::setTexture(Cc3dTexture*texture){
    assert(texture);
    setRCObject(m_texture, texture);
}

void Cc3dSubMesh::setMaterial(Cc3dMaterial*material){
    assert(material);
    setRCObject(m_material, material);
}
void Cc3dSubMesh::setSubMeshData(Cc3dSubMeshData*subMeshData){
    assert(subMeshData);
    setRCObject(m_subMeshData, subMeshData);
    
};
void Cc3dSubMesh::setIndexVBO(Cc3dIndexVBO3d*indexVBO){
    assert(indexVBO);
    setRCObject(m_indexVBO, indexVBO);
}
void Cc3dSubMesh::submit(GLenum usage){
    submitPosition(usage);
    submitNormal(usage);
    submitColor(usage);
    submitTexCoord(usage);
    submitIndex(usage);
}
void Cc3dSubMesh::submitPosition(GLenum usage){
    assert(m_subMeshData);
    assert(m_indexVBO);
    m_indexVBO->submitPosition(m_subMeshData->getPositionArray(),m_subMeshData->getPositionArrayLen(),usage);
}
void Cc3dSubMesh::submitNormal(GLenum usage){
    assert(m_subMeshData);
    assert(m_indexVBO);
    m_indexVBO->submitNormal(m_subMeshData->getNormalArray(),m_subMeshData->getNormalArrayLen(),usage);
}
void Cc3dSubMesh::submitColor(GLenum usage){
    assert(m_subMeshData);
    assert(m_indexVBO);
    m_indexVBO->submitColor(m_subMeshData->getColorArray(),m_subMeshData->getColorArrayLen(),usage);
}
void Cc3dSubMesh::submitTexCoord(GLenum usage){
    assert(m_subMeshData);
    assert(m_indexVBO);
    m_indexVBO->submitTexCoord(m_subMeshData->getTexCoordArray(),m_subMeshData->getTexCoordArrayLen(),usage);
}
void Cc3dSubMesh::submitIndex(GLenum usage){
    assert(m_subMeshData);
    assert(m_indexVBO);
    m_indexVBO->submitIndex(m_subMeshData->getIndexArray(),m_subMeshData->getIndexArrayLen(),usage);
}

void Cc3dSubMesh::drawC3D(Renderer *renderer, const Cc3dMatrix4& modelMat){
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(Cc3dSubMesh::onDraw,this, Mat4(modelMat.getArray()), 0);
    renderer->addCommand(&_customCommand);

}
void Cc3dSubMesh::onDraw(const Mat4& modelMat, uint32_t flags){
    assert(m_subMeshData&&m_texture&&m_indexVBO
           &&m_program);
    
    if(m_indexVBO->getIndexArrayLen()==0)return;
    
	//enable array attribute
	bool isPostionAttribEnabled=Cc3dIndexVBO3d::isEnabledAttribArray_position();
	bool isTexCoordAttribEnabled=Cc3dIndexVBO3d::isEnabledAttribArray_texCoord();
	bool isNormalAttribEnabled=Cc3dIndexVBO3d::isEnabledAttribArray_normal();
	bool isColorAttribEnabled=Cc3dIndexVBO3d::isEnabledAttribArray_color();
    Cc3dIndexVBO3d::enableAttribArray_position(true);
	Cc3dIndexVBO3d::enableAttribArray_texCoord(true);
	Cc3dIndexVBO3d::enableAttribArray_normal(true);
	Cc3dIndexVBO3d::enableAttribArray_color(true);
    //apply state
    //for performance sake, we only apply state, not restore
    {
        //set depthTest
        CCDirector::sharedDirector()->setDepthTest(m_isDoDepthTest);
        //set blend function
        ccGLBlendFunc(m_blendFunc.src, m_blendFunc.dst);
    }
    
    //enable server state (i don't know what this means :( )
    ////ccGLEnable(m_eGLServerState);
    //pass values for cocos2d-x build-in uniforms
    Cc3dProgram*program=(Cc3dProgram*)getShaderProgram();
    program->use();
    program->mySetUniformsForBuiltins(Cc3dMatrix4(modelMat.m),
                                      Cc3dDirector::sharedDirector()->getCamera()->calculateViewMat(),
                                      Cc3dDirector::sharedDirector()->getCamera()->calculateProjectionMat());
    //pass values for my own uniforms
    m_passUnifoCallback(this, program,Cc3dMatrix4(modelMat.m));//m_program
    //attach texture to texture attach point
    ccGLBindTexture2DN(0, this->m_texture->getID());
    //draw
    m_indexVBO->setPointers();
    m_indexVBO->draw(GL_TRIANGLES);
	//recover array attribute state
	Cc3dIndexVBO3d::enableAttribArray_position(isPostionAttribEnabled);
	Cc3dIndexVBO3d::enableAttribArray_texCoord(isTexCoordAttribEnabled);
	Cc3dIndexVBO3d::enableAttribArray_normal(isNormalAttribEnabled);
	Cc3dIndexVBO3d::enableAttribArray_color(isColorAttribEnabled);
}

bool Cc3dSubMesh::getIsDoDepthTest()const {
    return m_isDoDepthTest;
}
void Cc3dSubMesh::setIsDoDepthTest(bool isDoDepthTest){
    m_isDoDepthTest=isDoDepthTest;
    
}
