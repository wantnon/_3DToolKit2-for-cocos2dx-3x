#include "c3dSkinActor.h"
#include "c3dScanf.h"
void Cc3dAniLayer::doImport(string filePath,bool valueOnly){
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
    string folderFullPath=CCFileUtils::sharedFileUtils()->fullPathForFilename(folderPath);//Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
    
    bool isFolderExist=CCFileUtils::sharedFileUtils()->isDirectoryExist(folderFullPath);//Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
  if (isFolderExist){
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dAniLayer.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"r");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}

		//------------------read file
		//	cout<<"hi"<<endl;
		assert(m_aniFrameList.empty());
		int t_nAniFrame;
		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%i",&t_nAniFrame);
		if(!valueOnly)assert(string(tCharBuffer)=="m_aniFrameList.size");
		for(int i=0;i<t_nAniFrame;i++){
			//		cout<<"i="<<i<<endl;
			fskipOneStr(fp,valueOnly);
			if(!valueOnly)assert(string(tCharBuffer)=="aniFrame");
			float time;
			fskipOneStr(fp,valueOnly);
			if(!valueOnly)assert(string(tCharBuffer)=="m_time");
			fscanf(fp,"%f",&time);
			float m[16];
			fskipOneStr(fp,valueOnly);
			if(!valueOnly)assert(string(tCharBuffer)=="m_vertexTransformMat");
			for(int j=0;j<16;j++){
				fscanf(fp,"%f",&m[j]);
			}
			Cc3dMatrix4 vertexTransformMat(m);
			Cc3dAniFrame aniFrame(vertexTransformMat,time);
			m_aniFrameList.push_back(aniFrame);

		}
		//	cout<<"ha"<<endl;
		//--------------------------close file
		fclose(fp);

	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}

}
void Cc3dSkinCluster::doImport(string filePath,bool valueOnly){
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
    string folderFullPath=CCFileUtils::sharedFileUtils()->fullPathForFilename(folderPath);//Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
    
    bool isFolderExist=CCFileUtils::sharedFileUtils()->isDirectoryExist(folderFullPath);//Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
    if (isFolderExist){
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkinCluster.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"r");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}

		//------------------read file
		assert(m_vertexIndexList.empty());
		int t_nVertexIndex;
		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%i",&t_nVertexIndex);
		if(!valueOnly)assert(string(tCharBuffer)=="m_vertexIndexList.size");
		for(int i=0;i<t_nVertexIndex;i++){
			int vertexIndex;
			fscanf(fp,"%i",&vertexIndex);
			m_vertexIndexList.push_back(vertexIndex);
		}
		assert(m_vertexWeightList.empty());
		int t_nVertexWeight;
		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%i",&t_nVertexWeight);
		if(!valueOnly)assert(string(tCharBuffer)=="m_vertexWeightList.size");
		for(int i=0;i<t_nVertexWeight;i++){
			float vertexWeight;
			fscanf(fp,"%f ",&vertexWeight);
			m_vertexWeightList.push_back(vertexWeight);
		}
		int t_nAniLayer;
		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%i",&t_nAniLayer);
		if(!valueOnly)assert(string(tCharBuffer)=="m_aniLayerList.size");

		//--------------------------close file
		fclose(fp);
		//------------------m_aniLayerList
		assert(m_aniLayerList.empty());
		for(int i=0;i<t_nAniLayer;i++){
			string numStr=numberToStr(i);
			Cc3dAniLayer*p=new Cc3dAniLayer();
			p->autorelease();
			this->addAniLayer(p);
			p->doImport(folderPath+"/m_aniLayer_"+numStr,valueOnly);	
		}
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}
}

