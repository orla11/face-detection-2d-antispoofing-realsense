#ifndef _SIZE_H
#define _SIZE_H

class Size {
    private:
        int height_param;
        int width_param;

    public:
        Size() : height_param(), width_param() {};
        Size(int height_, int width_)
                    : height_param(height_), width_param(width_) {};

        inline void setHeight(int height_) { this->height_param = height_; };
        inline int height() const { return this->height_param; };

        inline void setWidth(int width_) { this->width_param = width_; };
        inline int width() const { return this->width_param; };
};

#endif