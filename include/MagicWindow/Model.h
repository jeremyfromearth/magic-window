#pragma once

// cinder
#include "cinder/Json.h"

/**
 * A basic model class
 */
namespace magicwindow {
class Model {

public:
    /**
     * Ctr
     */
    Model(ci::JsonTree & data);
    
    /**
     * Ctr
     * Provides a second parameter for id key other than the default "id"
     */
    Model(ci::JsonTree & data, std::string idKey);

    /**
     * Ctr
     * Constructor without need for json object and default id property
     */
    Model(int id);

    /**
     * Ctr
     * Constructor with no id parameter, automatically generates id
     */
    Model();

    /**
     * Returns the model id
     */
    int getId() { return uid; }
    
    /**
     * Returns the raw json object
     */
    ci::JsonTree & getRawData();
    
protected:
    int uid;
    ci::JsonTree raw;
    static int newId() { static int id = 0; return ++id; }
};
}