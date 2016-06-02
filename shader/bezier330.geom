#version 330
layout(lines_adjacency) in;
layout(line_strip, max_vertices = 100000) out;

void linearBezierPoint(vec4 p0, vec4 p1,float t)
{
    vec4 erg = (1.0-t) * p0.xyzw + (t * p1.xyzw);

    gl_Position = erg;
    EmitVertex();
    EndPrimitive();
}

void quadratischerBezierPunkt(vec4 p0, vec4 p1, vec4 p2, float t)
{   //emt : Eins minus t
    float emt = 1.0 - t;
    float emt2 = emt * emt;
    float t2 = t * t;

    vec4 erg = emt2 * p0.xyzw + 2. * t * emt * p1.xyzw + t2 * p2.xyzw;

    gl_Position = erg;
    EmitVertex();
    //EndPrimitive();
}

void kubischerBezierPunkt(vec4 p0, vec4 p1, vec4 p2, vec4 p3, float t)
{
    float emt = 1.0 - t;
    float emt2 = emt * emt;
    float emt3 = emt2 * emt;
    float t2 = t * t;
    float t3 = t2 * t;

    vec4 erg = emt3 * p0.xyzw +
            3. * t * emt2 * p1.xyzw +
            3. * t2 * emt * p2.xyzw +
            t3 * p3.xyzw;

    gl_Position = erg;
    EmitVertex();
    //EndPrimitive();
}

void main()
{

    for(float t=0.1; t < 1; t+=0.11)
    {
        kubischerBezierPunkt(gl_in[0].gl_Position , gl_in[1].gl_Position, gl_in[2].gl_Position, gl_in[3].gl_Position, t);
        //quadratischerBezierPunkt(gl_in[0].gl_Position , gl_in[1].gl_Position, gl_in[2].gl_Position, t);
        //linearBezierPoint(gl_in[0].gl_Position, gl_in[1].gl_Position,t);
    }
    EndPrimitive();
}
