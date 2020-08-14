/**
 * @file shaderSource.hpp
 * @author noasakurajin (noasakurajin@web.de)
 * @brief The source code for the default shader
 * @version 0.1.1
 * @date 2020-07-23
 * 
 * @copyright Copyright (c) 2020
 * @license This file is licensed under the LGPL v3 license.
 */

#pragma once

namespace redhand {

    const char DefaultFragmentSource[] =
        "#version 330 core\
        \n \
        \n out vec4 FragColor;\
        \n \
        \n in vec3 vertColor;\
        \n in vec2 TexCoord;\
        \n \
        \n uniform int textureMode;\
        \n uniform float colorAlpha;\
        \n uniform vec2 textureScale;\
        \n uniform sampler2D tex1;\
        \n \
        \n void main(){\
        \n     if(textureMode == 1){\
        \n         FragColor = texture(tex1, vec2(textureScale.x * TexCoord.x,textureScale.y * TexCoord.y)) * vec4(vertColor, colorAlpha);\
        \n     }else{\
        \n         FragColor = vec4(vertColor, colorAlpha);\
        \n     }\
        \n }\
        \0";

    const char DefaultVertexSource[] =
        "#version 330 core\
        \n \
        \n layout (location = 0) in vec3 aPos;\
        \n layout (location = 1) in vec3 aColor;\
        \n layout (location = 2) in vec2 aTexCoord;\
        \n \
        \n out vec3 vertColor;\
        \n out vec2 TexCoord;\
        \n \
        \n uniform mat4 worldTransformation;\
        \n uniform mat4 camera;\
        \n uniform mat4 projection;\
        \n \
        \n void main(){\
        \n     gl_Position = projection * camera * worldTransformation * vec4(aPos.xyz, 1.0f);\
        \n     vertColor = aColor;\
        \n     TexCoord = aTexCoord;\
        \n }\
        \0";

} // namespace redhand
