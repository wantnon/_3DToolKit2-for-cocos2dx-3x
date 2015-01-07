//
//  c3dSubMeshData.h
//  HelloCpp
//
//  Created by Yang Chao (wantnon) on 14-1-2.
//
//

#ifndef __HelloCpp__c3dSubMeshData__
#define __HelloCpp__c3dSubMeshData__

#include <iostream>
#include "c3dVector.h"
#include "c3dIDTriangle.h"
#include "c3dCommonFunc.h"
class Cc3dVertex USED_FOR_SUBMIT_TO_VBO
{
protected:
    Cc3dVector4 m_pos;
    Cc3dVector2 m_texCoord;
    Cc3dVector4 m_norm;
    Cc3dVector4 m_color;
    Cc3dVector2 m_texCoord2;
public:
    Cc3dVertex(){
        initMembers();
    };
    ~Cc3dVertex(){};//used for submit to vbo, so destructor must not be virtual
    Cc3dVertex(const Cc3dVector4&pos,const Cc3dVector2&texCoord,const Cc3dVector4&norm,const Cc3dVector4&color=Cc3dVector4(1,1,1,1),const Cc3dVector2&texCoord2=Cc3dVector2(0,0)){
        initMembers();
        init(pos,texCoord,norm,color,texCoord2);
    }
    void init(const Cc3dVector4&pos,const Cc3dVector2&texCoord,const Cc3dVector4&norm,const Cc3dVector4&color=Cc3dVector4(1,1,1,1),const Cc3dVector2&texCoord2=Cc3dVector2(0,0)){
        m_pos=pos;
        m_texCoord=texCoord;
        m_norm=norm;
        m_color=color;
        m_texCoord2=texCoord2;
    }
    Cc3dVector4 getPos()const {
        return m_pos;
    }
    Cc3dVector4 getNorm()const {
        return m_norm;
    }
    Cc3dVector4 getColor()const{
        return m_color;
    }
    Cc3dVector2 getTexCoord()const {
        return m_texCoord;
    }
    void setPos(const Cc3dVector4&pos){
        m_pos=pos;
        
    }
    
    void setTexCoord(const Cc3dVector2&texCoord){
        m_texCoord=texCoord;
    }
    void setNorm(const Cc3dVector4&norm){
        m_norm=norm;
    }
    void setColor(const Cc3dVector4&color){
        m_color=color;
    }
    void setTexCoord2(const Cc3dVector2&texCoord2){
        m_texCoord2=texCoord2;
    }
protected:
    void initMembers(){
        m_color.init(1,1,1,1);
    }
    
};

class Cc3dSubMeshData:public CCObject
{
public:
    Cc3dSubMeshData(){};
    virtual~Cc3dSubMeshData(){};
    
    bool init(){return true;}
    
    void initPositionList(const float positionArray[],int positionArrayLen);
    void initTexCoordList(const float texCoordArray[],int texCoordArrayLen);
    void initNormalList(const float normalArray[],int normalArrayLen);
    void initColorList(const float colorArray[],int colorArrayLen);
    void initIDtriList(const short indexArray[],int indexArrayLen);
    
    void initPositionList(const vector<Cc3dVector4>&positionList){m_positionList=positionList;};
    void initTexCoordList(const vector<Cc3dVector2>&texCoordList){m_texCoordList=texCoordList;};
    void initNormalList(const vector<Cc3dVector4>&normalList){m_normalList=normalList;};
    void initColorList(const vector<Cc3dVector4>&colorList){m_colorList=colorList;};
    void initIDtriList(const vector<Cc3dIDTriangle>&IDtriList){m_IDtriList=IDtriList;};
    
    int getPositionArrayLen()const{return (int)m_positionList.size()*4;}
    int getTexCoordArrayLen()const{return (int)m_texCoordList.size()*2;}
    int getNormalArrayLen()const{return (int)m_normalList.size()*4;}
    int getColorArrayLen()const{return (int)m_colorList.size()*4;}
    int getIndexArrayLen()const{return (int)m_IDtriList.size()*3;}
    
    const float*getPositionArray(){return m_positionList.front().getArray();}
    const float*getTexCoordArray(){return m_texCoordList.front().getArray();}
    const float*getNormalArray(){return m_normalList.front().getArray();}
    const float*getColorArray(){return m_colorList.front().getArray();}
    const short*getIndexArray(){return m_IDtriList.front().getArray();}
public:
    Cc3dVertex getVertexByIndex(int index)const {
        assert(index>=0&&index<(int)m_positionList.size());
        Cc3dVertex vertex(m_positionList[index],m_texCoordList[index],m_normalList[index],m_colorList[index]);
        return vertex;
    }
    Cc3dIDTriangle getIDtriByIndex(int index)const {
        assert(index>=0&&index<(int)m_IDtriList.size());
        return m_IDtriList[index];
    }
    void setVertexByIndex(int index,const Cc3dVertex&vertex){
        assert(index>=0&&index<(int)m_positionList.size());
        m_positionList[index]=vertex.getPos();
        m_texCoordList[index]=vertex.getTexCoord();
        m_normalList[index]=vertex.getNorm();
        m_colorList[index]=vertex.getColor();
    }
public:
    vector<Cc3dVector4> m_positionList;
    vector<Cc3dVector2> m_texCoordList;
    vector<Cc3dVector4> m_normalList;
    vector<Cc3dVector4> m_colorList;
    vector<Cc3dIDTriangle> m_IDtriList;

};
#endif /* defined(__HelloCpp__c3dSubMeshData__) */
