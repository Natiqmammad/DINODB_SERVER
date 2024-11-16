#ifndef PERMISSIONS_H
#define PERMISSIONS_H
#include<iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
const std::string permissions_file="/home/DinoDB/dinodb/DBS/users/grands.dbs";
const std::string permissions_temp="/home/DinoDB/dinodb/DBS/users/tempgrands.dbs";
std::string grandssa[]={"ALL","SERVICE","WRITE","MODIFY","READ","CREATE","SELECT","UPDATE","INSERT","DELETE","DROP","ALTER","GRANT","SHOW"};
struct ALL{
 const std::string permissions[12]={"CREATE","SERVICE","SELECT","UPDATE","INSERT","DELETE","DROP","ALTER","GRANT","SHOW"};
};

struct WRITE{
 const std::string permissions[12]={"CREATE","SELECT","UPDATE","INSERT","ALTER","SHOW"};
};
struct MODIFY{
 const std::string permissions[12]={"CREATE","SELECT","UPDATE","INSERT","DELETE","DROP","ALTER","SHOW"};
};
struct CREAT{
 const std::string permissions[1]={"CREATE"};
};
struct SELECT{
 const std::string permissions[3]={"SELECT"};
};

struct UPDATE{
 const std::string permissions[3]={"UPDATE","SERVICE"};
};
struct INSERT{
 const std::string permissions[3]={"INSERT"};
};


struct DELETE{
 const std::string permissions[5]={"DELETE"};
 //NESE SILME
};
struct DROP{
    //YA TABLE YA DA DB SILME
 const std::string permissions[15]={"DROP"};
};


struct ALTER{
 const std::string permissions[5]={"ALTER"};
};
struct SHOW{
 const std::string permissions[5]={"SHOW"};
};
struct SERVICE{
 const std::string permissions[8]={"SERVICE"};
};

struct GRANT{
 const std::string permissions[7]={"GRANT","UPDATE","SERVICE"};
};
struct READ{
 const std::string permissions[3]={"SHOW","SELECT"};
};
#endif