void Cc3dSkin::doImport(string filePath,bool valueOnly){
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
    string folderFullPath=CCFileUtils::sharedFileUtils()->fullPathForFilename(folderPath);//Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
    
    bool isFolderExist=CCFileUtils::sharedFileUtils()->isDirectoryExist(folderFullPath);//Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
  if (isFolderExist){
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkin.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"r");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}

		//------------------read file

		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%i",&m_skinType);
		if(!valueOnly)assert(string(tCharBuffer)=="m_skinType");

		fskipOneStr(fp,valueOnly);
		int t_nCluster;
		fscanf(fp,"%i",&t_nCluster);
		if(!valueOnly)assert(string(tCharBuffer)=="m_clusterList.size");

		//--------------------------close file
		fclose(fp);
		//------------------m_clusterList
		assert(m_clusterList.empty());
		for(int i=0;i<t_nCluster;i++){
			string numStr=numberToStr(i);
			Cc3dSkinCluster* cluster=new Cc3dSkinCluster();
			cluster->autorelease();
			addCluster(cluster);
			cluster->doImport(folderPath+"/m_skinCluster_"+numStr,valueOnly);	
		}
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}
}
void Cc3dSkinSubMeshData::doImport(string filePath,bool valueOnly){
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
    string folderFullPath=CCFileUtils::sharedFileUtils()->fullPathForFilename(folderPath);//Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
    
    bool isFolderExist=CCFileUtils::sharedFileUtils()->isDirectoryExist(folderFullPath);//Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
   if (isFolderExist){
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkinSubMeshData.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"r");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}

		//------------------read file
		//vlist
		//assert(vlist.empty());
        assert(m_positionList.empty());
        assert(m_texCoordList.empty());
        assert(m_colorList.empty());
        assert(m_normalList.empty());
		int t_nv;
		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="m_vlist.size");
		fscanf(fp,"%i",&t_nv);
		for(int i=0;i<t_nv;i++){
			fskipOneStr(fp,valueOnly);
			if(!valueOnly)assert(string(tCharBuffer)=="vertex");
			Cc3dVector4 pos;
			Cc3dVector2 texCoord;
			Cc3dVector4 norm;
			Cc3dVector4 color;
			float posx,posy,posz;
			float s,t;
			float normx,normy,normz;
			float r,g,b,a;
			fskipOneStr(fp,valueOnly);
			fscanf(fp,"%f",&posx);
			fscanf(fp,"%f",&posy);
			fscanf(fp,"%f",&posz);
			fskipOneStr(fp,valueOnly);
			fscanf(fp,"%f",&s);
			fscanf(fp,"%f",&t);
			fskipOneStr(fp,valueOnly);
			fscanf(fp,"%f",&normx);
			fscanf(fp,"%f",&normy);
			fscanf(fp,"%f",&normz);
			fskipOneStr(fp,valueOnly);
			fscanf(fp,"%f",&r);
			fscanf(fp,"%f",&g);
			fscanf(fp,"%f",&b);
			fscanf(fp,"%f",&a);
			pos.init(posx,posy,posz,1);
			texCoord.init(s,t);
			norm.init(normx,normy,normz,0);
			color.init(r,g,b,a);
			Cc3dVertex vertex(pos,texCoord,norm,color);
			////vlist.push_back(vertex);
            m_positionList.push_back(pos);
            m_texCoordList.push_back(texCoord);
            m_normalList.push_back(norm);
            m_colorList.push_back(color);
		}
		assert(m_IDtriList.empty());
		int t_nIDtri;
		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="m_IDtriList.size");
		fscanf(fp,"%i",&t_nIDtri);
		for(int i=0;i<t_nIDtri;i++){
			fskipOneStr(fp,valueOnly);
			if(!valueOnly)assert(string(tCharBuffer)=="IDtri");
			int vID0,vID1,vID2;
			fscanf(fp,"%i",&vID0);
			fscanf(fp,"%i",&vID1);
			fscanf(fp,"%i",&vID2);
			Cc3dIDTriangle IDtri(vID0,vID1,vID2);
			m_IDtriList.push_back(IDtri);

		}

		//--------------------------close file
		fclose(fp);
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}
}


