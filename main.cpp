#include <iostream>
#include <Block.h>
#include <math.h>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
using namespace std;

enum Operation{encryption,decryption};
//Podajemy na wejscie napis
//ilosc znakow*8=bity;
//(ilosc znakow/8) w zaokragleniu do gornej liczby calkowitej (ceil)=ilosc blokow
//jesli brakuje bitow do 64, wypelniamy reszte znakiem o numerze 128 (10000000), a reszte 0 (00000000) tzw. padding

vector <Block> stringToCharArrays(string text)
{
    vector <Block> tablica;
    unsigned int num=0;
    for(unsigned int i=0;i<ceil((double)text.length()/16);i++)
    {
        vector <char> tablicaCharow;
        for(int j=0;j<16;j++)
        {
            if(num==text.length())
            {
                tablicaCharow.push_back('8');
                tablicaCharow.push_back('0');
            }
            else
            if(num>text.length())
            {
                tablicaCharow.push_back('0');
                tablicaCharow.push_back('0');
            }
            else
            {
                tablicaCharow.push_back(text[num]);
            }
            num++;
        }
        tablica.push_back(tablicaCharow);
    }
    return tablica;
}

void initPermutation (Block &block)
{
    vector <char> blockCopy,newContent;
    for(unsigned int i=0;i<block.size();i++)
    {
        blockCopy.push_back(block.getContent()[i]);
        newContent.push_back(block.getContent()[i]);
    }
    int num=0;
    int num2=58;
    for(int j=0;j<8;j++)
    {
        for(int i=0;i<8;i++)
        {
            newContent[num]=blockCopy[num2-1];
            num++;
            num2-=8;
        }
        if(num2==0)
        num2=57;
        else
        num2+=66;
    }
    block.setContent(newContent);
}


void PC1_Permutation (Block &block)
{
    vector <char> blockCopy,newContent;
    for(unsigned int i=0;i<block.size();i++)
    {
        blockCopy.push_back(block.getContent()[i]);
    }

    int num=57;
    for(int i=0;i<28;i++)
    {
        if(num<0)
        num+=65;
        newContent.push_back(blockCopy[num-1]);
        num-=8;
    }
    num=63;
    for(int i=28;i<52;i++)
    {
        if(num<0)
        num+=63;
        newContent.push_back(blockCopy[num-1]);
        num-=8;
    }
    num=28;
    for(int i=52;i<56;i++)
    {
        newContent.push_back(blockCopy[num-1]);
        num-=8;
    }
    block.setContent(newContent);
}

string resultToHex (vector <char> result)
{
    int num=0;
    string key;
    for(int i=0;i<16;i++)
    {
        int temp=0;
        if(result[num]==1)
        temp+=8;
        if(result[num+1]==1)
        temp+=4;
        if(result[num+2]==1)
        temp+=2;
        if(result[num+3]==1)
        temp+=1;

        switch (temp)
        {
            case 0:
            {
              key=key+"0";
              break;
            }
            case 1:
            {
              key=key+"1";
              break;
            }
            case 2:
            {
              key=key+"2";
              break;
            }
            case 3:
            {
              key=key+"3";
              break;
            }
            case 4:
            {
              key=key+"4";
              break;
            }
            case 5:
            {
              key=key+"5";
              break;
            }
            case 6:
            {
              key=key+"6";
              break;
            }
            case 7:
            {
              key=key+"7";
              break;
            }
            case 8:
            {
              key=key+"8";
              break;
            }
            case 9:
            {
              key=key+"9";
              break;
            }
            case 10:
            {
              key=key+"A";
              break;
            }
            case 11:
            {
              key=key+"B";
              break;
            }
            case 12:
            {
              key=key+"C";
              break;
            }
            case 13:
            {
              key=key+"D";
              break;
            }
            case 14:
            {
              key=key+"E";
              break;
            }
            case 15:
            {
              key=key+"F";
              break;
            }
        }
        num=num+4;
    }
    return key;
}

