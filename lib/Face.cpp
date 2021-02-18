#include "../include/Face.h"
#include "../include/Coordinates.hpp"
#include "../include/Size.hpp"

using namespace std;

Face::Face() : coordinates(), size(), face_depth() {};
Face::Face(Coordinates coordinates_, Size size_, int face_depth_)
            :  coordinates(coordinates_), size(size_), face_depth(face_depth_) {};

void Face::setCoordinates(Coordinates coordinates_) {
    this->coordinates.set_x1(coordinates_.x1());
    this->coordinates.set_x2(coordinates_.x2());
    this->coordinates.set_y1(coordinates_.y1());
    this->coordinates.set_y2(coordinates_.y2());
};

const Coordinates& Face::getCoordinates() const {
    return this->coordinates;
};

void Face::setSize(Size size) {
    this->size.setHeight(size.height());
    this->size.setWidth(size.width());
};

const Size& Face::getSize() const {
    return this->size;
};

void Face::setFaceDepth(int face_depth_) {
    this->face_depth = face_depth_;
};

int Face::faceDepth() const {
    return this->face_depth;
};