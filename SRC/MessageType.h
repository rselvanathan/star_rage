/*
This enum keeps all the different types of messages which can be passed between classes. Used for 
Message Dispatching

Author : Romesh Selvanathan

Date : 3/05/12
*/

#ifndef __MESSAGETYPE_H__
#define __MESSAGETYPE_H__

enum MessageType
{
	msg_playerNormalShot,
	msg_playerSprint_On,
	msg_playerSprint_Off,
	msg_playerPickedHealth,
	msg_playerPickedEnergy,
	msg_playerHasBeenAttacked,
	msg_playerCollidesAsteroid,

	msg_enemyShoutHelp,

	msg_elasticCollision,

	msg_decalBlood,
	msg_decalBrokenGlass
};

#endif