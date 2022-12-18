#version 330 core
in vec3 vFragPosition;
in vec2 vTexCoords;
in vec3 vNormal;
uniform int mode;
uniform sampler2D aqua_tex;


out vec4 out_color;

float fog_init() // fog setting up
{
    float fog_cord = (gl_FragCoord.z/gl_FragCoord.w)/700.0f;
    float fog_destiny = 6.0;
    float fog = fog_cord * fog_destiny;
    
    return exp(-pow(1.0 / fog, 2.0));   
}

void main()
{
    vec3 aqua = vec3(0.0f, 0.7f, 0.9f);

    vec3 lightDir = vec3(1.0f, 1.0f, 0.0f);

    vec3 col  = aqua;
    vec4 tex = texture(aqua_tex, vTexCoords);
    vec4 fog_col = vec4(1,1,1,1);


    float kd = max(dot(vNormal, lightDir), 0.0);
    float alpha = fog_init(); // alpha mixing with fog


    if (mode == 1) {
        out_color = mix(tex, vec4(kd * col , 1.0), 0.1); // тексутра + цвет
        out_color = mix(out_color, fog_col, alpha);
    }
    else if (mode == 2){
        out_color =  mix(vec4(abs(vNormal), 1.0f), vec4(kd * col, 1.0), 0.2); // нормали + цвет

    }else{
        out_color = vec4(kd * col , 1.0); // цвет
        out_color = mix(out_color, fog_col, alpha);
    }

    float p = sqrt((80 - vFragPosition.x)*(80 - vFragPosition.x) + (120 - vFragPosition.z)*(120 - vFragPosition.z));
    out_color = out_color*vec4(kd*aqua, 1-p/200.0f);
}