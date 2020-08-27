// BinDelta.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>


enum Direction 
{ 
    FacingRight = 0, 
    FacingDown = 1,
    FacingLeft = 2, 
    FacingUp = 3 
};

struct HitboxDir
{
    int Left;
    int Right;
    int Top;
    int Bottom;
};

void PushValue(int b, std::vector<unsigned char>* out)
{
    if (b >= 0)
    {
        assert(b < 256);
        out->push_back(b); // little endian
        out->push_back(0);
    }
    else
    {
        int u = 0x10000 + b;
        int low = u & 0xFF;
        out->push_back(low);
        u >>= 8;
        assert(u < 256);
        int high = u & 0xFF;
        out->push_back(high);
    }
}

void PrintUsage()
{
    std::cout << "Usage: lhbm.exe [input file] [output file] [hitboxSizeMultiplierForward] [hitboxMultiplierSides] \n";
    std::cout << "    Example: lhbm.exe input.smc output.smc 3 2\n";
}

int main(int argc, void** argv)
{
    if (argc != 5)
    {
        PrintUsage();
        return -1;
    }

    char* inputFileName = reinterpret_cast<char*>(argv[1]);
    char* outputFileName = reinterpret_cast<char*>(argv[2]);
    char* hitbox1Str = reinterpret_cast<char*>(argv[3]);
    char* hitbox2Str = reinterpret_cast<char*>(argv[4]);

    int ff = atoi(hitbox1Str);
    int hf = atoi(hitbox2Str);

    FILE* pB = nullptr;
    fopen_s(&pB, inputFileName, "rb");

    if (!pB)
    {
        std::cout << "Couldn't open input file " << inputFileName << ".\n";
        return -1;
    }

    // Check size
    fseek(pB, 0, SEEK_END);
    long sizeB = ftell(pB);
    fseek(pB, 0, SEEK_SET);

    bool header;
    if (sizeB == 0x00100000)
    {
        header = false;
    }
    else if (sizeB == 0x00100200)
    {
        header = true;
    }
    else
    {
        std::cout << "ROM file " << inputFileName << " has unexpected size.\n";
        return -1;    
    }

    std::vector<unsigned char> dataB;
    dataB.resize(sizeB);

    fread(dataB.data(), 1, sizeB, pB);

    fclose(pB);

    HitboxDir allDirs[] =
    {
        {0, 25, -8, 8},
        {-16, 16, 0, 15},
        {-25, 0, -8, 8},
        {-16, 16, -15, 0}
    };

    // Enlarge hitboxes
    allDirs[FacingRight].Right *= ff;
    allDirs[FacingRight].Top *= hf;
    allDirs[FacingRight].Bottom *= hf;

    allDirs[FacingDown].Bottom *= ff;
    allDirs[FacingDown].Left *= hf;
    allDirs[FacingDown].Right *= hf;

    allDirs[FacingLeft].Left *= ff;
    allDirs[FacingLeft].Top *= hf;
    allDirs[FacingLeft].Bottom *= hf;

    allDirs[FacingUp].Top *= ff;
    allDirs[FacingUp].Left *= hf;
    allDirs[FacingUp].Right *= hf;

    // Transfer hitbox info into byte data
    std::vector<unsigned char> hitboxBytes;

    for (int i = 0; i < 4; ++i)
    {
        PushValue(allDirs[i].Left, &hitboxBytes);
        PushValue(allDirs[i].Right, &hitboxBytes);
        PushValue(allDirs[i].Top, &hitboxBytes);
        PushValue(allDirs[i].Bottom, &hitboxBytes);
    }

    std::cout << "Patching: \n";
    for (int i = 0; i < hitboxBytes.size(); ++i)
    {
        std::cout << std::hex << std::uppercase << ((int)hitboxBytes[i]) << " ";
        if (i % 8 == 7)
        {
            std::cout << "\n";
        }
    }

    // Patch the new tables in
    int destOffset = 0xB710;

    if (header)
    {
        destOffset += 0x200;
    }

    for (size_t i = 0; i < hitboxBytes.size(); ++i)
    {
        dataB[destOffset + i] = hitboxBytes[i];
    }

    pB = nullptr;
    fopen_s(&pB, outputFileName, "wb");

    if (!pB)
    {
        std::cout << "Couldn't open output file " << inputFileName << ".\n";
        return -1;
    }

    fwrite(dataB.data(), 1, dataB.size(), pB);
    fclose(pB);

    std::cout << "Done.\n";
    return 0;
}