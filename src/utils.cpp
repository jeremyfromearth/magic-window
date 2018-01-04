#include "MagicWindow/utils.h"

using namespace std;
using namespace boost;
using namespace ci;
using namespace ci::app;
using namespace ci::fs;
using namespace magicwindow;

////////////////////////////////////////////////////////////////////////////
// json
////////////////////////////////////////////////////////////////////////////

Color json::rgb(const ci::JsonTree & data) {
  return Color(
    data.getChild("r").getValue<float>(),
    data.getChild("g").getValue<float>(),
    data.getChild("b").getValue<float>()
  );
}

ColorA json::rgba(const ci::JsonTree & data) {
  return ColorA(
    data.getChild("r").getValue<float>(),
    data.getChild("g").getValue<float>(),
    data.getChild("b").getValue<float>(),
    data.getChild("a").getValue<float>()
  );
}

ci::vec2 json::vec2(const JsonTree & data, string x, string y) {
  return ci::vec2(
    data.getChild(x).getValue<float>(),
    data.getChild(y).getValue<float>()
  );
}


ci::vec3 json::vec3(const JsonTree & data, string x, string y, string z) {
  return ci::vec3(
    data.getChild(x).getValue<float>(),
    data.getChild(y).getValue<float>(),
    data.getChild(z).getValue<float>()
  );
}


JsonTree json::merge_trees(vector<JsonTree> & trees) {
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
