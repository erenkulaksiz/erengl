#version 330 core
out vec4 FragColor;
in vec3 FragPos;
in vec2 texCoord;
in vec3 Normal;

uniform sampler2D tex0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main() {
	float ambient = 0.05f;

	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - FragPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - FragPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	FragColor = texture(tex0, texCoord) * lightColor * (specular + diffuse + ambient);
}