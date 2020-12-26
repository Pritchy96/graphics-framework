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
            explicit GeometryList(shared_ptr<vector<shared_ptr<Viewport>>> viewport_list) : viewports_(viewport_list) {};
            ~GeometryList() = default;
            vector<shared_ptr<Geometry>> geometry_list; 

            void push_back(shared_ptr<Geometry> geometry) {
                geometry_list.push_back(geometry);

                for (shared_ptr<Viewport> v : (*viewports_)) {
                    v->geo_renderable_pairs.emplace_back(geometry, nullptr);
                }
            }
            
            void erase(vector<shared_ptr<Geometry>>::iterator position) {
                (*position)->is_dead = true;
                geometry_list.erase(position);
            }

            void erase(int index) {
                auto iter = geometry_list.begin() + index;
                (*iter)->is_dead = true;
                geometry_list.erase(iter);
            }

            vector<shared_ptr<Geometry>>::iterator begin() { return geometry_list.begin(); }
            vector<shared_ptr<Geometry>>::iterator end() { return geometry_list.end(); }

        private:    
            shared_ptr<vector<shared_ptr<Viewport>>> viewports_;
    };

#endif


