#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;

//layout(location = 1) in vec3 v_normal;
layout(location = 1) in vec3 v_color;

layout(location = 2) in vec2 tex_coord;

//layout(location = 3) in vec3 v_color;
layout(location = 3) in vec3 v_normal;

// ex 6  : din cate observ, daca interschimb aici pipe-ul 1 cu pipe-ul 3, in FragmentShader.glsl
// daca folosesc normala vertexilor pe post de culoare, va avea functionalitatea culorii vertexilor
// pe post de culoare dinainte sa schimb pipe-urile.

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_tex_coord;
out vec3 frag_color;


// daca folosesc versiunea 410, pot scrie:
// layout(location = 0) out vec4 vertex_out_attribute_name;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_position = v_position;
    frag_normal = v_normal;
    frag_tex_coord = tex_coord;
    frag_color = v_color;

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
