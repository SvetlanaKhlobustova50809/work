#version 430 core
in vec3 vFragPosition;
in vec2 vTexCoords;
in vec3 vNormal;
uniform int mode;
uniform sampler2D rock_tex; 
uniform sampler2D sand_tex;
uniform sampler2D aqua_tex;
uniform sampler2D snow_tex;

out vec4 out_color;

vec3 smooth_color(in float border, in vec3 prev_col, in vec3 cur_col)
{
    float h = vFragPosition.y;
    if (h >= border + 4.0f)
        return cur_col;

    float p = int(h * 100) % 100;
    vec3 col;
    if (h < border + 1.0f)
        col = (p*cur_col+(400-p)*prev_col) / 400;
    else if (h < border + 2.0f)
        col = ((100+p)*cur_col+(300-p)*prev_col) / 400;
    else if (h < border + 3.0f)
        col = ((200+p)*cur_col+(200-p)*prev_col) / 400;
    else
        col = ((300+p)*cur_col+(100-p)*prev_col) / 400;

    return col;
}

vec4 smooth_tex(in float border, in sampler2D prev_tex, in sampler2D cur_tex)
{
    float h = vFragPosition.y;    
    if (h >= border + 4.0f)
        return texture(cur_tex, vTexCoords);
    
    float p = int(h * 100) % 100;
    vec4 tex;
    if (h < border + 1.0f)
        tex = mix(texture(prev_tex,vTexCoords), texture(cur_tex,vTexCoords), p/400);
    else if (h < border + 2.0f)
        tex = mix(texture(prev_tex,vTexCoords), texture(cur_tex,vTexCoords), 0.25+p/400);
    else if (h < border + 3.0f)
        tex = mix(texture(prev_tex,vTexCoords), texture(cur_tex,vTexCoords), 0.5+p/400);
    else
        tex = mix(texture(prev_tex,vTexCoords), texture(cur_tex,vTexCoords), 0.75+p/400);

    return tex;   
}

float fog_init() // fog setting up
{
    float fog_cord = (gl_FragCoord.z/gl_FragCoord.w)/700.0f;
    float fog_destiny = 6.0;
    float fog = fog_cord * fog_destiny;
    
    return exp(-pow(1.0 / fog, 2.0));   
}

void main()
{
    vec3 lightDir = vec3(1.0f, 1.0f, 0.0f);

    vec3 col = vec3(0.0f, 0.9f, 0.75f);
    vec4 tex = texture(aqua_tex, vTexCoords);
    vec4 fog_col = vec4(1,1,1,1);

    float kd = max(dot(vNormal, lightDir), 0.0);
    float alpha = fog_init(); // alpha mixing with fog

    vec3 snow = vec3(0.8f, 0.8f, 0.8f);
    vec3 sand = vec3(0.6f, 0.6f, 0.3f);
    vec3 aqua = vec3(0.0f, 0.7f, 0.9f);
    vec3 rock = vec3(0.4f, 0.41f, 0.4f);
    vec3 tree = vec3(0.15f, 0.3f, 0.17f);

    // color picker
    // if (vFragPosition.y <= 0){ // aqua
        // col = sand;
        // tex = texture(sand_tex, vTexCoords);
    if (vFragPosition.y <= 2){ // sand
        col = smooth_color(0, sand, sand);
        tex = smooth_tex(0, sand_tex, sand_tex);
    }else if (vFragPosition.y <= 15){ // tree
        col = smooth_color(2.0f, sand, tree);
        tex = smooth_tex(2.0f, sand_tex, rock_tex);
    }else if (vFragPosition.y <= 25){ // rock
        col = smooth_color(15.0f, tree, rock);
        tex = texture(rock_tex, vTexCoords);        
    }else{ // snow
        col = smooth_color(25.0f, rock, snow);
        tex = smooth_tex(25.0f, rock_tex, snow_tex);            
    }

    vec4 color;
    if (mode == 1)
        color = mix(tex, vec4(kd * col, 1.0), 0.3); // tex + color
    else if (mode == 2)
        color =  mix(vec4(abs(vNormal), 1.0f), vec4(kd * col, 1.0), 0.2); // normals + col
    else
        color = vec4(kd * col, 1.0); // color


    out_color = mix(color, fog_col, alpha); // +fog
}