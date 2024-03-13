#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
//uniform vec3 objectColor;

void main()
{
    vec3 objectColor;
    float x = FragPos.x;

    if (x < -0.5) 
    {
        objectColor = vec3(1.0, 0.0, 0.0);  // 红色
    } 
    else if (x < 0.0) 
    {
        objectColor = vec3(1.0, 0.5, 0.0);  // 橙色
    } else if (x < 0.5) 
    {
        objectColor = vec3(1.0, 1.0, 0.0);  // 黄色
    } else 
    {
        objectColor = vec3(0.0, 1.0, 0.0);  // 绿色
    }

    float lineFrequency = 0.5;  // 等值线频率
    float linePattern = mod(x, lineFrequency);

    if (linePattern < 0.005 || linePattern > 0.005 + lineFrequency) {
        objectColor = vec3(0.0, 0.0, 0.0);  // 黑色，虚线部分
    }
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
} 