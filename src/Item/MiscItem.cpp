#include "Item\MiscItem.h"

MiscItem::MiscItem(Items item, TextureHolder & textureHolder, int ID){
	this->item = item;
	this->ID = ID;
	sprite.setTexture(*textureHolder.getTexture((Textures::ID)(item + Textures::Arrow)));
	setPosition(3, 3);
}

MiscItem::~MiscItem(){

}

void MiscItem::draw(sf::RenderTarget& target, sf::RenderStates states)const{
	states.transform *= getTransform();
	target.draw(sprite, states);
}