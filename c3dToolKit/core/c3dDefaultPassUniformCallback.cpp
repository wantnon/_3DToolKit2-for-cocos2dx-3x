//
//  c3dDefaultPassUniformCallback.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-1-4.
//
//

#include "c3dDefaultPassUniformCallback.h"
#include "c3dSubMesh.h"
#include "c3dLightSource.h"
#include "c3dNode.h"
#include "c3dDirector.h"
void passUnifoCallback_showDepth(Cc3dNode*node,Cc3dProgram*program,const Cc3dMatrix4&modelMat){
    assert(program->getName()=="toolKitRes/shader/c3dShowDepth");
    //do nothing
}
void passUnifoCallback_texOnly(Cc3dNode*node,Cc3dProgram*program,const Cc3dMatrix4&modelMat){
    assert(program->getName()=="toolKitRes/shader/c3dTexOnly");
    //do nothing
}
void passUnifoCallback_renderDepth(Cc3dNode*node,Cc3dProgram*program,const Cc3dMatrix4&modelMat){
    assert(program->getName()=="toolKitRes/shader/c3dRenderDepth");
    //do nothing
}

void passUnifoCallback_texOnlyAndShadow(Cc3dNode*node,Cc3dProgram*program,const Cc3dMatrix4&modelMat){
    assert(program->getName()=="toolKitRes/shader/c3dTexOnlyAndShadow");
    Cc3dSubMesh*sprite=(Cc3dSubMesh*)node;
    //----prepare uniform values
    //other uniform values
    Cc3dMatrix4 worldToLightViewportTexCoord=calculateWorldToViewportTexCoordMatrix(
                                                                                    Cc3dDirector::sharedDirector()->getCameraLightView()->calculateViewMat(),
                                                                                    Cc3dDirector::sharedDirector()->getCameraLightView()->calculateProjectionMat()
                                                                                    );
    //
    //----pass uniform values
    program->passUnifoValueMatrixNfv("worldToLightViewportTexCoord",worldToLightViewportTexCoord.convertToVector());
    program->passUnifoValueMatrixNfv("modelMat",modelMat.convertToVector());
    
    assert(Cc3dDirector::sharedDirector()->getShadowMap());
    ccGLBindTexture2DN(1, Cc3dDirector::sharedDirector()->getShadowMap()->getID());
}
void passUnifoCallback_classicLighting(Cc3dNode*node,Cc3dProgram*program,const Cc3dMatrix4&modelMat){
    assert(program->getName()=="toolKitRes/shader/c3dClassicLighting");
    Cc3dSubMesh*sprite=(Cc3dSubMesh*)node;
    //----prepare uniform values
    //other uniform values
    Cc3dVector4 lightPosWorld=Cc3dDirector::sharedDirector()->getLightSource()->getWorldPositionC3D();
    Cc3dVector4 eyePosWorld=Cc3dDirector::sharedDirector()->getCamera()->getEyePos();
    Cc3dVector4 diffuseMaterial=sprite->getMaterial()->getDiffuse();
    Cc3dVector4 diffuseLight=Cc3dDirector::sharedDirector()->getLightSource()->getDiffuse();
    Cc3dVector4 ambientMaterial=sprite->getMaterial()->getAmbient();
    Cc3dVector4 ambientLight=Cc3dDirector::sharedDirector()->getLightSource()->getAmbient();
    Cc3dVector4 specularMaterial=sprite->getMaterial()->getSpecular();
    Cc3dVector4 specularLight=Cc3dDirector::sharedDirector()->getLightSource()->getSpecular();
    float shininess=sprite->getMaterial()->getShininess();
    //----pass uniform values
    program->passUnifoValueMatrixNfv("modelMat",modelMat.convertToVector());
    program->passUnifoValueNfv("lightPos_world",lightPosWorld.convertToVector());
    program->passUnifoValueNfv("eyePos_world",eyePosWorld.convertToVector());
    program->passUnifoValueNfv("diffuseMaterial", diffuseMaterial.convertToVector());
    program->passUnifoValueNfv("diffuseLight",diffuseLight.convertToVector());
    program->passUnifoValueNfv("ambientMaterial", ambientMaterial.convertToVector());
    program->passUnifoValueNfv("ambientLight", ambientLight.convertToVector());
    program->passUnifoValueNfv("specularMaterial", specularMaterial.convertToVector());
    program->passUnifoValueNfv("specularLight", specularLight.convertToVector());
    program->passUnifoValue1f("shininess", shininess);
};

