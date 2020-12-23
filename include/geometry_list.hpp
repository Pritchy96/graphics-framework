#ifndef GEOMETRYLIST_HPP
#define GEOMETRYLIST_HPP

#include <iterator>
#include <vector>
#include <memory>

#include "viewport.hpp"
#include "geometry.hpp"

    using namespace std;

    class GeometryList {    
        public:
            GeometryList(shared_ptr<vector<shared_ptr<Viewport>>> viewport_list) : viewports(viewport_list) {};
            ~GeometryList() {}
            vector<shared_ptr<Geometry>> geometry_list; 

            void push_back(shared_ptr<Geometry> geometry) {
                geometry_list.push_back(geometry);

                for (shared_ptr<Viewport> v : (*viewports)) {
                    v->geoRenderablePairs.push_back(make_pair(geometry, nullptr));
                }
            }
            
            void erase(vector<shared_ptr<Geometry>>::iterator position) {
                (*position)->isDead = true;
                geometry_list.erase(position);
            }

            void erase(int index) {
                vector<shared_ptr<Geometry>>::iterator iter = geometry_list.begin() + index;
                (*iter)->isDead = true;
                geometry_list.erase(iter);
            }

            vector<shared_ptr<Geometry>>::iterator begin() { return geometry_list.begin(); }
            vector<shared_ptr<Geometry>>::iterator end() { return geometry_list.end(); }

        private:    
            shared_ptr<vector<shared_ptr<Viewport>>> viewports;
    };


    

#endif


