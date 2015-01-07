#ifndef _c3dSkinActor__
#define _c3dSkinActor__
#include<vector>
using namespace std;
#include "c3dActor.h"
class Cc3dAniFrame
{
protected:
	Cc3dMatrix4 m_vertexTransformMat;
	float m_time;
public:
	Cc3dAniFrame(){
		m_time=0;
	}
	virtual ~Cc3dAniFrame(){
	
	}

	Cc3dAniFrame(const Cc3dMatrix4&vertexTransformMat,float time){
		m_vertexTransformMat=vertexTransformMat;
		m_time=time;
	}

	void setVertexTransformMat(const Cc3dMatrix4&vertexTransformMat){m_vertexTransformMat=vertexTransformMat;}
	Cc3dMatrix4 getVertexTransformMat()const{return m_vertexTransformMat;}
	void setTime(float time){m_time=time;}
	float getTime()const{return m_time;}
};
class Cc3dAniLayer:public CCObject
{
protected:
	vector<Cc3dAniFrame>  m_aniFrameList;
public:
	void addAniFrame(const Cc3dAniFrame&aniFrame){
		m_aniFrameList.push_back(aniFrame);
	}
	const Cc3dAniFrame&getAniFrameByIndex(int index){
		assert(index>=0&&index<(int)m_aniFrameList.size());
		return m_aniFrameList[index];
	}
	int getAniFrameCount()const {return (int)m_aniFrameList.size();}
	Cc3dAniFrame getAniFrameByTime(float time){
		assert(m_aniFrameList.empty()==false);
		assert(time>=m_aniFrameList[0].getTime());
		assert(time<=m_aniFrameList[(int)m_aniFrameList.size()-1].getTime());
		Cc3dAniFrame aniFrameFoe;
		Cc3dAniFrame aniFrameNxt;
		int nAniFrame=(int)m_aniFrameList.size();
		for(int i=0;i<nAniFrame;i++){
			const Cc3dAniFrame&aniFrame=m_aniFrameList[i];
			if(aniFrame.getTime()==time){
				return aniFrame;
			}else if(aniFrame.getTime()>time){
				assert(i-1>=0);
				aniFrameFoe=m_aniFrameList[i-1];
				aniFrameNxt=m_aniFrameList[i];
				//calculate interpolated aniFrame
				float timeFoe=aniFrameFoe.getTime();
				const Cc3dMatrix4&vertexTransformMatFoe=aniFrameFoe.getVertexTransformMat();
				float timeNxt=aniFrameNxt.getTime();
				const Cc3dMatrix4&vertexTransformMatNxt=aniFrameNxt.getVertexTransformMat();
				float weightFoe=(timeNxt-time)/(timeNxt-timeFoe);
				float weightNxt=(time-timeFoe)/(timeNxt-timeFoe);
				Cc3dMatrix4 vertexTransformMat=vertexTransformMatFoe*weightFoe+vertexTransformMatNxt*weightNxt;
				Cc3dAniFrame aniFrame(vertexTransformMat,time);
				return aniFrame;
			}
		}
		assert(false);
	}
	//void doExport(string filePath,bool valueOnly);
	void doImport(string filePath,bool valueOnly);

};
class Cc3dSkinCluster:public CCObject
{
protected:
	vector<int> m_vertexIndexList;
	vector<float> m_vertexWeightList;
	vector<Cc3dAniLayer*>  m_aniLayerList;
public:
	Cc3dSkinCluster(){
	
	}
	virtual~Cc3dSkinCluster(){
		for(int i=0;i<(int)m_aniLayerList.size();i++){
			Cc3dAniLayer*aniLayer=m_aniLayerList[i];
			if(aniLayer)aniLayer->release();
		}
	}
	//void doExport(string filePath,bool valueOnly);
	void doImport(string filePath,bool valueOnly);
	void addAniLayer(Cc3dAniLayer*aniLayer){
		m_aniLayerList.push_back(aniLayer);
		aniLayer->retain();
	}
	Cc3dAniLayer* getAniLayerByIndex(int index){
		assert(index>=0&&index<(int)m_aniLayerList.size());
		return m_aniLayerList[index];
		
	}
	int getAniLayerCount()const{return (int)m_aniLayerList.size();}
	void setVertexIndexList(int*vertexIndexArray,int len){
		m_vertexIndexList.clear();
		for(int i=0;i<len;i++){
			m_vertexIndexList.push_back(vertexIndexArray[i]);
		}
	}
	void setVertexWeightList(double*vertexWeightArray,int len){
		m_vertexWeightList.clear();
		for(int i=0;i<len;i++){
			m_vertexWeightList.push_back(vertexWeightArray[i]);
		}
	}
	int getVertexIndexAt(int index){
		assert(index>=0&&index<(int)m_vertexIndexList.size());
		return m_vertexIndexList[index];
	}
	float getVertexWeightAt(int index){
		assert(index>=0&&index<(int)m_vertexWeightList.size());
		return m_vertexWeightList[index];
	}
	int getVertexIndexCount()const{return (int)m_vertexIndexList.size();}
	int getVertexWeightCount()const{
		assert((int)m_vertexWeightList.size()==(int)m_vertexIndexList.size());
		return (int)m_vertexWeightList.size();
	
	}


};
class Cc3dSkin:public CCObject
{
protected:
	int m_skinType;
	vector<Cc3dSkinCluster*> m_clusterList;
public:
	Cc3dSkin(){
		m_skinType=0;
	}
	virtual ~Cc3dSkin(){
		for(int i=0;i<(int)m_clusterList.size();i++){
			m_clusterList[i]->release();
		}
	}
	//void doExport(string filePath,bool valueOnly);
	void doImport(string filePath,bool valueOnly);
	void setSkinType(int skinType){m_skinType=skinType;} 
	int getSkinType()const{return m_skinType;}
	void addCluster(Cc3dSkinCluster*cluster){
		m_clusterList.push_back(cluster);
		cluster->retain();
	}
	Cc3dSkinCluster* getClusterByIndex(int index){
		assert(index>=0&&index<(int)m_clusterList.size());
		return m_clusterList[index];
	}
	int getClusterCount(){
		return (int)m_clusterList.size();
	}

};
class _CmeshIDvID{
protected:
	int m_meshID;
	int m_vID;
public:
	_CmeshIDvID(){
		initMembers();
	}
	_CmeshIDvID(int meshID,int vID){
		init(meshID,vID);
	}
	virtual ~_CmeshIDvID(){
		
	}

