# Rift Warrior (iOS)
![Rift Warrior](https://github.com/likexx/riftwarrior/blob/master/code/projects/riftwarrior/proj.ios/152.png?raw=true "Icon")<br/>
**The source code for iOS game "Rift Warrior" (Yes, I'm the developer and product owner)** <br/>
The original game can be downloaded from the link: [Rift Warrior at AppStore](https://itunes.apple.com/app/id634847542). <br/><br/>
**Preview GamePlay Video on Youtube**
[![GamePlay Video](https://raw.githubusercontent.com/likexx/riftwarrior/master/screenshot.jpg)](http://www.youtube.com/watch?v=ttRaKQFSFJs) <br/><br/>
**Preview Game Introduction Video on Tudou(For China)**
[![GamePlay Video](https://raw.githubusercontent.com/likexx/riftwarrior/master/tudou_screenshot.jpg)](http://video.tudou.com/v/XODA0MDk2MTYw.html?spm=a2h0k.8191414.0.0&from=s1.8-1-1.2&f=22077663) <br/><br/>
All account specific logic&data have been removed, such as In-App-Purchase, Google Ads, Social Network Sharing, and Flurry tracking. All others remain the same.

## Current Issues
* On my 2015 Mackbook Pro (8GB), it runs extremely slow on XCode 9 simulator (but runable), and may cause occational crash. On a 2015 iMac with 16G memory, everything seems fine. So try it yourself on a better dev machine.

## Build and Run in Xcode
Use the latest Xcode (tested with Xcode 9) and open:  **code/projects/riftwarrior/proj.ios/riftwarrior.xcodeproj**

## Necessary Tools
* The game map was created/modified using [Tiled Map Editor](http://www.mapeditor.org/) -- I'm not sure if the latest tiled map editor is supported by cocos2d-x 2.2.6. If not, try find the editor released in 2014
* The opening scene anaimation was done by Cocos Builder [link](https://github.com/cocos2d/CocosBuilder)
* The original game data (configurations) was encrypted using sqlcipher. Now it's just a simple sqlite db located at **code/projects/riftwarrior/Resources/db/game.db**

## Code Structure

Game entry point: AppDelegate.cpp -> startGame() <br/><br/>

GameScene: Base Layer containing MainUI, WorldMap and GameStage.<br/>
GameStage: Kind of a "ShowStage" to place map, tower(building), and actors(enemy/player). This is similar to the "stage" concept in Flash game. <br/>
StageMap: Handling all map related data

## Component Based Design
One the core issues in tower defense (or RTS) game design is how to construct code to handle different attacking weapons with various attack styles. The answer is to use component based pattern.<br/><br/>

Taking "Building.h" for example. Building is kind of a container which contains different weapon. In the code, each "building" contains a "AttackComponent", and we inherit "AttackComponent" to implement different concrete attack weapons. The building itself doesn't care how the weapon attacks.<br/>
Same applies for Player.h
<br/><br/>
In our later game "Rise of the Orcs", we further moved to have MoveComponent, AttackComponent and AIComponent to implement more complicated logic in various aspects. For this tower defense game, it's roughly good enough to handle.

