//----------------------------------------------------------------------------------------
/**
 * \file    ColorChangingCrystals.h
 * \author  Laura Katerina Dudkova
 * \date    2026/05/11
 * \brief   Defines structures and functions for interactive, color-changing crystal objects.
 */

#pragma once
#include "pgr.h"
#include "SceneObjectsData.h"
#include "FlatShadedObject.h"

 /**
  * \brief Enumeration of available colors for the interactable crystals.
  */
enum Colors {
	RED,
	PURPLE,
	BLUE,
	YELLOW,
	GREEN,
	WHITE,
	COLOR_COUNT   ///< \brief Total number of available colors used for modulo cycling.
};

/**
 * \brief Structure representing an interactable color-changing crystal.
 */
struct Crystal {
	FlatShadedObject* crystal; ///< \brief Pointer to the flat-shaded visual representation of the crystal.
	unsigned int id;            ///< \brief Unique identifier for the crystal (used for stencil picking).
	unsigned int currColor;     ///< \brief Current color state of the crystal based on the Colors enum.
};

const unsigned int crystalsNum = 3; ///< \brief Total number of color-changing crystals in the scene.

/**
 * \brief Set a specific color to a flat-shaded crystal object.
 * \details Uses the Colors enum index to apply the corresponding RGB vector to the object's color property.
 * \param crystal Pointer to the FlatShadedObject to modify.
 * \param color The target color index from the Colors enum.
 */
void setColor(FlatShadedObject* crystal, unsigned int color) {
	switch (color) {
	case RED:
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

	default:
		;
	}
}

/**
 * \brief Cycle the crystal to its next color state.
 * \details Increments the currColor index and wraps around using COLOR_COUNT, then applies the newly calculated color.
 * \param obj Pointer to the Crystal structure to update.
 */
void changeColor(Crystal* obj) {
	obj->currColor = (obj->currColor + 1) % COLOR_COUNT;
	setColor(obj->crystal, obj->currColor);
}
