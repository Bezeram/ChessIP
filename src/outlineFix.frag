uniform sampler2D texture;

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    
    // Discard almost-transparent pixels to avoid white outlines
    if (color.a < 0.9)
        discard;

    gl_FragColor = color;
}