
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    ZOOMIN,
    ZOOMOUT
};

// Default camera values
const float SPEED       =  2.5f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Camera options
    float MovementSpeed;
    float Zoom;
    float Radius;
    float CameraVerticalDegree;
    float CameraHorizontalDegree;
    
    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : Front(glm::vec3(0.0f, 0.0f, 0.0f)), MovementSpeed(SPEED), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        updateCameraVectors();
        CameraVerticalDegree = 0;
        CameraHorizontalDegree = 0;
        Radius = 20;
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ) : Front(glm::vec3(0.0f, 0.0f, 0.0f)), MovementSpeed(SPEED), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        updateCameraVectors();
        CameraVerticalDegree = 0;
        CameraHorizontalDegree = 0;
        Radius = 20;
    }
    
    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, glm::vec3(0.0,0.0,0.0), Up);
    }
    
    // Position calculator
    glm::vec3 pos_calc_y(glm::vec3 cur_pos, float velocity, float r, float dir)
    {
        glm::vec3 y_move = glm::vec3(0.0,10.0,0.0) * velocity * dir;
        glm::vec3 p_l = cur_pos + y_move;
        if (Position.x == 0)
        {
            CameraHorizontalDegree += velocity * dir;
            float camY = sin(CameraHorizontalDegree) * r;
            float camZ = cos(CameraHorizontalDegree) * r;
            std::cout << camY << " " << camZ << std::endl;
            return glm::vec3(Position.x, camY, camZ);
        }
        float a = pow(Radius,2) - pow(p_l.y,2);
        float c = p_l.z/p_l.x;
        float x1 = sqrt(a/(1+pow(c,2)));
        float x2 = -sqrt(a/(1+pow(c,2)));
        float z1 = c * x1;
        float z2 = c * x2;
        if (pow(p_l.x-x1,2)+pow(p_l.z-z1,2)<pow(p_l.x-x2,2)+pow(p_l.z-z2,2))
        {
            // x1
            return glm::vec3(x1,p_l.y,z1);
        }
        else{
            // x2
            return glm::vec3(x2,p_l.y,z2);
        }
    }
    
    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        float y = Position.y;
        float r = sqrt(pow(Radius,2) - pow(y,2));
        glm::vec3 pos;
        if (direction == FORWARD && Position.y < 14)
        {
                Position = pos_calc_y(Position, velocity, r, 1.0);
        }
        if (direction == BACKWARD && Position.y > -14)
        {
                Position = pos_calc_y(Position, velocity, r, -1.0);
        }
        if (direction == LEFT)
        {
            CameraVerticalDegree -= velocity;
            float camX = sin(CameraVerticalDegree) * r;
            float camZ = cos(CameraVerticalDegree) * r;
            Position = glm::vec3(camX, Position.y, camZ);
        }
        if (direction == RIGHT)
        {
            CameraVerticalDegree += velocity;
            float camX = sin(CameraVerticalDegree) * r;
            float camZ = cos(CameraVerticalDegree) * r;
            Position = glm::vec3(camX, Position.y, camZ);
        }
        if (direction == ZOOMIN)
        {
            if (Zoom >= 10.0f)
                Zoom -= velocity * 10;
            if (Zoom <= 10.0f)
                Zoom = 10.0f;
        }
        if (direction == ZOOMOUT)
        {
            if (Zoom <= 70.0f)
                Zoom += velocity * 10;
            if (Zoom >= 70.0f)
                Zoom = 70.0f;
        }
    }
    
private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front(0.0,0.0,1.0);
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif
