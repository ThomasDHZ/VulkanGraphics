#include "Sprite.h"

Sprite::Sprite()
{

}

Sprite::Sprite(VulkanDevice DeviceInfo, glm::ivec2 position)
{
	SpriteTexture = Texture2D(DeviceInfo, SpriteSheetLocation);
	Position = position;
}
