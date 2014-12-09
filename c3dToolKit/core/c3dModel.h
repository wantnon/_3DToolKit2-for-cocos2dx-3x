//
//  c3dModel.h
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-12-20.
//
//

#ifndef __HelloOpenGL__c3dModel__
#define __HelloOpenGL__c3dModel__

#include <iostream>
#include "c3dMesh.h"
class Cc3dModel:public Cc3dNode
{
protected:
    vector<Cc3dMesh*> m_meshList;
public:
    Cc3dModel(){
        init_dft();
    }
    virtual~Cc3dModel(){
   
    }
    void init_dft(){};
    int getMeshCount()const{return (int)m_meshList.size();}
    Cc3dMesh*getMeshByIndex(int index){
        assert(index>=0&&index<(int)m_meshList.size());
        return m_meshList[index];
    }
   
        
    void setProgram(Cc3dProgram *program){
        assert(program);
        this->Cc3dNode::setProgram(program);
        
        int n=(int)m_meshList.size();
        for(int i=0;i<n;i++){
            Cc3dMesh*p=m_meshList[i];
            if(p==NULL)continue;
            p->setProgram(program);
        }
    }
    void setPassUnifoCallback(c3dPassUnifoCallbackPtr callback){
        assert(callback);
        this->Cc3dNode::setPassUnifoCallback(callback);
        
        int n=(int)m_meshList.size();
        for(int i=0;i<n;i++){
            Cc3dMesh*p=m_meshList[i];
            if(p==NULL)continue;
            p->setPassUnifoCallback(callback);
        }
        
    }
 
   
	void setIsDoDepthTest(bool value){
		int n=(int)m_meshList.size();
        for(int i=0;i<n;i++){
            Cc3dMesh*p=m_meshList[i];
            if(p==NULL)continue;
            p->setIsDoDepthTest(value);
        }
        
    }
    void addMesh(Cc3dMesh*mesh);
    void submit(GLenum usage);
    void submitPosition(GLenum usage);
    void submitNormal(GLenum usage);
    void submitColor(GLenum usage);
    void submitTexCoord(GLenum usage);
    void submitIndex(GLenum usage);
};



#endif /* defined(__HelloOpenGL__c3dActor__) */
