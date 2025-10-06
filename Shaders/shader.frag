#version 330 core

in vec4 vCol;
in vec2 TexCoord;
out vec4 colour;

uniform vec3 lightColour;
uniform sampler2D texture2D;

void main()
{
    // แสงรอบ ๆ (Ambient Light)
    float ambientStrength = 0.3f;   // 30%
    vec3 ambient = ambientStrength * lightColour;

    // คำนวณสีสุดท้าย = สี texture * สีแสง
    colour = texture(texture2D, TexCoord) * vec4(ambient, 1.0);
}
