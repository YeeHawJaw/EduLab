//
// Created by Karol Hetman on 15/03/2023.
//

#ifndef EDULAB_SCENE2D_H
#define EDULAB_SCENE2D_H

#include "core.h"


class Object2D {
private:
    b2Body* body_{};
    ImVec4 color;
    b2Vec2 size;
    bool showForces = false;
    bool showVelocity = false;
public:
    Object2D(b2Body* body, ImColor color, b2Vec2 size);

    // getter methods
    void set_show_forces(bool show) { showForces = show; }
    void set_show_velocity(bool show) { showVelocity = show; }

    bool is_showing_forces() { return showForces; }
    bool is_showing_velocity() { return showVelocity; }


    b2Body* get_body() { return body_; }
    ImVec4 get_color() { return color; }
    b2Vec2 get_size() { return size; }
    b2Vec2 set_size(b2Vec2 newSize) { size = newSize; }
    ImVec4 set_color(ImVec4 newColor);
    static json get_stats();
    std::vector<b2Vec2> get_forces();
    b2Vec2 get_velocity();
    void add_force(b2Vec2 force);
    void change_density(float density);
    float get_mass();
    b2Vec2 get_position();


    void resize(b2Vec2 newSize)

    {
        b2Body* oldBody = body_;
        b2World* world = oldBody->GetWorld();
        float newWidth = newSize.x;
        float newHeight = newSize.y;

        // Store the old body's properties
        b2Vec2 position = oldBody->GetPosition();
        float angle = oldBody->GetAngle();
        b2BodyType bodyType = oldBody->GetType();
        b2Fixture* fixture = oldBody->GetFixtureList();

        // Destroy the old body
        world->DestroyBody(oldBody);

        // Create a new body with the new size
        b2BodyDef bodyDef;
        bodyDef.type = bodyType;
        bodyDef.position.Set(position.x, position.y);
        bodyDef.angle = angle;
        b2Body* newBody = world->CreateBody(&bodyDef);

        if (fixture->GetType() == b2Shape::e_circle)
        {
            // Create a new circle shape with the new radius
            auto* circleShape = dynamic_cast<b2CircleShape*>(fixture->GetShape());
            float newRadius = (newWidth); // average of new width and height
            circleShape->m_radius = newRadius;

            // Create a new fixture with the new circle shape
            b2FixtureDef fixtureDef;
            fixtureDef.shape = circleShape;
            fixtureDef.density = fixture->GetDensity();
            fixtureDef.friction = fixture->GetFriction();
            fixtureDef.restitution = fixture->GetRestitution();
            newBody->CreateFixture(&fixtureDef);
            set_size(b2Vec2(newRadius, newRadius));
        }
        else if (fixture->GetType() == b2Shape::e_polygon)
        {
            // Create a new polygon shape with the new width and height
            b2PolygonShape newShape;
            newShape.SetAsBox(newWidth / 2.0f, newHeight / 2.0f);

            // Create a new fixture with the new polygon shape
            b2FixtureDef fixtureDef;
            fixtureDef.shape = &newShape;
            fixtureDef.density = fixture->GetDensity();
            fixtureDef.friction = fixture->GetFriction();
            fixtureDef.restitution = fixture->GetRestitution();
            newBody->CreateFixture(&fixtureDef);
            set_size(b2Vec2(newWidth, newHeight));
        }
    }

    void changeAttributes(b2Body* body, float newFriction, float newDensity, float newRestitution)
    {
        // Get the body's fixture list
        b2Fixture* fixture = body->GetFixtureList();

        // Get the body's current properties

        fixture->SetFriction(newFriction);
        fixture->SetDensity(newDensity);
        fixture->SetRestitution(newRestitution);
    }
    void rotate(float angle) {
        body_->SetTransform(body_->GetPosition(), angle);
    }
};




struct Camera {
    float x, y; // Position of the camera
    float zoom; // Zoom level of the camera
    float sX, sY; // Screen size
    void sUpdate(float newX, float newY) { // Screen update
        this->sX = newX;
        this->sY = newY;
    }

    // Constructor to initialize the camera's position and zoom
    Camera(float x, float y, float zoom, float sX = 0, float sY = 0)
            : x(x), y(y), zoom(zoom), sX(sX), sY(sY) {}


    // Function to print the camera's position and zoom level
    void printInfo() const {
        std::cout << "Camera position: (" << x << ", " << y << ")\n";
        std::cout << "Camera zoom level: " << zoom << std::endl;
    }
};

class Scene2D {
public:
    explicit Scene2D(std::string name);

    // getter methods
    b2World* get_world() { return world_; }
    std::vector<Object2D*>& get_objects() { return objects_; }
    Camera* get_camera() { return &camera_; }
    [[nodiscard]] const std::string& get_name() const { return name_; }

    void MouseDown(const b2Vec2 &p);
    void MouseUp();
    void MouseMove(const b2Vec2 &p);

    void add_object(b2Body* body, ImVec4 color, b2Vec2 size);
    void delete_object(b2Body* body);


    void serialize(std::string filename);
    void deserialize(std::string filename);



private:
    b2World* world_;
    std::vector<Object2D*> objects_;
    Camera camera_ = Camera(0, 0, 10, 0, 0);
    b2Body* groundBody_;
    b2MouseJoint* mouseJoint_ = NULL;
    b2Vec2 mouseWorld_;

    std::string name_;


    void * CreateBox(float x, float y, float width, float height, b2BodyType bodyType, ImVec4 color, float angle,
              float density,
              float friction, float restitution);

    b2Body *CreateCircle(float x, float y, float radius, b2BodyType bodyType, ImVec4 color, float angle, float density,
                         float friction, float restitution);
};



#endif //EDULAB_SCENE2D_H
