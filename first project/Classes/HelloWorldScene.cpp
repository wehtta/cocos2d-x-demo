#include "HelloWorldScene.h"

using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
		
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(HelloWorld::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

//         // 2. Add a label shows "Hello World".
// 
//         // Create a label and initialize with string "Hello World".
//         CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
//         CC_BREAK_IF(! pLabel);
// 
//         // Get window size and place the label upper. 
//         CCSize size = CCDirector::sharedDirector()->getWinSize();
//         pLabel->setPosition(ccp(size.width / 2, size.height - 50));
// 
//         // Add the label to HelloWorld layer as a child layer.
//         this->addChild(pLabel, 1);
// 
//         // 3. Add add a splash screen, show the cocos2d splash image.
//         CCSprite* pSprite = CCSprite::create("HelloWorld.png");
//         CC_BREAK_IF(! pSprite);
// 
//         // Place the sprite on the center of the screen
//         pSprite->setPosition(ccp(size.width/2, size.height/2));
// 
//         // Add the sprite to HelloWorld layer as a child layer.
//         this->addChild(pSprite, 0);

		// Add the sprite 

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCSprite *player = CCSprite::create("player.png");
	
		CC_BREAK_IF(! player);
			
		player->setPosition(ccp(player->getContentSize().width/2,
				winSize.height/2));
		this->addChild(player);


        bRet = true;
    } while (0);

	   this->schedule(schedule_selector(HelloWorld::gameLogic), 1.0);
	   this->setTouchEnabled(true);

       return bRet;

}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void HelloWorld::addTarget()
{
	CCSprite *target = CCSprite::create("monster1.png");
	if(! target)
		return ;

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	int minY = target->getContentSize().height/2;
	int maxY = winSize.height;

	int rangeY = maxY -minY;
	int actualY = (rand()%rangeY) + minY;

	target->setPosition(ccp(winSize.width + (target->getContentSize().width/2),
		actualY));
	this->addChild(target);

	int minDuration = 2.0;
	int maxDuration = 4.0;
	int rangeDuration = maxDuration - minDuration;
	int actualDuration = (rand()%rangeDuration) + minDuration;

	CCFiniteTimeAction *actionMove = CCMoveTo::actionWithDuration(
		(float)actualDuration, ccp(0 - target->getContentSize().width/2, actualY));

	CCFiniteTimeAction *actionMoveDone = CCCallFuncN::actionWithTarget(this,
		callfuncN_selector(HelloWorld::spriteMoveFinished));

	target->runAction(CCSequence::actions(actionMove, actionMoveDone, NULL));

}

void HelloWorld::spriteMoveFinished(CCNode *sender)
{
	CCSprite *sprite =(CCSprite*) sender;
	this->removeChild(sprite, true);
}

void HelloWorld::gameLogic(float dt)
{
	this->addTarget();
}

void HelloWorld::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch*)(pTouches->anyObject());
	CCPoint location = touch->getLocationInView();

	location = CCDirector::sharedDirector()->convertToGL(location);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *projectile = CCSprite::create("CloseNormal.png");
	projectile->setPosition(ccp(20, winSize.height/2));

	int offX = location.x - projectile->getPosition().x;
	int offY = location.y - projectile->getPosition().y;

	if(offX <= 0)
		return ;

	this->addChild(projectile);
	int realX = winSize.width;
	float ratio = (float)offY /(float)offX ;
	int realY = realX*ratio + projectile->getPosition().y;
	CCPoint realDest = ccp(realX, realY);

	int offRealX = realX - projectile->getPosition().x;
	int offRealY = realY - projectile->getPosition().y;

	float length = sqrtf(offRealX * offRealX + offRealY*offRealY);
	float velocity = 480/1;
	float realMoveDuration = length/velocity;

	projectile->runAction(
		CCSequence::actions(
		CCMoveTo::actionWithDuration(realMoveDuration, realDest),
		CCCallFuncN::actionWithTarget(this, callfuncN_selector(HelloWorld::spriteMoveFinished)),
		NULL));



}