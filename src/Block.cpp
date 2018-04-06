#include "Block.h"

Block::Block(vector <char> buff)
{
    content=buff;
}

Block::~Block()
{

}

bool Block::isEmpty()
{
    if(content.size()>0)
    return false;
    else
    return true;
}

unsigned int Block::size()
{
    return content.size();
}

vector <char> Block::getContent()
{
    return content;
}

void Block::setContent(vector <char> c)
{
    content=c;
}

void Block::copyContent(Block b)
{
    content=b.getContent();
}
