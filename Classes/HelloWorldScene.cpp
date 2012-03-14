//#pragma warning(disable:4786)
#include <json/writer.h>
#include <json/reader.h>
#include <json/value.h>
#include <iostream>
#include <string>
#include <fstream>
#include "HelloWorldScene.h"

int stageTracker;

enum {
	kTagMenu = 1,
	kTagBgSprite = 2,
};

#define MENU_FONT_COLOR ccc3(32,32,64)

// cc2d namespace
USING_NS_CC;

CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::node();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
  stageTracker = 0;

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	CCMenuItemImage *pCloseItem = CCMenuItemImage::itemFromNormalImage(
										"CloseNormal.png",
										"CloseSelected.png",
										this,
										menu_selector(HelloWorld::menuCloseCallback) );
	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);

	/////////////////////////////
	// 3. add your codes below...
	//////////////////////////////
	
  //
  // Setup menu
  //
  CCMenuItemFont* prevStage = CCMenuItemFont::itemFromString ("Prev", this, menu_selector(HelloWorld::prevStage) );
  CCMenuItemFont* nextStage = CCMenuItemFont::itemFromString ("Next", this, menu_selector(HelloWorld::nextStage) );

  prevStage->setColor(MENU_FONT_COLOR);
  nextStage->setColor(MENU_FONT_COLOR);
  
  CCMenu* menu = CCMenu::menuWithItems(prevStage, nextStage, 0);
  menu->setPosition(ccp(100,100));
  nextStage->setPosition(ccp(100, 0));


  //
  // Setup background sprite
  //
	CCSprite *bgSprite = CCSprite::spriteWithFile("background.jpg");
  
  // So we can access easily later
  bgSprite->setTag(kTagBgSprite);

	// position the sprite on the center of the screen
	bgSprite->setAnchorPoint(ccp(0,0));
  bgSprite->setScale(1.0);
  
	// add background node  & menu to layer
	this->addChild(bgSprite, 0);
  this->addChild(menu);
  
	return true;
}


//
// JSON stream/parse
//
void HelloWorld::switchToStage(int stageToGoTo)
{
  // this JSON es no bueno - it's just a demo placeholder
  // code to 
  // 4 records
  string story_json = "[\"stage:\",{\"moveBgVertically\":-751,\"moveBgHorizontally\":-51,\"zoomFactor\":1,\"duration\":2.2},\"stage:\",{\"moveBgVertically\":-1,\"moveBgHorizontally\":-116,\"zoomFactor\":1,\"duration\":2.2},\"stage:\",{\"moveBgVertically\":-751,\"moveBgHorizontally\":-131,\"zoomFactor\":1,\"duration\":2.2},\"stage:\",{\"moveBgVertically\":-685,\"moveBgHorizontally\":-78,\"zoomFactor\":1,\"duration\":2.2}]";


  Json::Value root;
  Json::Reader reader;
  
  bool parsedSuccess = reader.parse(story_json, root, false);
  
  if ( !parsedSuccess )
  {
     // report to the user the failure and their locations in the document.
    CCLog("Failed to parse JSON");
  } 
  else 
  { 
    float xCoord     = root[stageToGoTo]["moveBgHorizontally"].asDouble(),
          yCoord     = root[stageToGoTo]["moveBgVertically"].asDouble(),
          duration   = root[stageToGoTo]["duration"].asDouble(),
          zoomFactor = root[stageToGoTo]["zoomFactor"].asDouble();

    CCSprite* node = (CCSprite*)getChildByTag( kTagBgSprite );

    CCActionInterval*  actionMoveTo  = CCMoveTo::actionWithDuration((ccTime)duration, 
                                               CCPointMake(xCoord, yCoord));
                 
    //CCActionInterval*  actionScaleTo = CCScaleTo::actionWithDuration((ccTime)duration, zoomFactor);
                 
    node->runAction(actionMoveTo);
    //node->runAction(actionScaleTo);
    CCLog("(%f, %f) duration: %f, zoomFactor: %f", yCoord, xCoord, duration, zoomFactor);
  }
  
  // Value Iterator - will use this later to iterate over nodes with dynamic numbers of children i.e., zones, lines of text, etc
  // for( Json::ValueIterator itr = root[index].begin() ; itr != root[index].end() ; itr++ ) {
  //   const Json::Value fields = root[index][itr.key().asString()];      
  //   CCLog("sub tree element: %s value: %s", itr.key().toStyledString().c_str(), fields.toStyledString().c_str());
  // }
}


//
// Switch to next "stage" of the page (i.e., switch paragraphs and pan/zoom to new location)
//
void HelloWorld::nextStage(CCObject* pSender)
{
  CCLog("nextStage stageTracker: %i", stageTracker);
  // quick hack for the demo
  if(stageTracker == 0)
  {
    stageTracker++;
  }
  else
  {
    stageTracker+=2;
  }
  
  switchToStage(stageTracker);

  #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  	exit(0);
  #endif
}


//
// Switch to previous "stage" 
//
void HelloWorld::prevStage(CCObject* pSender)
{
  CCLog("prevStage stageTracker: %i", stageTracker);
  if( stageTracker > 0)
  {
    stageTracker-=2;
    switchToStage(stageTracker);
  }
  
  #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  	exit(0);
  #endif
}

//
// Closes game
//
void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}