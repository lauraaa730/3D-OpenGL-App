#pragma once
#include "pgr.h"
#include "SceneObjectsData.h"
#include "FlatShadedObject.h"

enum Colors {
	RED,
	PURPLE,
	BLUE,
	YELLOW,
	GREEN,
	WHITE,
	COLOR_COUNT
};

struct Crystal {
	FlatShadedObject * crystal;
	unsigned int id;
	unsigned int currColor;
};

const unsigned int crystalsNum = 3;


void setColor(FlatShadedObject * crystal, unsigned int color) {
	switch (color) {
	case RED :
		crystal->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
		break;
	case PURPLE:
		crystal->setColor(glm::vec3(1.0f, 0.0f, 1.0f));
		break;
	case BLUE:
		crystal->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
		break;
	case YELLOW:
		crystal->setColor(glm::vec3(1.0f, 1.0f, 0.0f));
		break;
	case GREEN:
		crystal->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	case WHITE:
		crystal->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		break;

	default :
		;
	}
}

void changeColor(Crystal* obj) {
	obj->currColor = (obj->currColor + 1) % COLOR_COUNT;
	setColor(obj->crystal, obj->currColor);

}
