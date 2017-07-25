#ifndef _CELL_H_
#define _CELL_H_

class Cell {
   public:
    explicit Cell(unsigned int index, unsigned int level_);

   protected:
   private:
    unsigned int index_;
    unsigned int level_;
};

#endif  //_CELL_H_
