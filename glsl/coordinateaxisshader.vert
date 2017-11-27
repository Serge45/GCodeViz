//#version 130
attribute vec3 qt_Vertex;
uniform mat4 qt_ModelViewProjectionMatrix;
uniform vec4 qt_InColor;
varying vec4 qt_Color;

void main(void)
{
    gl_Position = qt_ModelViewProjectionMatrix * vec4(qt_Vertex, 1.0);
    qt_Color = qt_InColor;
}

