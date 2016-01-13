#pragma once

#include <stdio.h>

// boost
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>


// cinder
#include "cinder/Buffer.h"
#include "cinder/Json.h"
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"

namespace magicwindow {
    class json {
    public:
        ///////////////////////////////////////////////////////////////
        // static methods
        ///////////////////////////////////////////////////////////////
        /**
         * Get a value from a JsonTree
         * @param tree - The JsonTree to get the data from
         * @param path - A string representation of the path to node containing the desired data
         * @param defaultValue - A default value, returned if the node is not defined
         * @return a value of type T from a JsonTree
         */
        template <typename T>
        static T get(const ci::JsonTree & tree, const std::string & path, T defaultValue, std::string separators = "/.:|") {
            std::vector<std::string> parts;
            boost::split(parts, path, boost::is_any_of(separators));
            if (parts.size() && tree.hasChild(parts[0])) {
                int i = 0;
                ci::JsonTree child = tree;
                while (i < parts.size()) {
                    if (child.hasChild(parts[i])) {
                        ci::JsonTree c = child.getChild(parts[i]);
                        child = c;
                        i++;
                    } else {
                        return defaultValue;
                    }
                }
                return child.getValue<T>();
            }
            return defaultValue;
        }
        
        /**
         * Returns an RGB color
         * Supplied data should be formatted as such: {r: 1.0, g: 1.0, b: 1.0}
         */
        static ci::Color getColor(const ci::JsonTree & data);
        
        /**
         * Returns an RGBA color
         * Supplide data should be formatted as such: {r: 1.0, g: 1.0, b: 1.0, a: 1.0}
         */
        static ci::ColorA getColorA(const ci::JsonTree & data);

        /**
         * Takes a JsonTree containing x, y coordinate data and returns a Vec2f
         * @param data - A JsonTree with the following format {x : 0, y : 0}
         * @param x - The key for the x value (defaults to x)
         * @param y - The key for the y value (defaults to y)
         */
        static ci::vec2 getVec2(const ci::JsonTree & data, std::string x = "x", std::string y = "y");

        /**
        * Takes a JsonTree containing x, y, z coordinate data and returns a Vec3f
        * @param data - A JsonTree with the following format {x : 0, y : 0, z : 0}
        * @param x - The key for the x value (defaults to x)
        * @param y - The key for the y value (defaults to y)
        * @param z - The key for the z value (defaults to z)
        */
        static ci::vec3 getVec3(const ci::JsonTree & data, std::string x = "x", std::string y = "y", std::string z = "z");

        /**
        * Merges json multiple trees into one tree
        * @param trees - A vector of JsonTree objects to be merged
        * @return A single JsonTree consisting of the supplied trees
        */
        static ci::JsonTree mergeTrees(std::vector<ci::JsonTree> & trees);
    };
    
    class loader {
    public:
        /**
         * Blocking download method. Downloads any file from a valid url to the specified destination. 
         * Replaces existing files of the same name
         * @param src - The path to load the file from
         * @param destination - The path to save the file to
         */
        static void download(ci::fs::path src, ci::fs::path destination);
    };
}