void PC2_Permutation(Block &block)
{
    vector <char> keyMerged;

    char pc2[48]=
    {
        14,17,11,24,1,5,
        3,28,15,6,21,10,
        23,19,12,4,26,8,
        16,7,27,20,13,2,
        41,52,31,37,47,55,
        30,40,51,45,33,48,
        44,49,39,56,34,53,
        46,42,50,36,29,32
    };

    for(int i=0;i<48;i++)
    {
        keyMerged.push_back(block.getContent()[pc2[i]-1]);
    }

    block.setContent(keyMerged);

}

vector<char> P_Permutation(vector<char> key)
{
    vector <char> keyMerged;

    char pc[32]=
    {
        16,7,20,21,
        29,12,28,17,
        1,15,23,26,
        5,18,31,10,
        2,8,24,14,
        32,27,3,9,
        19,13,30,6,
        22,11,4,25
    };

    for(int i=0;i<32;i++)
    {
        keyMerged.push_back(key[pc[i]-1]);
    }

    return keyMerged;

}

vector<vector<char> > PrepareSBoxes()
{
    char s1[64]=
    {
        14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
        0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
        4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
        15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
    };

    char s2[64]=
    {
        15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
        3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
        0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
        13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
    };

    char s3[64]=
    {
        10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
        13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
        13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
        1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
    };

    char s4[64]=
    {
        7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
        13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
        10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
        3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
    };

    char s5[64]=
    {
        2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
        14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
        4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
        11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
    };

    char s6[64]=
    {
        12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
        10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
        9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
        4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
    };

    char s7[64]=
    {
        4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
        13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
        1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
        6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
    };

    char s8[64]=
    {
        13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
        1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
        7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
        2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
    };

    vector <vector <char> > blocks;
    vector <char> empty,temp;
    for(int i=0;i<64;i++)
    temp.push_back(s1[i]);
    blocks.push_back(temp);
    temp=empty;
    for(int i=0;i<64;i++)
    temp.push_back(s2[i]);
    blocks.push_back(temp);
    temp=empty;
    for(int i=0;i<64;i++)
    temp.push_back(s3[i]);
    blocks.push_back(temp);
    temp=empty;
    for(int i=0;i<64;i++)
    temp.push_back(s4[i]);
    blocks.push_back(temp);
    temp=empty;
    for(int i=0;i<64;i++)
    temp.push_back(s5[i]);
    blocks.push_back(temp);
    temp=empty;
    for(int i=0;i<64;i++)
    temp.push_back(s6[i]);
    blocks.push_back(temp);
    temp=empty;
    for(int i=0;i<64;i++)
    temp.push_back(s7[i]);
    blocks.push_back(temp);
    temp=empty;
    for(int i=0;i<64;i++)
    temp.push_back(s8[i]);
    blocks.push_back(temp);
    return blocks;
}

vector <char> sBoxes (vector <char> key, vector <vector <char> > boxes)
{
    vector <char> result;
    int num=0;
    for(int i=0;i<8;i++)
    {
        vector <char> rowTemp;
        vector <char> columnTemp;
        rowTemp.push_back(key[num]);
        rowTemp.push_back(key[num+5]);
        for(int j=0;j<4;j++)
        columnTemp.push_back(key[num+1+j]);
        int row=0;
        int column=0;
        int temp=2;
        for(int j=0;j<2;j++)
        {
            if(rowTemp[j]==1)
            row=row+temp;
            temp=temp/2;
        }
        temp=8;
         for(int j=0;j<4;j++)
         {
            if(columnTemp[j]==1)
            column=column+temp;
            temp=temp/2;
         }
        int iresult=boxes[i][(row*16)+column];

        temp=8;
        for(int j=0;j<4;j++)
        {
            if(temp<=iresult)
            {
                result.push_back(1);
                iresult-=temp;
            }
            else
            result.push_back(0);
            temp=temp/2;
        }
        num+=6;
    }
    return result;
}


vector<char> finPermutation(vector <char> block)
{
    int num=0;
    int num2=40;
    vector <char> blockCopy,newContent;
    for(unsigned int i=0;i<block.size();i++)
    {
        blockCopy.push_back(block[i]);
        newContent.push_back(block[i]);
    }

    for(int j=0;j<8;j++)
    {
        for(int i=0;i<8;i++)
        {
            newContent[num]=blockCopy[num2-1];
            num++;
            if((num%2)==1)
            num2-=32;
            else
            num2+=40;
        }
        num2-=33;
    }

    return newContent;
}