void Cc3dSkinSubMesh::doImport(string filePath,bool valueOnly){
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
    string folderFullPath=CCFileUtils::sharedFileUtils()->fullPathForFilename(folderPath);//Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
    
    bool isFolderExist=CCFileUtils::sharedFileUtils()->isDirectoryExist(folderFullPath);//Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
   if (isFolderExist){
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkinSubMesh.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"r");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}

		//------------------read file
		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="texFilePath");
		char t_texFilePath[1024];
		fscanf(fp,"%s",t_texFilePath);
		string texFilePath=string(t_texFilePath);
		Cc3dTexture*texture=Cc3dTextureCache::sharedTextureCache()->addImage(texFilePath.c_str());
		this->setTexture(texture);

		Cc3dVector4 ambient;
		Cc3dVector4 diffuse;
		Cc3dVector4 specular;
		float shininess;

		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="ambient");
		ambient=fscanVector4(fp);

		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="diffuse");
		diffuse=fscanVector4(fp);

		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="specular");
		specular=fscanVector4(fp);

		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="shininess");
		fscanf(fp,"%f",&shininess);

		Cc3dMaterial*material=new Cc3dMaterial();
		material->autorelease();
		material->setAmbient(ambient);
		material->setDiffuse(diffuse);
		material->setSpecular(specular);
		material->setShininess(shininess);
		this->setMaterial(material);

		//--------------------------close file
		fclose(fp);
		//------------------m_subMeshData_backup
		assert(m_subMeshData_backup==NULL);
		m_subMeshData_backup=new Cc3dSkinSubMeshData();
		m_subMeshData_backup->autorelease();
		m_subMeshData_backup->retain();
		m_subMeshData_backup->doImport(folderPath+"/m_subMeshData_backup",valueOnly);
		//------------------m_subMeshData
		Cc3dSkinSubMeshData* subMeshData=new Cc3dSkinSubMeshData();
		subMeshData->autorelease();
		this->setSubMeshData(subMeshData);//wwwwwwwwwwwwwwwwwwwwwwww
		*subMeshData=*m_subMeshData_backup;
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}
}


void Cc3dSkinMesh::doImport(string filePath,bool valueOnly){
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
    string folderFullPath=CCFileUtils::sharedFileUtils()->fullPathForFilename(folderPath);//Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
    
    bool isFolderExist=CCFileUtils::sharedFileUtils()->isDirectoryExist(folderFullPath);//Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
   if (isFolderExist){
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkinMesh.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"r");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}
		//------------------read file

		fskipOneStr(fp,valueOnly);
		int t_nSkinSubMesh;
		fscanf(fp,"%i",&t_nSkinSubMesh);
		if(!valueOnly)assert(string(tCharBuffer)=="m_skinSubMesh.size");

		fskipOneStr(fp,valueOnly);
		int t_nVertexDup;
		fscanf(fp,"%i",&t_nVertexDup);
		if(!valueOnly)assert(string(tCharBuffer)=="m_vertexDupList.size");
		//m_vertexDupList
		assert(m_vertexDupList.empty());
		for(int i=0;i<t_nVertexDup;i++){
			vector<_CmeshIDvID> meshIDvIDList;
			int t_nMeshIDvID;
			fskipOneStr(fp,valueOnly);
			fscanf(fp,"%i",&t_nMeshIDvID);
			if(!valueOnly)assert(string(tCharBuffer)=="nMeshIDvID");
			for(int j=0;j<t_nMeshIDvID;j++){
				_CmeshIDvID meshIDvID;
				int meshID,vID;
				fskipOneStr(fp,valueOnly);
				fscanf(fp,"%i",&meshID);
				fscanf(fp,"%i",&vID);
				if(!valueOnly)assert(string(tCharBuffer)=="meshIDvID");
				meshIDvID.init(meshID,vID);
				meshIDvIDList.push_back(meshIDvID);
			}
			m_vertexDupList.push_back(meshIDvIDList);
		}
		//RTmat and Scale
		float m[16];
		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="RTmat");
		for(int j=0;j<16;j++){
			fscanf(fp,"%f",&m[j]);
		}
		Cc3dMatrix4 RTmat(m);
		float scaleX,scaleY,scaleZ;
		fskipOneStr(fp,valueOnly);
		if(!valueOnly)assert(string(tCharBuffer)=="scale");
		fscanf(fp,"%f",&scaleX);
		fscanf(fp,"%f",&scaleY);
		fscanf(fp,"%f",&scaleZ);
		////Cc3dTransform*transform=new Cc3dTransform();
        ////transform->autorelease();
		////transform->init(RTmat,scaleX,scaleY,scaleZ);
		////this->setTransform(transform);
        this->setTransformC3D(RTmat);
        this->setScaleC3D(scaleX, scaleY, scaleZ);
		//--------------------------close file
		fclose(fp);
		//------------------m_skin
       string fullPathOfm_skin=CCFileUtils::sharedFileUtils()->fullPathForFilename(folderPath+"/m_skin");
        if(CCFileUtils::sharedFileUtils()->isDirectoryExist(fullPathOfm_skin)){//Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath+"/m_skin")){
            Cc3dSkin*skin=new Cc3dSkin();
            skin->autorelease();
            setSkin(skin);
            skin->doImport(folderPath+"/m_skin",valueOnly);
        }
		//------------------skinSubMeshes
		assert(this->getSubMeshCount()==0);
		for(int i=0;i<t_nSkinSubMesh;i++){
			string numStr=numberToStr(i);
			Cc3dSkinSubMesh*p=new Cc3dSkinSubMesh();
			p->autorelease();
			this->addSubMesh(p);
			p->doImport(folderPath+"/m_skinSubMesh_"+numStr,valueOnly);
		}
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}
}

