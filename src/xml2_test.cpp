#include "tinyxml2.h"
#include  <iostream>

using namespace tinyxml2;
using namespace std;

//param：xmlPath:xml文件路径
//return:0,成功，非0，失败
int createXML(const char* xmlPath){
	const char* declaration ="<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
	XMLDocument doc;
	doc.Parse(declaration);//会覆盖xml所有内容	
	//添加申明可以使用如下两行
	//XMLDeclaration* declaration=doc.NewDeclaration();
	//doc.InsertFirstChild(declaration);
	XMLElement* root=doc.NewElement("DBUSER");
	doc.InsertEndChild(root);	
	return doc.SaveFile(xmlPath); // XML_SUCCESS = 0,
}


//用户类
class User{
	public:
		User(){
			gender=0;
		};
		
		User(const string& userName, const string& password, int gender, const string& mobile, const string& email){
			this->userName=userName;
			this->password=password;
			this->gender=gender;
			this->mobile=mobile;
			this->email=email;
		};
		
		string userName;
		string password;
		int gender;
		string mobile;
		string email;
};


int insertXMLNode(const char* xmlPath,const User& user){
	XMLDocument doc;
	int res=doc.LoadFile(xmlPath);
	if(res!=0){
		cout<<"load xml file failed"<<endl;
		return res;
	}

	XMLElement* root=doc.RootElement();
	
	XMLElement* userNode = doc.NewElement("User");
	userNode->SetAttribute("Name",user.userName.c_str());
	userNode->SetAttribute("Password ",user.password.c_str());
	root->InsertEndChild(userNode);
	
	XMLElement* gender = doc.NewElement("Gender");
	XMLText* genderText=doc.NewText( ( to_string(user.gender)  ).c_str() )  ;
	gender->InsertEndChild(genderText);
	userNode->InsertEndChild(gender);
					
	XMLElement* mobile = doc.NewElement("Mobile");
	mobile->InsertEndChild(doc.NewText(user.mobile.c_str()));
	userNode->InsertEndChild(mobile);
	
	XMLElement* email = doc.NewElement("Email");
	email->InsertEndChild(doc.NewText(user.email.c_str()));
	userNode->InsertEndChild(email);
	
	return doc.SaveFile(xmlPath);
}




// 查询XML文件的指定节点
XMLElement* queryUserNodeByName(XMLElement* root,const string& userName){	
	XMLElement* userNode=root->FirstChildElement("User");
	while(userNode!=NULL){
		if(userNode->Attribute("Name")==userName)
			break;
		userNode=userNode->NextSiblingElement();//下一个兄弟节点
	}
	return userNode;
}



// 获取用户信息的函数
User* queryUserByName(const char* xmlPath,const string& userName){
	XMLDocument doc;
	if(doc.LoadFile(xmlPath)!=0){
		cout<<"load xml file failed"<<endl;
		return NULL;
	}
	XMLElement* root=doc.RootElement();
	XMLElement* userNode=queryUserNodeByName(root,userName);
	
	if(userNode!=NULL){  //searched successfully
		User* user=new User();
		user->userName=userName;
		user->password=userNode->Attribute("Password");
		XMLElement* genderNode=userNode->FirstChildElement("Gender");
		user->gender=atoi(genderNode->GetText());
		XMLElement* mobileNode=userNode->FirstChildElement("Mobile");
		user->mobile=mobileNode->GetText();		
		XMLElement* emailNode=userNode->FirstChildElement("Email");
		user->email=emailNode->GetText();			
		return user;
	}
	return NULL;
}