vector<char> E_Permutation (vector <char> key)
{
    vector <char> blockCopy,newContent;
    for(unsigned int i=0;i<key.size();i++)
    {
        blockCopy.push_back(key[i]);
    }

    int num=32;
    for(int i=0;i<48;i++)
    {
        newContent.push_back(blockCopy[num-1]);
        if(num==32)
        num=0;
        num+=1;
        if(i%6==5)
        num-=2;
    }
    return newContent;
}

vector<Block> CharArraysToBlocks (vector <Block> tablica)
{
    vector <Block> tabFin;
    for(unsigned int i=0;i<tablica.size();i++)
    {
        vector <char> tabBlock;
        for(unsigned int j=0;j<tablica[i].size();j++)
        {
            int rest;

            switch (tablica[i].getContent()[j])
            {
                case '0':
                {
                    rest=0;
                    break;
                }
                case '1':
                {
                    rest=1;
                    break;
                }
                case '2':
                {
                    rest=2;
                    break;
                }
                case '3':
                {
                    rest=3;
                    break;
                }
                case '4':
                {
                    rest=4;
                    break;
                }
                case '5':
                {
                    rest=5;
                    break;
                }
                case '6':
                {
                    rest=6;
                    break;
                }
                case '7':
                {
                    rest=7;
                    break;
                }
                case '8':
                {
                    rest=8;
                    break;
                }
                case '9':
                {
                    rest=9;
                    break;
                }
                case 'A':
                {
                    rest=10;
                    break;
                }
                case 'B':
                {
                    rest=11;
                    break;
                }
                case 'C':
                {
                    rest=12;
                    break;
                }
                case 'D':
                {
                    rest=13;
                    break;
                }
                case 'E':
                {
                    rest=14;
                    break;
                }
                case 'F':
                {
                    rest=15;
                    break;
                }
            }



            if((rest/8)>0)
            {
                tabBlock.push_back(1);
                rest=rest-8;
            }
            else
            tabBlock.push_back(0);
            if((rest/4)>0)
            {
                tabBlock.push_back(1);
                rest=rest-4;
            }
            else
            tabBlock.push_back(0);
            if((rest/2)>0)
            {
                tabBlock.push_back(1);
                rest=rest-2;
            }
            else
            tabBlock.push_back(0);
            if((rest/1)>0)
            {
                tabBlock.push_back(1);
                rest=rest-1;
            }
            else
            tabBlock.push_back(0);
        }
        Block block(tabBlock);
        tabFin.push_back(block);
    }
    return tabFin;
}

void splitKey (vector <char> key,vector <char> &C,vector <char> &D)
{
    for(int i=0;i<28;i++)
    {
        C.push_back(key[i]);
    }

    for(int i=28;i<56;i++)
    {
        D.push_back(key[i]);
    }
}

void splitWord (vector <char> key,vector <char> &L,vector <char> &R)
{
    for(int i=0;i<32;i++)
    {
        L.push_back(key[i]);
    }

    for(int i=32;i<64;i++)
    {
        R.push_back(key[i]);
    }
}

vector <char> returnShiftedKey (vector <char> key, int shifts)
{
    for (int i=0;i<shifts;i++)
    {
        char first;
        first=key[0];
        for(unsigned int j=0;j<key.size()-1;j++)
        {
            key[j]=key[j+1];
        }
        key[key.size()-1]=first;
    }

    return key;
}

void createKeyPairs (vector<char> keyC, vector<char> keyD, vector<Block> &conC,vector<Block> &conD)
{
    for(int i=0;i<16;i++)
    {
        switch(i)
        {
            case 0:
            {
                Block blockC(returnShiftedKey(keyC,1));
                conC.push_back(blockC);
                Block blockD(returnShiftedKey(keyD,1));
                conD.push_back(blockD);
                break;
            }
            case 1:
            case 8:
            case 15:
            {
                Block blockC (returnShiftedKey(conC[i-1].getContent(),1));
                conC.push_back(blockC);
                Block blockD (returnShiftedKey(conD[i-1].getContent(),1));
                conD.push_back(blockD);
                break;
            }
            default:
            {
                Block blockC(returnShiftedKey(conC[i-1].getContent(),2));
                conC.push_back(blockC);
                Block blockD(returnShiftedKey(conD[i-1].getContent(),2));
                conD.push_back(blockD);
                break;
            }
        }
    }
}

