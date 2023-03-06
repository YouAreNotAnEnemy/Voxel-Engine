#include "MouseListener.h"

ButtonMap MouseListener::buttons = ButtonMap();
ButtonMap MouseListener::buttonsDown = ButtonMap();
ButtonMap MouseListener::buttonsHolding = ButtonMap();
ButtonMap MouseListener::buttonsReleased = ButtonMap();

double MouseListener::scrollX = 0;
double MouseListener::scrollY = 0;

double MouseListener::xPos = 0;
double MouseListener::yPos = 0;

double MouseListener::lastX = 0;
double MouseListener::lastY = 0;

bool MouseListener::isDragging = false;
bool MouseListener::isMoving = false;