	int getMeshID()const{return m_meshID;}
	int getvID()const{return m_vID;}
	void init(int meshID,int vID){
		m_meshID=meshID;
		m_vID=vID;
	}
protected:
	void initMembers(){
		m_meshID=-1;
		m_vID=-1;
	}

};
class Cc3dSkinSubMeshData:public Cc3dSubMeshData
{
public:
	//void doExport(string filePath,bool valueOnly);
	void doImport(string filePath,bool valueOnly);
};
class Cc3dSkinSubMesh:public Cc3dSubMesh
{
protected:
	Cc3dSkinSubMeshData*m_subMeshData_backup;
public:
	Cc3dSkinSubMesh(){
		m_subMeshData_backup=NULL;
	}
	virtual ~Cc3dSkinSubMesh(){
		if(m_subMeshData_backup)m_subMeshData_backup->release();
	}
	//void doExport(string filePath,bool valueOnly);
	void doImport(string filePath,bool valueOnly);
	void backupSubMeshData(){//call after m_subMeshData has established
		m_subMeshData_backup=new Cc3dSkinSubMeshData();
		m_subMeshData_backup->autorelease();
		m_subMeshData_backup->retain();
		//copy m_subMeshData
		*m_subMeshData_backup=*(Cc3dSkinSubMeshData*)m_subMeshData;
	}
	Cc3dVertex getBackupVertexByIndex(int index){
		return m_subMeshData_backup->getVertexByIndex(index); 
	}

};

class Cc3dSkinMesh:public Cc3dMesh
{
protected:
	void* m_fbxMeshPtr;//this pointer only used for loading fbx data from fbx
	Cc3dSkin*m_skin;
	vector<vector<_CmeshIDvID> > m_vertexDupList;//m_vertexDupList[meshVID] is duplicated to {(meshID,vID),(meshID,vID),...}
public:
	Cc3dSkinMesh(){
		m_fbxMeshPtr=NULL;
		m_skin=NULL;
	}
	virtual ~Cc3dSkinMesh(){
		if(m_skin)m_skin->release();
	}
	//void doExport(string filePath,bool valueOnly);
	void doImport(string filePath,bool valueOnly);
	void setFbxMeshPtr(void*fbxMeshPtr){m_fbxMeshPtr=fbxMeshPtr;}
	void* getFbxMeshPtr(){return m_fbxMeshPtr;}

