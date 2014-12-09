//
//  c3dProgram.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-1-1.
//
//

#include "c3dProgram.h"
bool Cc3dProgram::init(string vertFileName,string fragFileName){
    GLchar * vertSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(vertFileName.c_str()).c_str())->getCString();
    GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(fragFileName.c_str()).c_str())->getCString();
    assert(vertSource);
    assert(fragSource);
    this->initWithVertexShaderByteArray(vertSource, fragSource);
    //bind attribute
    //below code is no longer needed, because bindPredefinedVertexAttribs() is called in link() in 3.x
//    this->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
//    this->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
//    this->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
//    this->addAttribute(kCCAttributeNameNormal, kCCVertexAttrib_Normals);
 
    //link  (must after bindAttribute)
    this->link();
    //get cocos2d-x build-in uniforms
    this->updateUniforms();
    //check gl error
    CHECK_GL_ERROR_DEBUG();
    return true;
}
void Cc3dProgram::mySetUniformsForBuiltins(const Cc3dMatrix4&M, const Cc3dMatrix4&V, const Cc3dMatrix4&P)
{
    
    if(_flags.usesP)
        setUniformLocationWithMatrix4fv(_builtInUniforms[UNIFORM_P_MATRIX], P.getArray(), 1);
    
    if(_flags.usesMV){
        Cc3dMatrix4 VM=V*M;
        setUniformLocationWithMatrix4fv(_builtInUniforms[UNIFORM_MV_MATRIX], VM.getArray(), 1);
    }
    
    if(_flags.usesMVP) {
        Cc3dMatrix4 PVM=P*V*M;
        setUniformLocationWithMatrix4fv(_builtInUniforms[UNIFORM_MVP_MATRIX], PVM.getArray(), 1);
    }
    
    if (_flags.usesNormal)
    {
        //calculate normal mat
        //http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/
        //http://www.songho.ca/opengl/gl_normaltransform.html
        //http://www.arcsynthesis.org/gltut/Illumination/Tut09%20Normal%20Transformation.html
        Cc3dMatrix4 normalMat4=transpose(inverse(M));
        GLfloat normalMat3[9];
        normalMat3[0] = normalMat4.getAt(0);normalMat3[1] = normalMat4.getAt(1);normalMat3[2] = normalMat4.getAt(2);
        normalMat3[3] = normalMat4.getAt(4);normalMat3[4] = normalMat4.getAt(5);normalMat3[5] = normalMat4.getAt(6);
        normalMat3[6] = normalMat4.getAt(8);normalMat3[7] = normalMat4.getAt(9);normalMat3[8] = normalMat4.getAt(10);
        setUniformLocationWithMatrix3fv(_builtInUniforms[UNIFORM_NORMAL_MATRIX], normalMat3, 1);
    }
    
    if(_flags.usesTime) {
        Director *director = Director::getInstance();
        // This doesn't give the most accurate global time value.
        // Cocos2D doesn't store a high precision time value, so this will have to do.
        // Getting Mach time per frame per shader using time could be extremely expensive.
        float time = director->getTotalFrames() * director->getAnimationInterval();
        
        setUniformLocationWith4f(_builtInUniforms[GLProgram::UNIFORM_TIME], time/10.0, time, time*2, time*4);
        setUniformLocationWith4f(_builtInUniforms[GLProgram::UNIFORM_SIN_TIME], time/8.0, time/4.0, time/2.0, sinf(time));
        setUniformLocationWith4f(_builtInUniforms[GLProgram::UNIFORM_COS_TIME], time/8.0, time/4.0, time/2.0, cosf(time));
    }
    
    if(_flags.usesRandom)
        setUniformLocationWith4f(_builtInUniforms[GLProgram::UNIFORM_RANDOM01], CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1());
}

void Cc3dProgram::attachUniform(string unifoName){
    m_unifoMap[unifoName] = glGetUniformLocation(this->getProgram(),unifoName.c_str());
}
void Cc3dProgram::passUnifoValue1f(string unifoName,GLfloat v){
    glUniform1f(m_unifoMap[unifoName],v);
}
void Cc3dProgram::passUnifoValue1i(string unifoName,GLint v){
    glUniform1i(m_unifoMap[unifoName],v);
}
void Cc3dProgram::passUnifoValueMatrixNfv(string unifoName,const GLfloat *array,int arrayLen){
    assert(arrayLen==4||arrayLen==9||arrayLen==16);
    switch (arrayLen) {
        case 4:
            glUniformMatrix2fv(m_unifoMap[unifoName],1,GL_FALSE,array);
            break;
        case 9:
            glUniformMatrix3fv(m_unifoMap[unifoName],1,GL_FALSE,array);
            break;
        case 16:
            glUniformMatrix4fv(m_unifoMap[unifoName],1,GL_FALSE,array);
            break;
        default:
            assert(false);
            break;
    }
    
}
void Cc3dProgram::passUnifoValueNfv(string unifoName,const GLfloat*array,int arrayLen){
    assert(arrayLen>=1&&arrayLen<=4);
    switch (arrayLen) {
        case 1:
            glUniform1fv(m_unifoMap[unifoName],1,array);
            break;
        case 2:
            glUniform2fv(m_unifoMap[unifoName],1,array);
            break;
        case 3:
            glUniform3fv(m_unifoMap[unifoName],1,array);
            break;
        case 4:
            glUniform4fv(m_unifoMap[unifoName],1,array);
            break;
        default:
            assert(false);
            break;
    }
    
}
void Cc3dProgram::passUnifoValueNiv(string unifoName,const GLint*array,int arrayLen){
    assert(arrayLen>=1&&arrayLen<=4);
    switch (arrayLen) {
        case 1:
            glUniform1iv(m_unifoMap[unifoName],1,array);
            break;
        case 2:
            glUniform2iv(m_unifoMap[unifoName],1,array);
            break;
        case 3:
            glUniform3iv(m_unifoMap[unifoName],1,array);
            break;
        case 4:
            glUniform4iv(m_unifoMap[unifoName],1,array);
            break;
        default:
            assert(false);
            break;
    }
    
}
void Cc3dProgram::passUnifoValueMatrixNfv(string unifoName,const vector<GLfloat>&valueList){
    passUnifoValueMatrixNfv(unifoName, &valueList.front(), (int)valueList.size());
}
void Cc3dProgram::passUnifoValueNfv(string unifoName,const vector<GLfloat>&valueList){
    passUnifoValueNfv(unifoName, &valueList.front(), (int)valueList.size());
}
void Cc3dProgram::passUnifoValueNiv(string unifoName,const vector<GLint>&valueList){
    passUnifoValueNiv(unifoName, &valueList.front(), (int)valueList.size());
}

