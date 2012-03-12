//#pragma warning(disable:4786)
#include <json/writer.h>
#include <json/reader.h>
#include <json/value.h>
#include <iostream>
#include <string>
#include <fstream>
#include "HelloWorldScene.h"

using namespace std;

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
  // CC2D-X Style File Reading - Need to add headers for IOS, etc. See 
  // 
  // std::string textFileName = "story.json";
  // textFileName = CCFileUtils::fullPathFromRelativePath(textFileName.c_str());
  // 
  // CCFileData data(textFileName.c_str(), "r");//r parametr depend on file type and operation - http://www.cplusplus.com/reference/clibrary/cstdio/fopen/
  // unsigned long nSize = data.getSize();
  // unsigned char* pBuffer = data.getBuffer();
  // 
  // bool parsedSuccess = reader.parse(pBuffer, root, false);
  //
  // if (!parsedSuccess) {
  //   CCLog("JSON Parsing Failure");  
  // }

  string json_example = "{\"array\":[\"item1\", \"item2\"], \"title\":\"Cocos2d-X... Meet JSON\"}";

  // Let's parse it    
  Json::Value root;
  Json::Reader reader;

  bool parsedSuccess = reader.parse(json_example, root, false);

  if ( !parsedSuccess ){
     // report to the user the failure and their locations in the document.
    CCLog("Failed to parse JSON");
    return 1;
  }

  // Let's extract the array that is contained inside the root object
  const Json::Value array = root["array"];

  // And print its values
  for ( int index = 0; index < array.size(); ++index ){  // Iterates over the sequence elements.
    CCLog("Element in array: %s", array[index].asString().c_str());
  }
  
  // Grab the title 
  std::string title = root["title"].asString();
  CCLog("TITLE: %s", title.c_str());

  // Pretty JSON output
  CCLog("Json Example pretty print: %s", root.toStyledString().c_str());

	// add a label pulled from our JSON file
	// create and initialize a label
  CCLabelTTF* pLabel = CCLabelTTF::labelWithString(title.c_str(), "Arial", 28);
	// ask director the window size
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	// position the label on the center of the screen
	pLabel->setPosition( ccp(size.width / 2, size.height - 50) );

	// add the label as a child to this layer
	this->addChild(pLabel, 1);

	// add "HelloWorld" splash screen"
	CCSprite* pSprite = CCSprite::spriteWithFile("HelloWorld.png");

	// position the sprite on the center of the screen
	pSprite->setPosition( ccp(size.width/2, size.height/2) );

	// add the sprite as a child to this layer
	this->addChild(pSprite, 0);
	
	return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
