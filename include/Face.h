#ifndef _FACE_H
#define _FACE_H

#include "Coordinates.hpp"
#include "Size.hpp"

using namespace std;

class Face {
    private:
        Coordinates coordinates;
        Size size;
        int face_depth;

    public:
        Face();
        Face(Coordinates coordinates_, Size size_, int face_depth_);

        // TODO
        //Face(const Face& fc);
        //Face& operator=(const Face& fc);

        void setCoordinates(Coordinates coordinates);
        const Coordinates& getCoordinates() const;

        void setSize(Size size);
        const Size& getSize() const;

        void setFaceDepth(int face_depth_);
        int faceDepth() const;

};

#endif