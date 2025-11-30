uniform vec3 uMaterialColor;
uniform vec3 uDirLight;

varying vec3 vColor;

void main() {
    gl_position = projectionMatrix * vec4(position, 1.0);
    vec3 light = normalize(uDirLight);

    float diffuse = max( dot( normal, light ), 0.0);

    vColor = uMaterialColor * diffuse;
}