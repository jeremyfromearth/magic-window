#include "MagicWindow/Model.h"
#include "cinder/app/App.h"
using namespace magicwindow;

Model::Model(ci::JsonTree & data) : raw(data) {
    uid = data.getChild("id").getValue<int>();
}

Model::Model(ci::JsonTree & data, std::string idKey) : raw(data) {
    uid = data.getChild(idKey).getValue<int>();
}

Model::Model(int id) : uid(id){}

Model::Model() {
     uid = newId();
 }

ci::JsonTree & Model::getRawData() { return raw; }