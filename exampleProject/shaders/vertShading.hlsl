precision highp float;
varying vec3 fPosition;
varying vec3 fNormal;

const float ambient_coeff   = 0.25;
const float specular_coeff  = 1.0;
const float specular_exp    = 32.0;
const vec3  light_direction = vec3(-1.0,1.0,1.0); // stationary light
const vec3  light_color     = vec3(1.0,1.0,1.0);
const vec3  object_color    = vec3(1.0,0.6,0.0); // yellow-ish orange

void main()
{
  vec3 l = normalize(light_direction);
  vec3 n = normalize(fNormal);
  vec3 e = normalize(-fPosition);
  vec3 h = normalize (e+l);

  vec3 ambient_color  = ambient_coeff  * object_color;
  vec3 specular_color = specular_coeff * pow(max(0.0,dot(n,h)),specular_exp) * light_color;

  gl_FragColor = vec4(ambient_color+specular_color, 1.0);
}