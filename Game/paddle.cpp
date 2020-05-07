#include "paddle.h"

void Paddle::FirePressed()
{
	array<Entity ^> ^attachments = this->GetAttachments();
	for(int i = 0; i < attachments->Length; i++)
	{
		//--------------------------------------------------
		// BALL LOGIC
		//--------------------------------------------------
		if(Ball::typeid == attachments[i]->GetType())
		{
			attachments[i]->Velocity.X = attachments[i]->Velocity.X + (0.4 * this->Velocity.X);
			attachments[i]->Velocity.Y = -5;

			this->UnAttach(attachments[i]);
		}
	}
}