// 修改XML文件的指定节点
//function:修改指定节点内容
//param:xmlPath:xml文件路径；user：用户对象
//return：bool
bool updateUser(const char* xmlPath,User* user){
	XMLDocument doc;
	if(doc.LoadFile(xmlPath)!=0){
		cout<<"load xml file failed"<<endl;
		return false;
	}
	XMLElement* root=doc.RootElement();
	XMLElement* userNode=queryUserNodeByName(root,user->userName);
	
	if(userNode!=NULL){
		if(user->password!=userNode->Attribute("Password"))
		{
			userNode->SetAttribute("Password",user->password.c_str());  //修改属性
		}
		XMLElement* genderNode=userNode->FirstChildElement("Gender");
		if(user->gender!=atoi(genderNode->GetText()))  
		{
			genderNode->SetText(( to_string(user->gender)  ).c_str());   //修改节点内容
		}
		XMLElement* mobileNode=userNode->FirstChildElement("Mobile");
		if(user->mobile!=mobileNode->GetText())
		{
			mobileNode->SetText(user->mobile.c_str());
		}
		XMLElement* emailNode=userNode->FirstChildElement("Email");
		if(user->email!=emailNode->GetText())
		{
			emailNode->SetText(user->email.c_str());
		}
		if(doc.SaveFile(xmlPath)==0)
			return true;
	}
	return false;
}






//function:删除指定节点内容
//param:xmlPath:xml文件路径；userName：用户名称
//return：bool
bool deleteUserByName(const char* xmlPath,const string& userName)
{
	XMLDocument doc;
	if(doc.LoadFile(xmlPath)!=0)
	{
		cout<<"load xml file failed"<<endl;
		return false;
	}
	XMLElement* root=doc.RootElement();
	//doc.DeleteNode(root);//删除xml所有节点
	XMLElement* userNode=queryUserNodeByName(root,userName);
	if(userNode!=NULL)
	{
		userNode->DeleteAttribute("Password");//删除属性
		XMLElement* emailNode=userNode->FirstChildElement("Email");
		userNode->DeleteChild(emailNode); //删除指定节点
		//userNode->DeleteChildren();//删除节点的所有孩子节点
		if(doc.SaveFile(xmlPath)==0)
			return true;
	}
	return false;
}



// 获取XML文件申明
//function:获取xml文件申明
//param:xmlPath:xml文件路径；strDecl：xml申明
//return：bool
bool getXMLDeclaration(const char* xmlPath,string& strDecl)
{
	XMLDocument doc;
	if(doc.LoadFile(xmlPath)!=0)
	{
		cout<<"load xml file failed"<<endl;
		return false;
	}
	XMLNode* decl=doc.FirstChild();  
	if (NULL!=decl)  
    {  
        XMLDeclaration* declaration =decl->ToDeclaration();  
        if (NULL!=declaration)  
        {  
              strDecl = declaration->Value();
			  return true;
        } 
    }
   	return false;
}




int main(int argc,char* argv[]){
	
	// create_xml
   int createXML_=createXML("./user.xml");
   if(createXML_==0){
	   cout<<"create xml success"<<endl;
   }else{
	   cout<<"create xml failed"<<endl;
   }

 

  	// insert_xml
	User user_("lvlv","00001111",0,"13995648666","1586666@qq.com");
   int inert_=insertXMLNode("./user.xml",user_);
   if(inert_==0){
	   cout<<"insert xml success"<<endl;
   }else{
	   cout<<"insert xml failed"<<endl;
   }


// //修改用户信息
	User user_1("lvlv","00001111",0,"13995648666","22222222@qq.com");
	bool updateUser_=updateUser("./user.xml",&user_1);
    if(updateUser_==true){
		cout<<"updateUser success"<<endl;
	}else{
		cout<<"updateUser failed"<<endl;
	}


// // 获取用户信息的函数
   std::string user_name="lvlv";
	User* user=queryUserByName("./user.xml",user_name);
    cout<<"user  "<<user<<endl;


// 删除XML文件的指定节点的信息
  	//删除用户某些信息
	if(deleteUserByName("./user.xml","lvlv")){
		cout<<"delete successfully"<<endl;}
	else{
		cout<<"delete failed"<<endl;
	}




//获取xml文件申明
	string strDecl;
	if(getXMLDeclaration("./user.xml",strDecl))
	{
		cout<<"declaration:"<<strDecl<<endl;
	}



	return 0;
}