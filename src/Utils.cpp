#include "MagicWindow/Utils.h"

using namespace std;
using namespace boost;
using namespace ci;
using namespace ci::app;
using namespace ci::fs;
using namespace magicwindow;

////////////////////////////////////////////////////////////////////////////
// json
////////////////////////////////////////////////////////////////////////////

Color json::getColor(const ci::JsonTree & data) {
  return Color(data.getChild("r").getValue<float>(),
               data.getChild("g").getValue<float>(),
               data.getChild("b").getValue<float>());
}

ColorA json::getColorA(const ci::JsonTree & data) {
  return ColorA(data.getChild("r").getValue<float>(),
                data.getChild("g").getValue<float>(),
                data.getChild("b").getValue<float>(),
                data.getChild("a").getValue<float>());
}

vec2 json::getVec2(const JsonTree & data, string x, string y) {
  return vec2(data.getChild(x).getValue<float>(),
              data.getChild(y).getValue<float>());
}


vec3 json::getVec3(const JsonTree & data, string x, string y, string z) {
  return vec3(data.getChild(x).getValue<float>(),
              data.getChild(y).getValue<float>(), 
              data.getChild(z).getValue<float>());
}


JsonTree json::mergeTrees(vector<JsonTree> & trees) {
  JsonTree result = JsonTree::makeObject();
  vector<JsonTree>::iterator it;
  for (it = trees.begin(); it != trees.end(); it++) {
    JsonTree::Iter jsonIter;
    for (jsonIter = it->begin(); jsonIter != it->end(); jsonIter++) {
      result.pushBack(*jsonIter);
    }
  }
  return result;
}

////////////////////////////////////////////////////////////////////////////
// loader
////////////////////////////////////////////////////////////////////////////

void loader::download(path src, path destination) {
  BufferRef data = loadStreamBuffer(ci::loadUrlStream(src.string()));
  string tempFilename = src.filename().string() + ".tmp";
  data->write(writeFile(tempFilename));
  remove(destination.c_str());
  rename(tempFilename, destination.c_str());
}