void Cc3dAniLayerInfo::doImport(string filePath,bool valueOnly){

	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
    string folderFullPath=CCFileUtils::sharedFileUtils()->fullPathForFilename(folderPath);//Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
    
    bool isFolderExist=CCFileUtils::sharedFileUtils()->isDirectoryExist(folderFullPath);//Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
  if (isFolderExist){
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dAniLayerInfo.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"r");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}

		//------------------read file

		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%f",&m_interval);
		if(!valueOnly)assert(string(tCharBuffer)=="m_interval");

		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%f",&m_startTime);
		if(!valueOnly)assert(string(tCharBuffer)=="m_startTime");

		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%f",&m_endTime);
		if(!valueOnly)assert(string(tCharBuffer)=="m_endTime");


		m_curTime=0;

		//--------------------------close file
		fclose(fp);
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}

}

void Cc3dSkinActor::doImport(string filePath){
	//--------------use filePath as folder path
	const string folderPath=filePath;
	//-----------------------------------------
    string folderFullPath=CCFileUtils::sharedFileUtils()->fullPathForFilename(folderPath);//Cc3dFileUtils::sharedFileUtils()->getFullPath(folderPath);
    
    bool isFolderExist=CCFileUtils::sharedFileUtils()->isDirectoryExist(folderFullPath);//Cc3dFileUtils::sharedFileUtils()->getIsFileOrClipExist(folderPath);
   if (isFolderExist){
		//------------------full path of file
		string fileFullPath=folderFullPath+"/Cc3dSkinActor.txt";
		//open file
		FILE  *fp=NULL;
		fp=fopen(fileFullPath.c_str(),"r");//override write
		if(fp==NULL){
			cout<<"Open "<<fileFullPath<<" failed!"<<endl;
			assert(false);
		}

		//------------------read file
		bool valueOnly;

		char t_str[1024];
		fscanf(fp,"%s",t_str);
		string str=t_str;
		if(str=="valueOnly"){
			valueOnly=true;
		}else if(str=="notValueOnly"){
			valueOnly=false;
		}else{
			assert(false);
		}
		cout<<"valueOnly:"<<valueOnly<<endl;

		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%i",&m_aniLayerIndex);

		fskipOneStr(fp,valueOnly);
		fscanf(fp,"%f",&m_interval);

		fskipOneStr(fp,valueOnly);
		int t_nAniLayerInfo;
		fscanf(fp,"%i",&t_nAniLayerInfo);

		fskipOneStr(fp,valueOnly);
		int t_nSkinMesh;
		fscanf(fp,"%i",&t_nSkinMesh);

		//--------------------------close file
		fclose(fp);
		//------------------m_aniLayerInfoList
		assert(m_aniLayerInfoList.empty());
		for(int i=0;i<t_nAniLayerInfo;i++){
			string numStr=numberToStr(i);
			Cc3dAniLayerInfo*p=new Cc3dAniLayerInfo();
			p->autorelease();
			this->addAniLayerInfo(p);
			p->doImport(folderPath+"/m_aniLayerInfo_"+numStr,valueOnly);
		}
		//------------------skinMeshes
		assert(this->getMeshCount()==0);
		for(int i=0;i<t_nSkinMesh;i++){
			string numStr=numberToStr(i);
			Cc3dSkinMesh*p=new Cc3dSkinMesh();
			p->autorelease();
			this->addMesh(p);
			p->doImport(folderPath+"/m_skinMesh_"+numStr,valueOnly);
		}
	}else{
		cout<<string("")+"warning: create path: "+folderFullPath+ " failed!"<<endl;
		assert(false);
	}

}