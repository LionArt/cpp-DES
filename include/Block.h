#ifndef BLOCK_H
#define BLOCK_H

#include <vector>

using namespace std;

class Block
{
    public:
        Block(vector <char> buff);
        bool isEmpty();
        unsigned int size();
        ~Block();
        vector <char> getContent();
        void setContent(vector <char> content);
        void copyContent(Block block);
    private:
    vector <char> content;
};

#endif // BLOCK_H
