#ifndef GEOMETRYLIST_HPP
#define GEOMETRYLIST_HPP

#include <iterator>
#include <vector>
#include <memory>
    
#include "graphics-framework/window_handler.hpp"
#include "graphics-framework/viewport.hpp"
#include "graphics-framework/geometry.hpp"

    class GeometryList {
        public:
            explicit GeometryList(std::shared_ptr<std::vector<std::shared_ptr<WindowHandler>>> window_handler_list) : window_handlers_(window_handler_list) {};
            ~GeometryList() = default;
            std::vector<std::shared_ptr<Geometry>> geometry_list;

            void push_back(std::shared_ptr<Geometry> geometry) { //NOLINT: lower case method name to match std::vector method.
                geometry_list.push_back(geometry);
            
                for (const std::shared_ptr<WindowHandler>& w : *window_handlers_) {
                    for (const std::shared_ptr<Viewport>& v : w->viewports) {
                        v->geo_renderable_pairs.emplace_back(geometry, nullptr);
                    }
                }
            }
            
            void erase(std::vector<std::shared_ptr<Geometry>>::iterator position) { //NOLINT: lower case method name to match std::vector method.
                (*position)->is_dead = true;
                geometry_list.erase(position);
            }

            void erase(int index) { //NOLINT: lower case method name to match std::vector method.
                auto iter = geometry_list.begin() + index;
                (*iter)->is_dead = true;
                geometry_list.erase(iter);
            }

            std::vector<std::shared_ptr<Geometry>>::iterator begin() { return geometry_list.begin(); }    //NOLINT: lower case method name to match std::vector method.
            std::vector<std::shared_ptr<Geometry>>::iterator end() { return geometry_list.end(); }    //NOLINT: lower case method name to match std::vector method.

        private:    
            std::shared_ptr<std::vector<std::shared_ptr<WindowHandler>>> window_handlers_;
    };

#endif
