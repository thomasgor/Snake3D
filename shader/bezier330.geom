#version 330
layout(lines) in;
layout(points, max_vertices = 1) out;

void linearBezierPoint(vec4 p0, vec4 p1,float t)
{
    vec4 erg;
    erg.x = (1.0-t) * p0.x + (t * p1.x);
    erg.y = (1.0-t) * p0.y + (t * p1.y);
    erg.z = (1.0-t) * p0.z + (t * p1.z);
    gl_Position = erg;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    gl_Position = gl_in[0].gl_Position;
    gl_PointSize= 100;

    EmitVertex();
    EndPrimitive();

        vec4 p0 = gl_in[0].gl_Position;
        vec4 p1 = gl_in[1].gl_Position;

    for(float t = 0; t <= 1; t+= 0.2)
    {
        vec4 erg;
        erg.x = (1.0-t) * p0.x + (t * p1.x);
        erg.y = (1.0-t) * p0.y + (t * p1.y);
        erg.z = (1.0-t) * p0.z + (t * p1.z);
        gl_Position = erg;
        gl_PointSize= 100;

        EmitVertex();
        EndPrimitive();

    }
}
