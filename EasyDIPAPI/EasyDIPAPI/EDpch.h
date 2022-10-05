
#ifndef __ED_PCH__
#define __ED_PCH__

#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <glad/glad.h>
#include <filesystem>

#include "Object.h"
#include "shaders/Shader.h"
//Shader
using RawData = unsigned char;




unsigned int GetTexture(RawData* data, unsigned int imgWidth, unsigned int imgHeight);
RawData* EDNegativeHA(RawData* data, unsigned int imgWidth, unsigned int imgHeight, RawData** outData = nullptr, int nChannels = 3);


bool EDInit();

#endif // !__ED_PCH__



