// Copyright(c) 2018 Seth Ballantyne <seth.ballantyne@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files(the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and / or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
#pragma once
#include "sprite.h"
#include "vector2.h"

using namespace System;
using namespace System::Collections::Generic;

/// <summary>
/// Base class for all entities within the game. Bricks, the paddle, balls and powerups
/// are all entities.
/// </summary>
public ref class Entity abstract : System::IEquatable<Entity ^>, ICloneable
{
private:
    // the surface and frame data associated with this entity.
    Sprite ^sprite = nullptr;

    // the current velocity of the entity.
    Vector2 velocity;

    // entity attached to this instance; these will move when the Entity
    // they're attached to moves.
    List<Entity ^> ^attachments = gcnew List<Entity ^>();

    // current movement speed of the entity.
    float speed = 0.0f;

    // true if this entity is attached to another, otherewise false.
    bool attached = false;

    // name of the entity as specified by the entity's name attribute in the xml file.
    String ^name = nullptr;

    // bounding box that's calculated based on the values in the entities sprite object.
    // this updates when the sprite moves and its frames change.
    // this is in screen coordinates; the coordinates in sprite->Frame->BoundingBox are in image coordinates.
    System::Drawing::Rectangle boundingBox;

    void UpdatePosition(int x, int y)
    {
		//this->Pos
		//this->Position.Y = y;
    }
public:
    /// <summary>
    /// Creates a new <see cref="Entity" /> instance.
    /// </summary>
    /// <param name="sprite">the entity's sprite.</param>
    /// <param name="velocity">the entity's initial velocity.</param>
    /// <exception cref="System::ArgumentNullException"><c>sprite</c> evaluates to <b>null</b>.</exception>
    Entity(Sprite ^sprite, Vector2 velocity, String ^name)
    {
        if(nullptr == sprite)
        {
            throw gcnew ArgumentNullException("sprite");
        }

        this->sprite = sprite;
        this->velocity = velocity;
		this->name = name;
        this->boundingBox.X = sprite->Position.X + sprite->CurrentFrame->BoundingBox.X;
        this->boundingBox.Y = sprite->Position.Y + sprite->CurrentFrame->BoundingBox.Y;
        this->boundingBox.Width = sprite->CurrentFrame->BoundingBox.Width;
        this->boundingBox.Height = sprite->CurrentFrame->BoundingBox.Height;
    }

    /// <summary>
    /// Attaches the specified entity to the current instance.
    /// </summary>
    /// <param name="entityToAttach">the entity to attach to this instance.</param>
    /// <param name="x">the position to attach <c>entityToAttach</c></param>
    /// <param name="y"></param>
    /// <exception cref="System::ArgumentNullException"></exception>
    /// <exception cref="System::ArgumentException"></exception>
    void Attach(Entity ^entityToAttach, int x, int y)
    {
        if(nullptr == entityToAttach)
        {
            throw gcnew ArgumentNullException("entityToAttach");
        }
        else if(attachments->Contains(entityToAttach))
        {
            throw gcnew ArgumentException("attaching an entity that's already attached.");
        }
		else if(entityToAttach->Attached)
		{
			throw gcnew ArgumentException("attaching an entity that's already attached to another entity.");
		}

        entityToAttach->Velocity = this->velocity;

		// update the attachments position in the game based on the specified local coordinates
		int screenX = this->BoundingBox.X + x;
		int screenY = this->BoundingBox.Y + y;

		entityToAttach->LocalCoordinates = System::Drawing::Point(x, y);
        entityToAttach->SetPosition(screenX, screenY);

        this->attachments->Add(entityToAttach);
        entityToAttach->Attached = true;
    }

    /// <summary>
    /// Unattaches the specified entity from this instance.
    /// </summary>
    /// <param name="entityToUnAttach"></param>
    /// <exception cref="System::ArgumentNullException"><c>entityToUnAttach</c> evaluates to <b>null</b>.</exception>
    void UnAttach(Entity ^entityToUnAttach)
    {
        if(nullptr == entityToUnAttach)
        {
            throw gcnew ArgumentNullException("entityToUnAttach");
        }

		attachments->Remove(entityToUnAttach);
        entityToUnAttach->Attached = false;
    }

    /// <summary>
    /// Gets the image coordinates of each frame and its associated bounding box.
    /// </summary>
    /// <returns>An array of Frame objects.</returns>
    array<Frame ^, 1>^ GetFrames()
    {
        return this->sprite->GetFrames();
    }

	/// <summary>
	/// Sets the Entity's frame to the specified frame and updates its boundingbox.
	/// </summary>
	void SetFrame(unsigned int frame)
	{
		if(frame >= this->sprite->GetFrames()->Length || frame < 0)
		{
			return;
		}

		this->sprite->CurrentFrameIndex = frame;
		this->boundingBox.X = sprite->Position.X + sprite->CurrentFrame->BoundingBox.X;
		this->boundingBox.Y = sprite->Position.Y + sprite->CurrentFrame->BoundingBox.Y;
		this->boundingBox.Width = sprite->CurrentFrame->BoundingBox.Width;
		this->boundingBox.Height = sprite->CurrentFrame->BoundingBox.Height;
	}

    /// <summary>
    /// Creates a deep copy of the current object.
    /// </summary>
    /// <returns></returns>
    virtual Object^ Clone() abstract;

    /// <summary>
    /// 
    /// </summary>
   virtual void Update() abstract;

    /// <summary>
    /// Computes and returns the hashcode of the current object.
    /// </summary>
    /// <returns>the objects hashcode as a signed 32-bit integer.</returns>
    int GetHashCode() override
    {
        int hash = 7;

        hash *= 37 + sprite->GetHashCode();
        hash *= 37 + velocity.GetHashCode();
        hash *= 37 + attachments->GetHashCode();
        hash *= 37 + speed.GetHashCode();
        hash *= 37 + attached.GetHashCode();
        hash *= 37 + name->GetHashCode();
        hash *= 37 + boundingBox.GetHashCode();

        return hash;
    }

    /// <summary>
    /// Returns a value indicating whether the current <see cref="Entity"/> instance 
    /// is equal to the specified object.
    /// </summary>
    /// <param name="obj">the object to compare against.</param>
    /// <returns><b>true</b> if this instance and <c>obj</c> are equal, otherwise <b>false</b>.</returns>
    bool Equals(Object ^obj) override
    {
        if(nullptr == obj)
        {
            return false;
        }

        if(obj->GetType() != GetType())
        {
            return false;
        }

        return Equals(safe_cast<Entity ^>(obj));
    }

    /// <summary>
    /// Returns a value indicating whether the current <see cref="Entity"/> instance is equal to the specified object.
    /// </summary>
    /// <param name="entity">the object to compare against.</param>
    /// <returns><b>true</b> if this instance and <c>obj</c> are equal, otherwise <b>false</b>.</returns>
    bool virtual Equals(Entity ^entity)
    {
        if(nullptr == entity || this->GetType() != entity->GetType())
        {
            return false;
        }

        // and do the comparison
        //return Object::Equals(entity) && (Barby == other.Barby);
        if(entity->Sprite == sprite &&
            entity->Velocity == velocity &&
            entity->Speed == speed &&
            entity->Attached == attached &&
            entity->Name == name &&
            entity->BoundingBox == boundingBox) //&&
            //Equals(entity))
        {
            return true;
        }

        return false;
    }

    /// <summary>
    /// Gets an array containing the entities attached to this object.
    /// </summary>
    /// <returns></returns>
    array<Entity ^, 1> ^GetAttachments()
    {
        return this->attachments->ToArray();
    }

    /// <summary>
    /// 
    /// </summary>
    void Move()
    {

    }

	/// <summary>
	/// Unattaches everything attached to the entity.
	/// </summary>
	void RemoveAttachments()
	{
		for(int i = 0; i < this->attachments->Count; i++)
		{
			this->attachments[i]->attached = false;
		}

		this->attachments->Clear();
	}

    /// <summary>
    /// Updates the screen coordinates of the entity.
    /// </summary>
    /// <param name="x">the position on the X axis.</param>
    /// <param name="y">the position on the Y axis.</param>
    void SetPosition(int x, int y)
    {
        this->sprite->Position.X = x;
        this->sprite->Position.Y = y;

        this->BoundingBox.X = this->sprite->Position.X + this->sprite->CurrentFrame->BoundingBox.X;
        this->BoundingBox.Y = this->sprite->Position.Y + this->sprite->CurrentFrame->BoundingBox.Y;

		for(int i = 0; i < this->attachments->Count; i++)
		{
			int attachmentX = this->Sprite->Position.X + this->attachments[i]->LocalCoordinates.X;
			int attachmentY = this->Sprite->Position.Y + this->attachments[i]->LocalCoordinates.Y;

			this->attachments[i]->SetPosition(attachmentX, attachmentY);
		}
    }

    /// <summary>
    /// Returns a value indicating whether the current instance is not equal to another instance
    /// of <see cref="Entity"/>.
    /// </summary>
    /// <param name="entity">the object to compare against.</param>
    /// <returns><b>true</b> if the objects don't match, otherwise <b>false</b>.</returns>
    bool operator != (Entity ^entity)
    {
        return !Equals(entity);
    }

    /// <summary>
    /// Returns a value indicating whether the current instance is equal to another instance
    /// of <see cref="Entity"/>.
    /// </summary>
    /// <param name="entity">the object to compare against.</param>
    /// <returns><b>true</b> if the objects match, otherwise <b>false</b>.</returns>
    bool operator == (Entity ^entity)
    {
        return Equals(entity);
    }

    /// <summary>
    /// Gets a reference to the bounding box for the entity's current frame.
    /// </summary>
    property System::Drawing::Rectangle% BoundingBox
    {
        System::Drawing::Rectangle% get()
        {
            return boundingBox;
        }
    }

    /// <summary>
    /// Gets the entity's <see cref="Sprite"/> object.
    /// </summary>
    property ::Sprite ^Sprite
    {
        ::Sprite ^get()
        {
            return sprite;
        }
    }

    /// <summary>
    /// Gets or sets the entity's position on the screen.
    /// </summary>
    /*property System::Drawing::Point Position
    {
        System::Drawing::Point get()
        {
            return this->sprite->Position;
        }

        void set(System::Drawing::Point value)
        {
            SetPosition(value.X, value.Y);
        }
    }*/

    /// <summary>
    /// Gets or sets the entity's current velocity.
    /// </summary>
    property Vector2% Velocity
    {
        Vector2% get()
        {
            return velocity;
        }

        void set(Vector2% value)
        {
            this->velocity = value;
        }
    }

    /// <summary>
    /// Gets or sets the entity's current speed in pixels.
    /// </summary>
    property float Speed
    {
        float get()
        {
            return speed;
        }

        void set(float value)
        {
            speed = value;
        }
    }

    /// <summary>
    /// Gets or sets a value indicating whether this object is attached to another <see cref="Entity"/>.
    /// </summary>
    property bool Attached
    {
        bool get()
        {
            return attached;
        }

        void set(bool value)
        {
            attached = value;
        }
    }

    /// <summary>
    /// Gets the name of the object as specified by the entity's <c>name</c> attribute in <c>entities.xml</c>.
    /// </summary>
    property String ^Name
    {
        String ^get()
        {
            return name;
        }
    }

	System::Drawing::Point LocalCoordinates = System::Drawing::Point(0,0);
};