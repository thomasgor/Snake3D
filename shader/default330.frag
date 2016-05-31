#version 330
#extension GL_ARB_separate_shader_objects : enable
// default330.frag: a simple fragment shader
//
// notes:
// GL_ARB_separate_shader_objects is not needed for version >= 410
uniform sampler2D texture;
layout(location = 0)out vec4 fragColor; // must be at 0
layout(location = 0)in vec4 texC;

void main() {
    fragColor = vec4(1.0,0.0,0.0,1.0);

    //fragColor = texture2D(texture,vec2(texC.x,texC.y));
}
