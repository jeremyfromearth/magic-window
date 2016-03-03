#pragma once

// std
#include <map>

// cinder
#include "cinder/Json.h"

// atlas
#include "Model.h"

namespace magicwindow {
    ///////////////////////////////////////////////////////////////
    // Collection - A collection of Model obejects of type M
    ///////////////////////////////////////////////////////////////
    template <class M> class Collection {
    public:
        ///////////////////////////////////////////////////////////////
        // Ctr
        // Compile-time test that the Model type is a sub-class of Model
        ///////////////////////////////////////////////////////////////
        Collection() {
            (void)static_cast<magicwindow::Model*>((M*)0);
        }
        
        ///////////////////////////////////////////////////////////////
        // Properties
        ///////////////////////////////////////////////////////////////
        /**
         * Id of this collection
         */
        int collectionId;

        ///////////////////////////////////////////////////////////////
        // Methods
        ///////////////////////////////////////////////////////////////
        /**
         * Add a single model of type M
         */
        void add(M & model) {
            modelMap.insert(std::pair<int, int>(model.getId(), models.size()));
            models.push_back(model);
        }
        
        /**
         * Add multiple models of type M
         */
        void add(std::vector<M> & newModels) {
            for(typename std::vector<M>::iterator iter = newModels.begin(); iter!= newModels.end(); iter++) {
                add(*iter);
            }
        }

        /**
         * Returns the model at the supplied index
         */
        M & at(int index) {
            return models[index];
        }
        
        /**
         * Returns the first iterator from the list of models
         */
        typename std::vector<M>::iterator begin() {
            return models.begin();
        }

        /**
         * Empties this collection
         */
        void clear() {
            models.clear();
            modelMap.clear();
        }
        
        /**
         * Returns the last iterator from the list of models
         */
        typename std::vector<M>::iterator end() {
            return models.end();
        }
        
        /**
         * Return a list of model ids
         */
        std::vector<int> getModelIds() {
            std::vector<int> result;
            for(typename std::vector<M>::iterator iter = models.begin(); iter != models.end(); iter++){
                result.push_back(iter->getId());
            }
            return result;
        }
        
        /**
         * Data should be in a JsonTree object
         */
        void init(ci::JsonTree data){
            ci::JsonTree::Iter iter;
            for(iter = data.begin(); iter != data.end(); iter++) {
                add(M(*iter));
            }
        }
        
        /**
         * Fetch a model by its id
         */
        M & getModelById(int id) {
            int index = modelMap[id];
            return models.at(index);
        }

		/**
		 * Returns 
 		 */
		std::vector<M> & getModels() {
			return models;
		}


        /**
         * Returns the number of models in this collection
         */
        int getSize() { return models.size(); }

        /**
         * Returns a boolean indicating this collection has an object by the supplied id
         */
        bool hasModelById(int id) {
            return modelMap.find(id) != modelMap.end();
        }

        /**
         * Returns a boolean indicating that the collection is empty
         */
        bool isEmpty() { return models.empty(); }

        /**
         * Removes the element with id
         */
        void removeModelById(int id) {
            if (hasModelById(id)) { 
                int index = modelMap.at(id);
                models.erase(models.begin() + index);
                modelMap.clear();
                for (int i = 0; i < models.size(); i++) {
                    modelMap.insert(std::pair<int, int>(models[i].getId(), i));
                }
            }
        }
        
    protected:
        ///////////////////////////////////////////////////////////////
        // Properties
        ///////////////////////////////////////////////////////////////
        std::vector<M> models;
        std::map<int, int> modelMap;
    };
}