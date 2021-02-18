#ifndef _COORDINATES_H
#define _COORDINATES_H

class Coordinates {
    private:
        int x1_coord;
        int x2_coord;
        int y1_coord;
        int y2_coord;

    public:
        Coordinates() : x1_coord(0), x2_coord(0), y1_coord(0), y2_coord(0) {};
        Coordinates(int x1_,int x2_, int y1_, int y2_)
                    : x1_coord(x1_), x2_coord(x2_), y1_coord(y1_), y2_coord(y2_) {};

        inline void set_x1(int x1_) { this->x1_coord = x1_; };
        inline int x1() const { return this->x1_coord; };

        inline void set_x2(int x2_) { this->x2_coord = x2_; };
        inline int x2() const { return this->x2_coord; };

        inline void set_y1(int y1_) { this->y1_coord = y1_; };
        inline int y1() const { return this->y1_coord; };

        inline void set_y2(int y2_) { this->y2_coord = y2_; };
        inline int y2() const { return this->y2_coord; };
};

#endif