	void deform(int aniLayerIndex,float time){
		if(m_skin){
            int meshVertexCount = (int)m_vertexDupList.size();

            vector<Cc3dMatrix4> deformationList;//deformation of each vertex
            deformationList.resize(meshVertexCount);
            for(int i=0;i<(int)deformationList.size();i++)deformationList[i]=zeroMat();
            vector<float> weightList;//weight of each vertex
            weightList.resize(meshVertexCount);
            for(int i=0;i<(int)weightList.size();i++)weightList[i]=0;
            
            // For all clusters, accumulate their deformation and weight
            // on each vertices and store them in deformationList and weightList.
			int clusterCount = m_skin->getClusterCount();
			for ( int clusterIndex=0; clusterIndex<clusterCount; ++clusterIndex)
			{
				Cc3dSkinCluster*cluster=m_skin->getClusterByIndex(clusterIndex);

				int aniLayerCount=cluster->getAniLayerCount();
				assert(aniLayerIndex<aniLayerCount);
				Cc3dAniLayer*aniLayer=cluster->getAniLayerByIndex(aniLayerIndex);
				int aniFrameCount=aniLayer->getAniFrameCount();
				const Cc3dAniFrame&aniFrame=aniLayer->getAniFrameByTime(time);
				Cc3dMatrix4 vertexTransformMat=aniFrame.getVertexTransformMat();
				float time=aniFrame.getTime();
				int clusterVertexIndexCount=cluster->getVertexIndexCount();

				for (int k = 0; k < clusterVertexIndexCount; ++k) 
				{             
					int meshVID = cluster->getVertexIndexAt(k);

					// Sometimes, the mesh can have less points than at the time of the skinning
					// because a smooth operator was active when skinning but has been deactivated during export.
					if (meshVID >= meshVertexCount)
						continue;

					float weight = cluster->getVertexWeightAt(k);

					if (weight == 0.0)continue;

					// Compute the influence of the link on the vertex.
					Cc3dMatrix4 influenceMat = vertexTransformMat*weight;
					{
						// Add to the sum of the deformations on the vertex.
						deformationList[meshVID]=deformationList[meshVID]+influenceMat;

						// Add to the sum of weights to either normalize or complete the vertex.
						weightList[meshVID]+=weight;
					}
				}

			}	
			//Actually deform each vertices here by information stored in deformationList and weightList
			for (int i = 0; i < meshVertexCount; i++) 
			{
				const int meshVID=i;
				vector<_CmeshIDvID> vertexDup=m_vertexDupList[meshVID];
				int nDup=(int)vertexDup.size();
				if(nDup==0)continue;
				//deform vertexes in vertexDup
				for(int j=0;j<nDup;j++){
					int meshID = vertexDup[j].getMeshID();
					int vID = vertexDup[j].getvID();
					Cc3dVertex newVertex=((Cc3dSkinSubMesh*)this->getSubMeshByIndex(meshID))->getBackupVertexByIndex(vID);//deform result
					// Deform vertex
					if (weightList[meshVID] != 0.0) 
					{
						//deform pos
						if(j==0){
							newVertex.setPos(deformationList[meshVID]*newVertex.getPos());
						}else{
							//same as vertexDup[0]
							int meshID0=vertexDup[0].getMeshID();
							int vID0=vertexDup[0].getvID();
							newVertex.setPos(this->getSubMeshByIndex(meshID0)->getVertexByIndex(vID0).getPos());
						}
						//deform norm
						newVertex.setNorm(deformationList[meshVID]*newVertex.getNorm());

					}
					// replace vertexDup[j] with newVertex
					this->getSubMeshByIndex(meshID)->setVertexByIndex(vID,newVertex);
				}

			}
		}
	}
	void setSkin(Cc3dSkin*skin){
		if(m_skin==NULL){
			m_skin=skin;
			m_skin->retain();
		}else{
			m_skin->release();
			m_skin=skin;
			m_skin->retain();
		}
	}
	Cc3dSkin*getSkin(){return m_skin;}
	void setVertexDupList(const vector<vector<_CmeshIDvID> >&vertexDupList){
		m_vertexDupList=vertexDupList;
	}
	vector<_CmeshIDvID> getVertexDupByIndex(int index){
		assert(index>=0&&index<(int)m_vertexDupList.size());
		return m_vertexDupList[index];
	}
	
};
class Cc3dAniLayerInfo:public CCObject
{
protected:
	//all time is second
	float m_interval;
	float m_startTime;
	float m_endTime;
	float m_curTime;
public:
	Cc3dAniLayerInfo(){
		m_interval=0;
		m_startTime=0;
		m_endTime=0;
		m_curTime=0;
	}
	//void doExport(string filePath,bool valueOnly);
	void doImport(string filePath,bool valueOnly);
	void setInterval(float interval){m_interval=interval;}
	void setStartTime(float startTime){m_startTime=startTime;}
	void setEndTime(float endTime){m_endTime=endTime;}
	void setCurTime(float curTime){m_curTime=curTime;}
	float getInterval()const{return m_interval;}
	float getStartTime()const{return m_startTime;}
	float getEndTime()const{return m_endTime;}
	float getCurTime()const{return m_curTime;}
	void progress(float dt){
		if((m_endTime>m_startTime)==false)return;
		m_curTime+=dt;
		if(m_curTime>m_endTime)m_curTime=0;//m_curTime-=(m_endTime-m_startTime);//wrap
	}


};
class Cc3dSkinActor:public Cc3dActor
{
protected:
	int m_aniLayerIndex;
	float m_interval;
	vector<Cc3dAniLayerInfo*> m_aniLayerInfoList;
public:
	Cc3dSkinActor(){
		m_interval=1.0f/10; //1.0/60;
		m_aniLayerIndex=0;
	}
	virtual ~Cc3dSkinActor(){
		for(int i=0;i<(int)m_aniLayerInfoList.size();i++){
			Cc3dAniLayerInfo*aniLayerInfo=m_aniLayerInfoList[i];
			if(aniLayerInfo)aniLayerInfo->release();
		}
	}
	//void doExport(string filePath,bool valueOnly);
	void doImport(string filePath);
	void addAniLayerInfo(Cc3dAniLayerInfo*aniLayerInfo){
		m_aniLayerInfoList.push_back(aniLayerInfo);
		aniLayerInfo->retain();
	}
	Cc3dAniLayerInfo*getAniLayerInfoByIndex(int index){
		assert(index>=0&&index<(int)m_aniLayerInfoList.size());
		return m_aniLayerInfoList[index];
	}
	void update(float dt){
	//	cout<<"dt:"<<dt<<endl;
		if(m_interval>0){
			playAnimation(dt);
		}
		

	}
	void playAnimation(float dt){
		if((int)m_aniLayerInfoList.size()==0)return;
		//update current aniLayerInfo
		Cc3dAniLayerInfo*curAniLayerInfo=m_aniLayerInfoList[m_aniLayerIndex];
		curAniLayerInfo->progress(dt);
		//calculate animation frame index
		float time = curAniLayerInfo->getCurTime();
		//deform
		deform(m_aniLayerIndex,time);
        this->submitPosition(GL_DYNAMIC_DRAW);
        this->submitNormal(GL_DYNAMIC_DRAW);
		/////this->submitVertex(GL_DYNAMIC_DRAW);//only need to submitVertex (actually only pos and norm)
	}
	void setAniLayerIndex(int aniLayerIndex){m_aniLayerIndex=aniLayerIndex;}
	int getAniLayerIndex()const{return m_aniLayerIndex;}
	void setInterval(float interval){m_interval=interval;}
	float getInterval()const{return m_interval;}
	Cc3dSkinMesh* findSkinMeshByFbxMeshPtr(void*fbxMeshPtr){
		int meshCount=this->getMeshCount();
		for(int i=0;i<meshCount;i++){
			Cc3dSkinMesh*mesh=(Cc3dSkinMesh*)this->getMeshByIndex(i);
			if(mesh->getFbxMeshPtr()==fbxMeshPtr){
				return mesh;
			}
		}
		return NULL;
	
	}
	void deform(int aniLayerIndex,float time){
		int nMeshCount=this->getMeshCount();
		for(int i=0;i<nMeshCount;i++){
			((Cc3dSkinMesh*)this->getMeshByIndex(i))->deform(aniLayerIndex,time);
		}
	}

};
#endif