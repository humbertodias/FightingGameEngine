#include "graphics/Animation.h"
#include <fstream>
#include <arpa/inet.h>



Animation::Animation(){ 
  currentAnimElemIndex = 0;
  currentAnimElemTimePassed = 0;
  animationTime = 0;
  animationTimePassed = 0;
  animFrame = 0;
}

Animation::~Animation(){ }

AnimationStateObj* Animation::saveState(){
  animObj.animFrame = animFrame;
  animObj.animationTimePassed = animationTimePassed;
  animObj.currentAnimElemIndex = currentAnimElemIndex;
  animObj.currentAnimElemTimePassed = currentAnimElemTimePassed;
  
  return &animObj;
}
void Animation::loadState(AnimationStateObj stateObj){
  animFrame = stateObj.animFrame;
  animationTimePassed = stateObj.animationTimePassed;
  currentAnimElemIndex = stateObj.currentAnimElemIndex;
  currentAnimElemTimePassed = stateObj.currentAnimElemTimePassed;
}

void Animation::loadAnimEvents(nlohmann::json json){
  loadAnimEvents(4, json);
}

void Animation::loadAnimEvents(float defaultScale, nlohmann::json json) {
  for (auto i : json.items()) {
    int animTime = i.value().at("time");
    float scale = defaultScale;
    int offsetX = 0;
    int offsetY = 0;

    if(i.value().count("offsetY")){
      offsetY = i.value().at("offsetY");
    }
    if(i.value().count("scale")){
      std::string animScaleStr = i.value().at("scale");
      scale = std::stof(animScaleStr);
      printf("the scale %f\n", scale);
    }
    if(i.value().count("offsetX")){
      offsetX = i.value().at("offsetX");
    }

    AnimationElement element(animTime, offsetX*scale, offsetY);
    if (i.value().count("color")) {
      element.isYellow = true;
    }

    GameTexture* text = &element.gameTexture;
    text->cartesian = true;
    std::string path(i.value().at("file").get<std::string>());
    size_t found = path.find("\%CHARNAME\%");
    if (found != std::string::npos) {
      path = path.replace(found, 10, charName);
      printf("path:%s\n", path.c_str());
    }

    const char* pathPointer = path.c_str();
    std::pair realDimensions = getDimensions(pathPointer);

    if(i.value().count("width")){
      realDimensions.first = i.value().at("width");
    }
    if(i.value().count("height")){
      realDimensions.second = i.value().at("height");
    }
    if (animTime == -1) {
      element.infiniteFrame = true;
    }
    text->loadTexture(pathPointer);
    // TODO: fix scale
    text->setDimensions(0, 0, realDimensions.first*scale, realDimensions.second*scale);
    // text->setColor(255, 0, 0);
    if (offsetX == -1) {
      element.offsetX = ((realDimensions.first * scale)/2);
    }
    if (offsetX == -2) {
      element.offsetX = ((realDimensions.first * scale));
    }
    if (offsetX == -3) {
      element.offsetX = ((realDimensions.first * scale));
    }
    if (offsetY == -1) {
      element.offsetY = ((realDimensions.second * scale)/2);
    }

    animationTime += animTime;
    element.endTime = animationTime;
    animationElements.push_back(element);

    if (element.infiniteFrame) {
      frames.push_back(animationElements.size() - 1);
    } else {
      for (int i = 1; i <= animTime; ++i) {
        frames.push_back(animationElements.size() - 1);
      }
    }
  }
}

void Animation::drawRect(SDL_Rect rect) {
  Camera* cam = graphics->getCamera();
  SDL_SetRenderDrawColor(graphics->getRenderer(), 128, 0, 128, 0xFF);
  rect.x = (rect.x - cam->cameraRect.x);
  rect.y = (rect.y + (graphics->getWindowHeight() - rect.h) - 60) + cam->cameraRect.y;

  SDL_RenderFillRect(graphics->getRenderer(), &rect);
  SDL_SetRenderDrawColor(graphics->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
}

void Animation::render(int x, int y, bool faceRight, int stateTime) {
  int camOffset = graphics->getCamera()->cameraRect.x;
  int frameIndex = 0;
  if (stateTime >= frames.size()) {
    frameIndex = frames[frames.size() - 1];
  } else {
    frameIndex = frames[stateTime];
  }
  AnimationElement* elem = &animationElements.at(frameIndex);

  GameTexture* currentText = &elem->gameTexture;
  int width = currentText->getDimensions().first;
  int offsetX = elem->offsetX;
  int offsetY = elem->offsetY;
  faceRight ? currentText->setCords(x-offsetX, ((y - 60) + offsetY)) : currentText->setCords((x+offsetX)-width, ((y - 60) + offsetY));

  if (hitShake) {
    if (hitShakeToggler == 3) {
    faceRight ? currentText->setCords((x-offsetX) + 10, ((y - 60) + offsetY)) : currentText->setCords(((x+offsetX)-width) - 10, ((y - 60) + offsetY));
    } else if (hitShakeToggler == 6) {
    faceRight ? currentText->setCords((x-offsetX) - 10, ((y - 60) + offsetY)) : currentText->setCords(((x+offsetX)-width) + 10, ((y - 60) + offsetY));
    }
    hitShakeToggler++;
    if (hitShakeToggler == 7) {
      hitShakeToggler = 0;
    }
  }

  if (isRed) {
    currentText->setColor(255, 0, 0);
  } 
  if (isGreen) {
    currentText->setColor(0, 128, 0);
  } 
  if (elem->isYellow) {
    currentText->setColor(255, 255, 0);
  } 
  if (isLight) {
    currentText->setColor(173,216,230);
  }
  currentText->render(faceRight);
  if (isRed || isLight || isGreen || elem->isYellow) {
    currentText->setColor(255, 255, 255);
  }
}

void Animation::renderHitspark(int x, int y, bool faceRight) {
  int camOffset = graphics->getCamera()->cameraRect.x;
  AnimationElement* elem = &animationElements.at(currentAnimElemIndex);

  GameTexture* currentText = &elem->gameTexture;
  int width = currentText->getDimensions().first;
  int offsetX = elem->offsetX;
  int offsetY = elem->offsetY;

  faceRight ? currentText->setCords(x+offsetX, ((y - 60) + offsetY)) : currentText->setCords(x-offsetX, ((y - 60) + offsetY));
  currentText->render(faceRight);

  currentAnimElemTimePassed++;
  animationTimePassed++;
  if(animationTimePassed == elem->endTime){
    if (currentAnimElemIndex+1 != animationElements.size()) {
      currentAnimElemIndex++;
    }
  } 
}

void Animation::setAnimTime(int time) {
  animationTimePassed = time;
}

void Animation::shake(int duration) {
}

void Animation::setAnimElemIndex(int index) {
  currentAnimElemIndex = index;
}

void Animation::resetAnimEvents() {
  animFrame = 0;
  animationTimePassed = 0;
  currentAnimElemIndex = 0;
  currentAnimElemTimePassed = 0;
}

int Animation::timeRemaining() {
  return animationTime - animationTimePassed;
}

std::pair<int, int> Animation::getDimensions(const char* path) {
    std::pair<int, int> returnPair;
    unsigned int width, height;

    std::ifstream img(path);

    // width and height are offset by 16 bytes
    // ty TCP training, everything has a TLV
    img.seekg(16);
    img.read((char *)&width, 4);
    img.read((char *)&height, 4);

    returnPair.first = ntohl(width);
    returnPair.second = ntohl(height);
    img.close();

    return returnPair;
}
