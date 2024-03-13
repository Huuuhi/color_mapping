#pragma once
#include <glm/glm.hpp>
#include <cmath>
#include <vector>


const float PI = 3.14159265359f;

void generateSphereVertices(float radius, int numSlices, int numStacks,
    std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<float>& scalarData) {
    /*  radius������İ뾶��
        numSlices�����ȷ����ϵķֶ����������˾��ߵ�������
        numStacks��γ�ȷ����ϵķֶ�����������γ�ߵ�������
        vertices�����ڴ洢���嶥��λ�õ�������
        normals�����ڴ洢���嶥�㷨��������������
        scalarData�����ڴ洢����ÿ������ı������ݵ�������
    */
    float phi, theta;
    glm::vec3 vertex;
    float scalar;
    for (int i = 0; i <= numStacks; i++) {
        phi = PI * i / numStacks;
        for (int j = 0; j <= numSlices; j++) {
            theta = 2 * PI * j / numSlices;
            vertex.x = radius * sin(phi) * cos(theta);
            vertex.y = radius * sin(phi) * sin(theta);
            vertex.z = radius * cos(phi);
            scalar = vertex.x;
            scalarData.push_back(scalar);
            vertices.push_back(vertex);
            glm::vec3 normal = glm::normalize(vertex); // ���ݶ���λ�ü��㷨������
            normals.push_back(normal); // �洢��������
        }
    }
    glm::vec3 topVertex = glm::vec3(0.0f, 0.0f, radius);
    glm::vec3 bottomVertex = glm::vec3(0.0f, 0.0f, -radius);
    vertices.push_back(topVertex);
    vertices.push_back(bottomVertex);
    normals.push_back(glm::normalize(topVertex));
    normals.push_back(glm::normalize(bottomVertex));
    return;
}

void generateSphereIndices(int numSlices, int numStacks,
    std::vector<unsigned int>& indices)
{
    for (int i = 0; i < numStacks; i++) {
        for (int j = 0; j < numSlices; j++) {
            int k1 = i * (numSlices + 1) + j;
            int k2 = k1 + (numSlices + 1);

            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);

            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }
    return;
}