vector <char> mergeKey (vector <char> C, vector <char> D)
{
    vector <char> mergedKey;
    for(unsigned int i=0;i<C.size();i++)
    mergedKey.push_back(C[i]);
    for(unsigned int i=0;i<D.size();i++)
    mergedKey.push_back(D[i]);

    return mergedKey;
}

vector <char> XORKey (vector <char> R, vector <char> key)
{
    vector <char> XORedKey;
    for(int i=0;i<R.size();i++)
    {
        if((R[i]==0 && key[i]==0) || (R[i]==1 && key[i]==1))
        XORedKey.push_back(0);
        else
        XORedKey.push_back(1);
    }
    return XORedKey;
}

void createSubKeys (vector<Block> C,vector<Block> D, vector<Block> &keyCon)
{
    keyCon.pop_back();
    for(int i=0;i<16;i++)
    {
        vector <char> mergedKey;
        for(unsigned int j=0;j<C[i].size();j++)
        mergedKey.push_back(C[i].getContent()[j]);
        for(unsigned int j=0;j<D[i].size();j++)
        mergedKey.push_back(D[i].getContent()[j]);
        Block block(mergedKey);
        PC2_Permutation(block);
        keyCon.push_back(block.getContent());
    }
}

vector<char> iterations(vector<char> R,vector<char> L,vector <Block> keys, vector <vector <char> > boxes,Operation op)
{
    vector<vector <char> > LCon,RCon;
    LCon.push_back(L);
    RCon.push_back(R);
    int j;
    if(op==encryption)
    j=0;
    else
    j=15;
    for(int i=0;i<16;i++)
    {
        vector <char> temp;
        temp=E_Permutation(RCon[i]);
        temp=XORKey(temp,keys[j].getContent());
        temp=sBoxes(temp,boxes);
        temp=P_Permutation(temp);
        temp=XORKey(temp,LCon[i]);
        RCon.push_back(temp);
        LCon.push_back(RCon[i]);
        if(op==encryption)
        j++;
        else
        j--;
    }
    vector<char> result;
    for(int i=0;i<RCon[16].size();i++)
    result.push_back(RCon[16][i]);
    for(int i=0;i<LCon[16].size();i++)
    result.push_back(LCon[16][i]);
    return result;
}

int main()
{
    string key,word;
    ofstream output;
    unsigned int op=0;
    cout<<"Wybierz opcje: "<<endl;
    cout<<"1. Enkrypcja."<<endl;
    cout<<"2. Dekrypcja."<<endl;
    while(op<1 || op>2)
    {
        cin>>op;
    }
    output.open ("out.txt");
    ifstream input("in.txt");
    while (input.good())
    {
        input>>key;
        input.ignore();
        getline(input,word);
        vector<Block> keyB,wordB,conC,conD;
        keyB=stringToCharArrays(key);
        wordB=stringToCharArrays(word);
        keyB=CharArraysToBlocks(keyB);
        wordB=CharArraysToBlocks(wordB);
        PC1_Permutation(keyB[0]);
        vector <char> keyC,keyD;
        splitKey(keyB[0].getContent(),keyC,keyD);
        createKeyPairs(keyC,keyD,conC,conD);
        createSubKeys(conC,conD,keyB);
        vector <vector <char> > boxes=PrepareSBoxes();
        string napis="";
        for(int i=0;i<wordB.size();i++)
        {
            initPermutation(wordB[i]);
            vector <char> wordL,wordR;
            splitWord(wordB[i].getContent(),wordL,wordR);
            vector <char> result;
            if(op==1)
            result=iterations(wordR,wordL,keyB,boxes,encryption);
            else
            result=iterations(wordR,wordL,keyB,boxes,decryption);
            result=finPermutation(result);
            napis=napis+resultToHex(result);
        }
        output<<napis<<endl;
    }
    input.close();
    output.close();
    return 0;
}
